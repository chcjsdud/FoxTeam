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
    OriginPos_ = float4::ZERO;

    // 1. 광선을 뷰포트영역 -> 투영영역
    UINT ViewPortNo = 1;
    D3D11_VIEWPORT ViewPort_ = {};
    GameEngineDevice::GetInst().GetContext()->RSGetViewports(&ViewPortNo, &ViewPort_);
    Direction_.x = (2.0f * _MousePosX) / ViewPort_.Width - 1.0f;
    Direction_.y = (-2.0f * _MousePosY) / ViewPort_.Height + 1.0f;
    Direction_.z = 1.0f;

    // 2. 투영영역 -> 뷰영역
    float4x4 ProjMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().Projection_;
    Direction_.x = Direction_.x / ProjMat._11;
    Direction_.y = Direction_.y / ProjMat._22;

    // 2. 광선을 뷰영역 -> 월드영역
    float4x4 ViewMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().View_;
    float4x4 InverseViewMat = ViewMat.InverseReturn();
    OriginPos_ = DirectX::XMVector3TransformCoord(OriginPos_.DirectVector, InverseViewMat.DirectMatrix);
    Direction_ = DirectX::XMVector3TransformNormal(Direction_.DirectVector, InverseViewMat.DirectMatrix);
    Direction_.Normalize3D();

    //// 4. 광선을 월드영역 -> 로컬영역
    //float4x4 WorldMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().WorldWorld_;
    //float4x4 InversWorldMat = WorldMat.InverseReturn();
    //OriginPos_ = DirectX::XMVector3TransformCoord(OriginPos_.DirectVector, InversWorldMat.DirectMatrix);
    //Direction_ = DirectX::XMVector3TransformNormal(Direction_.DirectVector, InversWorldMat.DirectMatrix);
    //Direction_.Normalize3D();

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
            // 구(Sphere) : 중심(Center), 반지름(Radius)
            // 광선(Ray) : 시작위치(OriginPos), 방향(Direction)
            // 구의 중심과 시작점을 연결하는 벡터 : Connect
            // 1. Connect와 광선의방향을 내적한 값이 0보다 작으면 광선의 시작점은 구를 지나친다
            // 2. Connect의 길이가 구의 반지름보다 작으면 광선의 시작점은 구안에 존재
            // -> Connect와 광선의 방향을 내적한 값이 0보다 클때 교차
            // Connect와 광선의방향 내적한 값 : Cross 라고할때
            // 피타고라스정리에 의해 2개의 거리를 구할수있다.
            // 1. 구의 센터에서 광선의방향벡터까지의 거리
            // 2. (구의 센터에서 광선의방향벡터까지의 거리지점에서 교차점까지의 거리
            // 단, 교차지점이 2개인경우 가까운 거리에 존재하는 교차점의 거리를 반환한다.
            GameEngineCollision* CurCollider = (*StartIter);
            DirectX::XMFLOAT3 CurColliderCenter = CurCollider->GetTransform()->GetCollisionData().Sphere.Center;
            float4 fCenter = float4(CurColliderCenter.x, CurColliderCenter.y, CurColliderCenter.z);
            float Radius = CurCollider->GetTransform()->GetCollisionData().Sphere.Radius;

            // 거리와 교차여부를 판단하기 위한 데이터 계산
            float4 Connect = fCenter - OriginPos_;
            float s = float4::Dot3D(Connect, Direction_);
            float l = float4::Dot3D(Connect, Connect);
            float r = std::pow(Radius, 2);

            // 광선이 구의 반대방향을 향하거나 구를 지나친다.
            if (s < 0 && l > r)
            {
                return false;
            }

            // 광선이 구를 비껴나가는 경우
            float m = l - static_cast<float>(std::pow(s, 2));
            if (m > r)
            {
                return false;
            }

            // 광선의 시작점으로부터 교차점까지의 거리
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

            // 광선 원위치 + (t * 광선방향) = 교차한점의 좌표
            // Dist : 출발점(원위치)로부터 충돌(교차)한 지점까지의 거리
            _PickedPos = OriginPos_ + (Direction_ * Distance);

            return true;
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
