#include "PreCompile.h"
#include "SJH_Ray.h"

#include <GameEngineBase/GameEngineMath.h>

#include <GameEngine/GameEngineDevice.h>
#include <GameEngine/GameEngineCore.h>
#include <GameEngine/CameraComponent.h>
#include <GameEngine/GameEngineCollision.h>

SJH_Ray* SJH_Ray::RayAtViewSpace(float _MousePosX, float _MousePosY)
{
    // 2D ȯ�濡���� ���콺 Ŭ�������� �̿��Ͽ�
    // 3D ȯ�濡���� ���콺 ��ǥ�� ���� ī�޶� �ٶ󺸴� ������ ����Ѵ�.
    OriginPos_ = float4(0.f, 0.f, 0.f, 0.f);

    // 1. ������ ����Ʈ���� -> ��������
    UINT ViewPortNo = 1;
    D3D11_VIEWPORT ViewPort_ = {};
    GameEngineDevice::GetInst().GetContext()->RSGetViewports(&ViewPortNo, &ViewPort_);
    float PointX = ((2.0f * _MousePosX) / ViewPort_.Width) - 1.0f;
    float PointY = (((2.0f * _MousePosY) / ViewPort_.Height) - 1.0f) * -1.0f;
    float PointZ = 1.0f;

    // 2. ������ �������� -> �俵��
    float4x4 ProjMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().Projection_;
    PointX = PointX / ProjMat._11;
    PointY = PointY / ProjMat._22;

    // 3. ������ �俵�� -> ���念��
    float4x4 ViewMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().View_;
    float4x4 InverseViewMat = ViewMat.InverseReturn();
    Direction_.x = (PointX * InverseViewMat._11) + (PointY * InverseViewMat._21) + (PointZ * InverseViewMat._31);
    Direction_.y = (PointX * InverseViewMat._12) + (PointY * InverseViewMat._22) + (PointZ * InverseViewMat._32);
    Direction_.z = (PointX * InverseViewMat._13) + (PointY * InverseViewMat._23) + (PointZ * InverseViewMat._33);
    Direction_.w = 0.0f;
    Direction_.Normalize3D();

    OriginPos_.x = InverseViewMat._41;
    OriginPos_.y = InverseViewMat._42;
    OriginPos_.z = InverseViewMat._43;
    OriginPos_.w = 0.0f;

    return this;
}

SJH_Ray* SJH_Ray::RayAtViewSpace(float4 _MouseClickPos)
{
    // 2D ȯ�濡���� ���콺 Ŭ�������� �̿��Ͽ�
    // 3D ȯ�濡���� ���콺 ��ǥ�� ���� ī�޶� �ٶ󺸴� ������ ����Ѵ�.
    float MousePosX = _MouseClickPos.x;
    float MousePosY = _MouseClickPos.y;

    return RayAtViewSpace(MousePosX, MousePosY);
}

bool SJH_Ray::IsPicked(float4& _PickedPos)
{
    // ���õ� Ÿ�� �ʱ�ȭ
    CurTarget_ = nullptr;

    // ���� ������ ��� �浹ü ��� Get
    std::map<int, std::list<GameEngineCollision*>> AllList = GetLevel()->GetAllCollision();

    // �ش� ������ �����ϴ� �浹ü Ž��
    int GroupCnt = AllList.size();
    for (int Group = 0; Group < GroupCnt; ++Group)
    {
        std::list<GameEngineCollision*>::iterator StartIter = AllList[Group].begin();
        std::list<GameEngineCollision*>::iterator EndIter = AllList[Group].end();
        for (; StartIter != EndIter; ++StartIter)
        {
            // ��(Sphere) : �߽�(Center), ������(Radius)
            // ����(Ray) : ������ġ(OriginPos), ����(Direction)
            // ���� �߽ɰ� �������� �����ϴ� ���� : Connect
            // 1. Connect�� �����ǹ����� ������ ���� 0���� ������ ������ �������� ���� ����ģ��
            // 2. Connect�� ���̰� ���� ���������� ������ ������ �������� ���ȿ� ����
            // -> Connect�� ������ ������ ������ ���� 0���� Ŭ�� ����
            // Connect�� �����ǹ��� ������ �� : Cross ����Ҷ�
            // ��Ÿ��������� ���� 2���� �Ÿ��� ���Ҽ��ִ�.
            // 1. ���� ���Ϳ��� �����ǹ��⺤�ͱ����� �Ÿ�
            // 2. (���� ���Ϳ��� �����ǹ��⺤�ͱ����� �Ÿ��������� ������������ �Ÿ�
            // ��, ���������� 2���ΰ�� ����� �Ÿ��� �����ϴ� �������� �Ÿ��� ��ȯ�Ѵ�.
            GameEngineCollision* CurCollider = (*StartIter);

            // ���� �˻��ϴ� ���� ���Ϳ� ������ Get
            DirectX::XMFLOAT3 CurColliderCenter = CurCollider->GetTransform()->GetCollisionData().Sphere.Center;
            float4 fCenter = float4(CurColliderCenter.x, CurColliderCenter.y, CurColliderCenter.z);
            float Radius = CurCollider->GetTransform()->GetCollisionData().Sphere.Radius;

            // �Ÿ��� �������θ� �Ǵ��ϱ� ���� ������ ���
            float4 Connect = fCenter - OriginPos_;
            float s = float4::Dot3D(Connect, Direction_);
            float l = float4::Dot3D(Connect, Connect);
            float r = std::pow(Radius, 2);

            // ������ ���� �ݴ������ ���ϰų� ���� ����ģ��.
            if (s < 0 && l > r)
            {
                return false;
            }

            // ������ ���� �񲸳����� ���
            float m = l - static_cast<float>(std::pow(s, 2));
            if (m > r)
            {
                return false;
            }

            // ������ ���������κ��� ������������ �Ÿ�
            float q = static_cast<float>(std::sqrt(r - m));
            float Distance = 0.0f;
            if (l > r)
            {
                Distance = s - q;
            }
            else
            {
                Distance = s + q;
            }

            // ���� ����ġ + (t * ��������) = ���������� ��ǥ
            // Dist : �����(����ġ)�κ��� �浹(����)�� ���������� �Ÿ�
            _PickedPos = OriginPos_ + (Direction_ * Distance);

            return true;
        }
    }

    // �浹�ϴ� �浹ü�� ���������ʴٸ� ����
    return false;
}

void SJH_Ray::Start()
{
}

void SJH_Ray::Update(float _DeltaTime)
{
}

SJH_Ray::SJH_Ray()
    : OriginPos_(float4::ZERO)
    , Direction_(float4::ZERO)
    , CurTarget_(nullptr)
{
}

SJH_Ray::~SJH_Ray()
{
}
