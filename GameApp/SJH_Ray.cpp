#include "PreCompile.h"
#include "SJH_Ray.h"

#include <GameEngineBase/GameEngineMath.h>

#include <GameEngine/GameEngineCore.h>
#include <GameEngine/CameraComponent.h>
#include <GameEngine/GameEngineCollision.h>

float4 SJH_Ray::RayAtViewSpace(float _MousePosX, float _MousePosY)
{
    // 2D ȯ�濡���� ���콺 Ŭ�������� �̿��Ͽ�
    // 3D ȯ�濡���� ���콺 ��ǥ�� ���� ī�޶� �ٶ󺸴� ������ ����Ѵ�.
    float4 ScreenSize = float4(1280.f, 720.f);                          // �ӽ�(���� ������ũ�� �������� �Լ� �߰�����)
    
    // ���콺 Ŀ�� ��ǥ�� -1���� +1 ������ �̵�
    float PointX = ((2.0f * _MousePosX) / ScreenSize.x) - 1.0f;
    float PointY = (((2.0f * _MousePosY) / ScreenSize.y) - 1.0f) * -1.0f;

    // ����Ʈ�� ��Ⱦ�� ����Ͽ� ��������� ����Ͽ� Ŭ����ǥ(��)�� ����
    float4x4 Proj = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().Projection_;

    float4x4 ProjectionMatrix4;
    float4x4::StoreFloat4x4(&ProjectionMatrix4, Proj);

    PointX = PointX / ProjectionMatrix4._11;
    PointY = PointY / ProjectionMatrix4._22;

    // ������� ���Լ��� Get
    float4x4 View = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().View_;
    float4x4 InversView = View.InverseReturn();

    float4x4 InversViewMatrix4;
    float4x4::StoreFloat4x4(&InversViewMatrix4, InversView);

    // ����������� �������� ����
    Direction_.x = (PointX * InversViewMatrix4._11) + (PointY * InversViewMatrix4._21) + InversViewMatrix4._31;
    Direction_.y = (PointX * InversViewMatrix4._12) + (PointY * InversViewMatrix4._22) + InversViewMatrix4._32;
    Direction_.z = (PointX * InversViewMatrix4._13) + (PointY * InversViewMatrix4._23) + InversViewMatrix4._33;

    //// ī�޶��� ��ġ Get
    //OriginPos_ = GetLevel()->GetMainCamera()->GetTransform()->GetWorldPosition();

    //// ������� Get
    //float4x4 Wolrd = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().WorldWorld_;

    // 
    //float4x4 TranslateMatrix;
    //TranslateMatrix.Translation(_Target->GetTransform()->GetLocalPosition());



    // ���� : Ŭ���� ����(����)�� ����Ű�� ����(����)�� ��ȯ
    return float4();
}

float4 SJH_Ray::RayAtViewSpace(float4 _MouseClickPos)
{
    // 2D ȯ�濡���� ���콺 Ŭ�������� �̿��Ͽ�
    // 3D ȯ�濡���� ���콺 ��ǥ�� ���� ī�޶� �ٶ󺸴� ������ ����Ѵ�.
    float MousePosX = _MouseClickPos.x;
    float MousePosY = _MouseClickPos.y;

    return RayAtViewSpace(MousePosX, MousePosY);
}

bool SJH_Ray::IsPicked(GameEngineCollision* _Collider)
{





    // ���� : ����󿡼� �ش� Ray(����)�� �浹�ϴ� �浹ü�� �ִ��� �˻�
    return false;
}

void SJH_Ray::Start()
{
}

void SJH_Ray::Update(float _DeltaTime)
{
    // �ش� ������ ���緹���� ��� ���Ϳ� �浹üũ
    

}

SJH_Ray::SJH_Ray()
    : OriginPos_(float4::ZERO)
    , Direction_(float4::ZERO)
{
}

SJH_Ray::~SJH_Ray()
{
}
