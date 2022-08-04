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
    OriginPos_ = float4(0.f, 0.f, 0.f, 0.f);

    // 1. 광선을 뷰포트영역 -> 투영영역
    UINT ViewPortNo = 1;
    D3D11_VIEWPORT ViewPort_ = {};
    GameEngineDevice::GetInst().GetContext()->RSGetViewports(&ViewPortNo, &ViewPort_);
    float PointX = ((2.0f * _MousePosX) / ViewPort_.Width) - 1.0f;
    float PointY = (((2.0f * _MousePosY) / ViewPort_.Height) - 1.0f) * -1.0f;
    float PointZ = 1.0f;

    // 2. 광선을 투영영역 -> 뷰영역
    float4x4 ProjMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().Projection_;
    PointX = PointX / ProjMat._11;
    PointY = PointY / ProjMat._22;

    // 3. 광선을 뷰영역 -> 월드영역
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
    // 2D 환경에서의 마우스 클릭지점을 이용하여
    // 3D 환경에서의 마우스 좌표와 현재 카메라가 바라보는 방향을 계산한다.
    float MousePosX = _MouseClickPos.x;
    float MousePosY = _MouseClickPos.y;

    return RayAtViewSpace(MousePosX, MousePosY);
}

bool SJH_Ray::IsPicked(float4& _PickedPos)
{
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
            bool CrossCheck = false;

            // 광선의 시작지점에서부터 선택된 충돌체와 교차하는 지점까지의 거리
            float Dist = 0.0f;
            if (true == (*StartIter)->RayCollision((*StartIter)->GetCollisionType(), OriginPos_, Direction_, Dist))
            {
                // 교차한 지점의 좌표를 반환
                _PickedPos = OriginPos_ + (Direction_ * Dist);

                return true;
            }
          
#pragma region 광선교차 공통화작업으로 인한 주석
            //======================= 220804 SJH 임시주석
            //// 현재 검색하는 구의 센터와 반지름 Get
            //DirectX::XMFLOAT3 CurColliderCenter = CurCollider->GetTransform()->GetCollisionData().Sphere.Center;
            //float4 fCenter = float4(CurColliderCenter.x, CurColliderCenter.y, CurColliderCenter.z);
            //float Radius = CurCollider->GetTransform()->GetCollisionData().Sphere.Radius;

            //// 거리와 교차여부를 판단하기 위한 데이터 계산
            //float4 Connect = fCenter - OriginPos_;
            //float s = float4::Dot3D(Connect, Direction_);
            //float l = float4::Dot3D(Connect, Connect);
            //float r = std::pow(Radius, 2);

            //// 광선이 구의 반대방향을 향하거나 구를 지나친다.
            //if (s < 0 && l > r)
            //{
            //    return false;
            //}

            //// 광선이 구를 비껴나가는 경우
            //float m = l - static_cast<float>(std::pow(s, 2));
            //if (m > r)
            //{
            //    return false;
            //}

            //// 광선의 시작점으로부터 교차점까지의 거리
            //float q = static_cast<float>(std::sqrt(r - m));
            //float Distance = 0.0f;
            //if (l > r)
            //{
            //    Distance = s - q;
            //}
            //else
            //{
            //    Distance = s + q;
            //}

            //// 광선 원위치 + (t * 광선방향) = 교차한점의 좌표
            //// Dist : 출발점(원위치)로부터 충돌(교차)한 지점까지의 거리
            //_PickedPos = OriginPos_ + (Direction_ * Distance);

            //return true;
#pragma endregion
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
{
}

SJH_Ray::~SJH_Ray()
{
}
