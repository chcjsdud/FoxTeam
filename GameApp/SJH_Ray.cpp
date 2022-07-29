#include "PreCompile.h"
#include "SJH_Ray.h"

#include <GameEngineBase/GameEngineMath.h>

#include <GameEngine/GameEngineCore.h>
#include <GameEngine/CameraComponent.h>
#include <GameEngine/GameEngineCollision.h>

float4 SJH_Ray::RayAtViewSpace(float _MousePosX, float _MousePosY)
{
    // 2D 환경에서의 마우스 클릭지점을 이용하여
    // 3D 환경에서의 마우스 좌표와 현재 카메라가 바라보는 방향을 계산한다.
    float4 ScreenSize = float4(1280.f, 720.f);                          // 임시(추후 윈도우크기 가져오는 함수 추가예정)
    
    // 마우스 커서 좌표를 -1에서 +1 범위로 이동합니다
    float PointX = ((2.0f * _MousePosX) / ScreenSize.x) - 1.0f;
    float PointY = (((2.0f * _MousePosY) / ScreenSize.y) - 1.0f) * -1.0f;

    // 뷰포트의 종횡비를 고려하여 투영행렬을 사용하여 클릭좌표(점)을 조정
    float4x4 Proj = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().Projection_;

    float4x4 ProjectionMatrix4;
    float4x4::StoreFloat4x4(&ProjectionMatrix4, Proj);

    PointX = PointX / ProjectionMatrix4._11;
    PointY = PointY / ProjectionMatrix4._22;

    // 뷰행렬의 역함수를 Get
    float4x4 View = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().View_;
    float4x4 InversView = View.InverseReturn();

    float4x4 InversViewMatrix4;
    float4x4::StoreFloat4x4(&InversViewMatrix4, InversView);

    // 뷰공간에서의 Ray(광선)의 방향을 계산
    Direction_.x = (PointX * InversViewMatrix4._11) + (PointY * InversViewMatrix4._21) + InversViewMatrix4._31;
    Direction_.y = (PointX * InversViewMatrix4._12) + (PointY * InversViewMatrix4._22) + InversViewMatrix4._32;
    Direction_.z = (PointX * InversViewMatrix4._13) + (PointY * InversViewMatrix4._23) + InversViewMatrix4._33;

    // 카메라의 위치 Get
    OriginPos_ = GetLevel()->GetMainCamera()->GetTransform()->GetWorldPosition();

    // 월드행렬 Get
    float4x4 Wolrd = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().WorldWorld_;

    // 현재 타겟의 
    //float4x4 TranslateMatrix;
    //TranslateMatrix.Translation(_Target->GetTransform()->GetLocalPosition());





    int a = 0;

    return float4();
}

float4 SJH_Ray::RayAtViewSpace(float4 _MouseClickPos)
{
    // 2D 환경에서의 마우스 클릭지점을 이용하여
    // 3D 환경에서의 마우스 좌표와 현재 카메라가 바라보는 방향을 계산한다.
    float MousePosX = _MouseClickPos.x;
    float MousePosY = _MouseClickPos.y;

    return RayAtViewSpace(MousePosX, MousePosY);
}

void SJH_Ray::Start()
{
}

void SJH_Ray::Update(float _DeltaTime)
{
    // 해당 광선과 현재레벨의 모든 액터와 충돌체크
    

}

SJH_Ray::SJH_Ray()
    : OriginPos_(float4::ZERO)
    , Direction_(float4::ZERO)
{
}

SJH_Ray::~SJH_Ray()
{
}
