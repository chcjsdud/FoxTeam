#include "PreCompile.h"
#include "YSJ_Ray.h"

#include <GameEngineBase/GameEngineMath.h>

#include <GameEngine/GameEngineDevice.h>
#include <GameEngine/GameEngineCore.h>
#include <GameEngine/CameraComponent.h>
#include <GameEngine/GameEngineFBXMesh.h>
#include <GameEngine/GameEngineCollision.h>

bool YSJ_Ray::IsPicked(const float4& _MousePos, float4& _PickedPos, GameEngineFBXRenderer* _Mesh)
{
    if (nullptr == _Mesh)
    {
        return IsColliderPicked(_MousePos, _PickedPos);
    }
    else
    {
        return IsMeshPicked(_Mesh, _MousePos, _PickedPos);
    }

    return false;
}

bool YSJ_Ray::IsColliderPicked(const float4& _MousePos, float4& _PickedPos)
{
    // ������ ���念������ �̵�
    // ����� : ����������ġ, �����ǹ���
    if (false == RayAtViewSpace(_MousePos))
    {
        return false;
    }

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
            // ������ ���������������� ���õ� �浹ü�� �����ϴ� ���������� �Ÿ�
            float Dist = 0.0f;
            if (true == (*StartIter)->BoundingToRayCollision((*StartIter)->GetCollisionType(), OriginPos_, Direction_, Dist))
            {
                // ������ ������ ��ǥ�� ��ȯ
                _PickedPos = OriginPos_ + (Direction_ * Dist);
                return true;
            }
        }
    }

    // �浹�ϴ� �浹ü�� ���������ʴٸ� ����
    return false;
}

bool YSJ_Ray::IsMeshPicked(GameEngineFBXRenderer* _Mesh, const float4& _MousePos, float4& _PickedPos)
{
    // ������ ���念������ �̵�
    // ����� : ����������ġ, �����ǹ���
    if (false == RayAtViewSpace(_MousePos))
    {
        return false;
    }

    // Mesh�� ���ٸ� ó�� �Ұ�
    if (nullptr == _Mesh)
    {
        return false;
    }
    
    float Dist = 0.0f;

    // ���������� ������������ �Ÿ��� �̿��Ͽ� �ش� ��ǥ�� ��ȯ
    if (true == _Mesh->CheckIntersects(OriginPos_, Direction_, Dist))
    {
        _PickedPos = OriginPos_ + (Direction_ * Dist);
        return true;
    }

    return false;
}

bool YSJ_Ray::RayAtViewSpace(float _MousePosX, float _MousePosY)
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

    return true;
}

bool YSJ_Ray::RayAtViewSpace(float4 _MouseClickPos)
{
    // 2D ȯ�濡���� ���콺 Ŭ�������� �̿��Ͽ�
    // 3D ȯ�濡���� ���콺 ��ǥ�� ���� ī�޶� �ٶ󺸴� ������ ����Ѵ�.
    float MousePosX = _MouseClickPos.x;
    float MousePosY = _MouseClickPos.y;

    return RayAtViewSpace(MousePosX, MousePosY);
}

void YSJ_Ray::Start()
{
}

void YSJ_Ray::Update(float _DeltaTime)
{
}

YSJ_Ray::YSJ_Ray()
    : OriginPos_(float4::ZERO)
    , Direction_(float4::ZERO)
{
}

YSJ_Ray::~YSJ_Ray()
{
}