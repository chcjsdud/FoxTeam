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

	// 광선 생성
	Ray_ = GetLevel()->CreateActor<LH_Ray>();
}

void LH_Mouse::Update(float _DeltaTime)
{
	// 마우스 위치갱신
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
    // 광선을 월드영역으로 이동
   // 결과값 : 광선시작위치, 광선의방향
    if (false == RayAtViewSpace(_MousePos))
    {
        return false;
    }

    // 현재 레벨의 모든 충돌체 목록 Get
    std::map<int, std::list<GameEngineCollision*>> AllList = GetLevel()->GetAllCollision();

    // 해당 광선과 교차하는 충돌체 탐색
    std::list<GameEngineCollision*>::iterator StartIter = AllList[static_cast<int>(_CollisionGroup)].begin();
    std::list<GameEngineCollision*>::iterator EndIter = AllList[static_cast<int>(_CollisionGroup)].end();
    for (; StartIter != EndIter; ++StartIter)
    {
        // 광선의 시작지점에서부터 선택된 충돌체와 교차하는 지점까지의 거리
        float Dist = 0.0f;
        if (true == (*StartIter)->BoundingToRayCollision((*StartIter)->GetCollisionType(), OriginPos_, Direction_, Dist))
        {
            // 교차한 지점의 좌표를 반환
            _PickedPos = OriginPos_ + (Direction_ * Dist);
            return true;
        }
    }

    // 충돌하는 충돌체가 존재하지않다면 실패
    return false;
}

bool LH_Mouse::LH_Ray::IsPickedMesh(const float4 & _MousePos, float4 & _PickedPos, GameEngineFBXMesh * _Mesh)
{
    // 광선을 월드영역으로 이동
   // 결과값 : 광선시작위치, 광선의방향
    if (false == RayAtViewSpace(_MousePos))
    {
        return false;
    }

    // Mesh가 없다면 처리 불가
    if (nullptr == _Mesh)
    {
        return false;
    }

    // 광선교차검사를 위한 Mesh 정보 Get
    std::vector<FbxMeshSet>& vecAllMeshSet = _Mesh->GetAllMeshMap();
    std::vector<FbxExMeshInfo>& vecMeshInfo = _Mesh->GetMeshInfos();

    // 교차체크
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

                // 교차성공시 교차점까지의 거리를 이용하여 해당 좌표를 반환
                if (true == DirectX::TriangleTests::Intersects(OriginPos_.DirectVector, Direction_.DirectVector, Vertex0.DirectVector, Vertex1.DirectVector, Vertex2.DirectVector, Dist))
                {
                    // 교차한 지점의 좌표를 반환
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

    // 해당 광선과 교차하는 충돌체 탐색
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
//    // 광선을 월드영역으로 이동
//   // 결과값 : 광선시작위치, 광선의방향
//    if (false == RayAtViewSpace(_MousePos))
//    {
//        return nullptr;
//    }
//
//    // Mesh가 없다면 처리 불가
//    if (nullptr == _Mesh)
//    {
//        return nullptr;
//    }
//
//    // 광선교차검사를 위한 Mesh 정보 Get
//    std::vector<FbxMeshSet>& vecAllMeshSet = _Mesh->GetAllMeshMap();
//    std::vector<FbxExMeshInfo>& vecMeshInfo = _Mesh->GetMeshInfos();
//
//    // 교차체크
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
//                // 교차성공시 교차점까지의 거리를 이용하여 해당 좌표를 반환
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

    return true;
}

bool LH_Mouse::LH_Ray::RayAtViewSpace(float4 _MouseClickPos)
{
    // 2D 환경에서의 마우스 클릭지점을 이용하여
    // 3D 환경에서의 마우스 좌표와 현재 카메라가 바라보는 방향을 계산한다.
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
