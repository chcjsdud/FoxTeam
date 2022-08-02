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

    // ViewPort And Projection Matrix Get
    UINT ViewPortNo = 1;
    D3D11_VIEWPORT ViewPort_ = {};
    GameEngineDevice::GetInst().GetContext()->RSGetViewports(&ViewPortNo, &ViewPort_);

    float4x4 ProjMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().Projection_;

    // 1. ��꿡 �ʿ��� ���⺤�� Get
    OriginPos_ = float4::ZERO;

    Direction_.x = ((2.0f * _MousePosX) / ViewPort_.Width - 1.0f) / ProjMat._11;
    Direction_.y = ((-2.0f * _MousePosY) / ViewPort_.Height + 1.0f) / ProjMat._22;
    Direction_.z = 1.0f;

    // 2. View Matrix & Inverse View Matrix Get
    float4x4 ViewMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().View_;
    float4x4 InverseViewMat = ViewMat.InverseReturn();

    // 3. ����(Ray)�� ���念������ �̵�
    OriginPos_ = DirectX::XMVector3TransformCoord(OriginPos_.DirectVector, InverseViewMat.DirectMatrix);
    Direction_ = DirectX::XMVector3TransformNormal(Direction_.DirectVector, InverseViewMat.DirectMatrix);
    Direction_.Normalize3D();

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
            // �˻��ϴ� �浹ü�� ������ �����Ѵٸ� Ž������
            GameEngineCollision* CurCollider = (*StartIter);
            DirectX::XMFLOAT3 CurColliderCenter = CurCollider->GetTransform()->GetCollisionData().Sphere.Center;
            float4 fCenter = float4(CurColliderCenter.x, CurColliderCenter.y, CurColliderCenter.z);
            float Radius = CurCollider->GetTransform()->GetCollisionData().Sphere.Radius;

            // 
            float4 Subtract = OriginPos_ - fCenter;
            float b = float4::Dot3D(Subtract, Direction_);
            float c = float4::Dot3D(Subtract, Subtract) - Radius * Radius;
            if (b * b - c >= 0)
            {
                // ��ġ�� ��ȯ�� ���� ��ġ�� ����
                _PickedPos = CurCollider->GetActor()->GetTransform()->GetLocalPosition();

                // ���� ���õ� Ÿ�� ����
                CurTarget_ = CurCollider->GetActor();

                return true;
            }
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
