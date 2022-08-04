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
    OriginPos_ = float4::ZERO;

    // 1. ������ ����Ʈ���� -> ��������
    UINT ViewPortNo = 1;
    D3D11_VIEWPORT ViewPort_ = {};
    GameEngineDevice::GetInst().GetContext()->RSGetViewports(&ViewPortNo, &ViewPort_);
    Direction_.x = (2.0f * _MousePosX) / ViewPort_.Width - 1.0f;
    Direction_.y = (-2.0f * _MousePosY) / ViewPort_.Height + 1.0f;
    Direction_.z = 1.0f;

    // 2. �������� -> �俵��
    float4x4 ProjMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().Projection_;
    Direction_.x = Direction_.x / ProjMat._11;
    Direction_.y = Direction_.y / ProjMat._22;

    // 2. ������ �俵�� -> ���念��
    float4x4 ViewMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().View_;
    float4x4 InverseViewMat = ViewMat.InverseReturn();
    OriginPos_ = DirectX::XMVector3TransformCoord(OriginPos_.DirectVector, InverseViewMat.DirectMatrix);
    Direction_ = DirectX::XMVector3TransformNormal(Direction_.DirectVector, InverseViewMat.DirectMatrix);
    Direction_.Normalize3D();

    //// 4. ������ ���念�� -> ���ÿ���
    //float4x4 WorldMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().WorldWorld_;
    //float4x4 InversWorldMat = WorldMat.InverseReturn();
    //OriginPos_ = DirectX::XMVector3TransformCoord(OriginPos_.DirectVector, InversWorldMat.DirectMatrix);
    //Direction_ = DirectX::XMVector3TransformNormal(Direction_.DirectVector, InversWorldMat.DirectMatrix);
    //Direction_.Normalize3D();

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
