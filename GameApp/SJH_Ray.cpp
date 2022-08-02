#include "PreCompile.h"
#include "SJH_Ray.h"

#include <GameEngineBase/GameEngineMath.h>

#include <GameEngine/GameEngineDevice.h>
#include <GameEngine/GameEngineCore.h>
#include <GameEngine/CameraComponent.h>
#include <GameEngine/GameEngineCollision.h>

SJH_Ray* SJH_Ray::RayAtViewSpace(float _MousePosX, float _MousePosY)
{
    // 2D 환경에서의 마우스 클릭지점을 이용하여
    // 3D 환경에서의 마우스 좌표와 현재 카메라가 바라보는 방향을 계산한다.

    // ViewPort And Projection Matrix Get
    UINT ViewPortNo = 1;
    D3D11_VIEWPORT ViewPort_ = {};
    GameEngineDevice::GetInst().GetContext()->RSGetViewports(&ViewPortNo, &ViewPort_);

    float4x4 ProjMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().Projection_;

    // 1. 계산에 필요한 방향벡터 Get
    OriginPos_ = float4::ZERO;

    Direction_.x = ((2.0f * _MousePosX) / ViewPort_.Width - 1.0f) / ProjMat._11;
    Direction_.y = ((-2.0f * _MousePosY) / ViewPort_.Height + 1.0f) / ProjMat._22;
    Direction_.z = 1.0f;

    // 2. View Matrix & Inverse View Matrix Get
    float4x4 ViewMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().View_;
    float4x4 InverseViewMat = ViewMat.InverseReturn();

    // 3. 광선(Ray)을 월드영역으로 이동
    OriginPos_ = DirectX::XMVector3TransformCoord(OriginPos_.DirectVector, InverseViewMat.DirectMatrix);
    Direction_ = DirectX::XMVector3TransformNormal(Direction_.DirectVector, InverseViewMat.DirectMatrix);
    Direction_.Normalize3D();

    return this;
}

SJH_Ray* SJH_Ray::RayAtViewSpace(float4 _MouseClickPos)
{
    // 2D 환경에서의 마우스 클릭지점을 이용하여
    // 3D 환경에서의 마우스 좌표와 현재 카메라가 바라보는 방향을 계산한다.
    float MousePosX = _MouseClickPos.x;
    float MousePosY = _MouseClickPos.y;

    return RayAtViewSpace(MousePosX, MousePosY);
}

bool SJH_Ray::IsPicked(float4& _PickedPos)
{
    // 선택된 타겟 초기화
    CurTarget_ = nullptr;

    // 현재 레벨의 모든 충돌체 목록 Get
    std::map<int, std::list<GameEngineCollision*>> AllList = GetLevel()->GetAllCollision();

    // 해당 광선과 교차하는 충돌체 탐색
    int GroupCnt = AllList.size();
    for (int Group = 0; Group < GroupCnt; ++Group)
    {
        std::list<GameEngineCollision*>::iterator StartIter = AllList[Group].begin();
        std::list<GameEngineCollision*>::iterator EndIter = AllList[Group].end();
        for (; StartIter != EndIter; ++StartIter)
        {
            // 검사하는 충돌체가 광선과 교차한다면 탐색종료
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
                // 위치값 반환을 위해 위치값 저장
                _PickedPos = CurCollider->GetActor()->GetTransform()->GetLocalPosition();

                // 현재 선택된 타겟 저장
                CurTarget_ = CurCollider->GetActor();

                return true;
            }
        }
    }

    // 충돌하는 충돌체가 존재하지않다면 실패
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
