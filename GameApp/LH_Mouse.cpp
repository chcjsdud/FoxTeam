#include "PreCompile.h"

#include "LH_Mouse.h"

#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngine/GameEngineDevice.h>
#include <GameEngine/GameEngineCore.h>
#include <GameEngine/CameraComponent.h>
#include <GameEngine/GameEngineFBXMesh.h>

#include "SJH_Yuki.h"
#include "SJH_FloorMap.h"

void LH_Mouse::Start()
{
	Renderer_ = CreateTransformComponent<GameEngineUIRenderer>();
	Renderer_->SetImage(GameEngineString::toupper("Cursor_01.png"));
	Renderer_->GetTransform()->SetLocalPosition({ 12.0f, -10.0f, 0.0f });
	Renderer_->GetTransform()->SetLocalScaling({ 30.0f, 30.0f, 30.0f });

	Collider_ = CreateTransformComponent<GameEngineCollision>();
	Collider_->GetTransform()->SetLocalScaling(float4{ 30.0f, 30.0f, 30.0f });
	Collider_->SetCollisionInfo(static_cast<int>(CollisionGroup::MousePointer), CollisionType::AABBBox3D);

	// ���� ����
	Ray_ = GetLevel()->CreateActor<LH_Ray>();
}

void LH_Mouse::Update(float _DeltaTime)
{
	// ���콺 ��ġ����
	GetTransform()->SetWorldPosition(GameEngineInput::GetInst().GetMouse3DPos());
}

LH_Mouse::LH_Mouse()
	: Renderer_(nullptr)
	, Collider_(nullptr)
	, Ray_(nullptr)
{
}

LH_Mouse::~LH_Mouse()
{
}

bool LH_Mouse::Mouse_GetCollision(CollisionGroup _CollisionGroup)
{
    if (true == GameEngineWindow::GetInst().IsWindowRangeOut(GameEngineInput::GetInst().GetMousePos()))
    {
        return false;
    }

    return Ray_->IsPicked(GameEngineInput::GetInst().GetMousePos(), MousePos_, _CollisionGroup);
}

bool LH_Mouse::Mouse_GetCollisionMesh(GameEngineFBXMesh* _Mesh)
{
    if (true == GameEngineWindow::GetInst().IsWindowRangeOut(GameEngineInput::GetInst().GetMousePos()))
    {
        return false;
    }

    return Ray_->IsPickedMesh(GameEngineInput::GetInst().GetMousePos(), MousePos_, _Mesh);
}

GameEngineActor* LH_Mouse::Mouse_GetCollisionActor(CollisionGroup _CollisionGroup)
{
    return Ray_->IsPickedActor(GameEngineInput::GetInst().GetMousePos(), _CollisionGroup);;
}






bool LH_Mouse::LH_Ray::IsPicked(const float4& _MousePos, float4& _PickedPos, CollisionGroup _CollisionGroup)
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
    std::list<GameEngineCollision*>::iterator StartIter = AllList[static_cast<int>(_CollisionGroup)].begin();
    std::list<GameEngineCollision*>::iterator EndIter = AllList[static_cast<int>(_CollisionGroup)].end();
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

    // �浹�ϴ� �浹ü�� ���������ʴٸ� ����
    return false;
}

bool LH_Mouse::LH_Ray::IsPickedMesh(const float4 & _MousePos, float4 & _PickedPos, GameEngineFBXMesh * _Mesh)
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

    // ���������˻縦 ���� Mesh ���� Get
    std::vector<FbxMeshSet>& vecAllMeshSet = _Mesh->GetAllMeshMap();
    std::vector<FbxExMeshInfo>& vecMeshInfo = _Mesh->GetMeshInfos();

    // ����üũ
    float Dist = 0.0f;
    for (int MeshCnt = 0; MeshCnt < static_cast<int>(vecAllMeshSet.size()); ++MeshCnt)
    {
        for (int MaterialCnt = 0; MaterialCnt < static_cast<int>(vecAllMeshSet[MeshCnt].MatialData.size()); ++MaterialCnt)
        {
            for (int FaceNumCnt = 0; FaceNumCnt < vecMeshInfo[MeshCnt].FaceNum; ++FaceNumCnt)
            {
                int Vertex0_Index = vecAllMeshSet[MeshCnt].Indexs[0][MaterialCnt][FaceNumCnt + 0];
                int Vertex1_Index = vecAllMeshSet[MeshCnt].Indexs[0][MaterialCnt][FaceNumCnt + 1];
                int Vertex2_Index = vecAllMeshSet[MeshCnt].Indexs[0][MaterialCnt][FaceNumCnt + 2];

                float4 Vertex0 = vecAllMeshSet[MeshCnt].Vertexs[Vertex0_Index].POSITION * GetTransform()->GetTransformData().WorldWorld_;
                float4 Vertex1 = vecAllMeshSet[MeshCnt].Vertexs[Vertex1_Index].POSITION * GetTransform()->GetTransformData().WorldWorld_;
                float4 Vertex2 = vecAllMeshSet[MeshCnt].Vertexs[Vertex2_Index].POSITION * GetTransform()->GetTransformData().WorldWorld_;

                // ���������� ������������ �Ÿ��� �̿��Ͽ� �ش� ��ǥ�� ��ȯ
                if (true == DirectX::TriangleTests::Intersects(OriginPos_.DirectVector, Direction_.DirectVector, Vertex0.DirectVector, Vertex1.DirectVector, Vertex2.DirectVector, Dist))
                {
                    // ������ ������ ��ǥ�� ��ȯ
                    _PickedPos = OriginPos_ + (Direction_ * Dist);
                    return true;
                }
            }
        }
    }

    return false;
}

GameEngineActor* LH_Mouse::LH_Ray::IsPickedActor(const float4& _MousePos, CollisionGroup _CollisionGroup)
{
    if (false == RayAtViewSpace(_MousePos))
    {
        return nullptr;
    }

    std::list<GameEngineCollision*> GruopList = GetLevel()-> GetCollisionGroup(static_cast<int>(_CollisionGroup));

    // �ش� ������ �����ϴ� �浹ü Ž��
    std::list<GameEngineCollision*>::iterator StartIter = GruopList.begin();
    std::list<GameEngineCollision*>::iterator EndIter = GruopList.end();
    for (; StartIter != EndIter; ++StartIter)
    {
        float Dist = 0.0f;
        if (true == (*StartIter)->BoundingToRayCollision((*StartIter)->GetCollisionType(), OriginPos_, Direction_, Dist))
        {
            return (*StartIter)->GetActor();
        }
    }

    return nullptr;
}

//GameEngineActor* LH_Mouse::LH_Ray::IsPickedMeshActor(const float4& _MousePos, GameEngineFBXMesh* _Mesh)
//{
//    // ������ ���念������ �̵�
//   // ����� : ����������ġ, �����ǹ���
//    if (false == RayAtViewSpace(_MousePos))
//    {
//        return nullptr;
//    }
//
//    // Mesh�� ���ٸ� ó�� �Ұ�
//    if (nullptr == _Mesh)
//    {
//        return nullptr;
//    }
//
//    // ���������˻縦 ���� Mesh ���� Get
//    std::vector<FbxMeshSet>& vecAllMeshSet = _Mesh->GetAllMeshMap();
//    std::vector<FbxExMeshInfo>& vecMeshInfo = _Mesh->GetMeshInfos();
//
//    // ����üũ
//    float Dist = 0.0f;
//    for (int MeshCnt = 0; MeshCnt < static_cast<int>(vecAllMeshSet.size()); ++MeshCnt)
//    {
//        for (int MaterialCnt = 0; MaterialCnt < static_cast<int>(vecAllMeshSet[MeshCnt].MatialData.size()); ++MaterialCnt)
//        {
//            for (int FaceNumCnt = 0; FaceNumCnt < vecMeshInfo[MeshCnt].FaceNum; ++FaceNumCnt)
//            {
//                int Vertex0_Index = vecAllMeshSet[MeshCnt].Indexs[0][MaterialCnt][FaceNumCnt + 0];
//                int Vertex1_Index = vecAllMeshSet[MeshCnt].Indexs[0][MaterialCnt][FaceNumCnt + 1];
//                int Vertex2_Index = vecAllMeshSet[MeshCnt].Indexs[0][MaterialCnt][FaceNumCnt + 2];
//
//                float4 Vertex0 = vecAllMeshSet[MeshCnt].Vertexs[Vertex0_Index].POSITION * GetTransform()->GetTransformData().WorldWorld_;
//                float4 Vertex1 = vecAllMeshSet[MeshCnt].Vertexs[Vertex1_Index].POSITION * GetTransform()->GetTransformData().WorldWorld_;
//                float4 Vertex2 = vecAllMeshSet[MeshCnt].Vertexs[Vertex2_Index].POSITION * GetTransform()->GetTransformData().WorldWorld_;
//
//                // ���������� ������������ �Ÿ��� �̿��Ͽ� �ش� ��ǥ�� ��ȯ
//                if (true == DirectX::TriangleTests::Intersects(OriginPos_.DirectVector, Direction_.DirectVector, Vertex0.DirectVector, Vertex1.DirectVector, Vertex2.DirectVector, Dist))
//                {
//                    return _Mesh->;
//                }
//            }
//        }
//    }
//
//    return false;
//}

bool LH_Mouse::LH_Ray::RayAtViewSpace(float _MousePosX, float _MousePosY)
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

bool LH_Mouse::LH_Ray::RayAtViewSpace(float4 _MouseClickPos)
{
    // 2D ȯ�濡���� ���콺 Ŭ�������� �̿��Ͽ�
    // 3D ȯ�濡���� ���콺 ��ǥ�� ���� ī�޶� �ٶ󺸴� ������ ����Ѵ�.
    float MousePosX = _MouseClickPos.x;
    float MousePosY = _MouseClickPos.y;

    return RayAtViewSpace(MousePosX, MousePosY);
}

void LH_Mouse::LH_Ray::Start()
{
}

void LH_Mouse::LH_Ray::Update(float _DeltaTime)
{
}

LH_Mouse::LH_Ray::LH_Ray()
    : OriginPos_(float4::ZERO)
    , Direction_(float4::ZERO)
{
}

LH_Mouse::LH_Ray::~LH_Ray()
{
}
