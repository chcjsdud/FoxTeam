#include "PreCompile.h"
#include "SJH_FloorMap.h"

#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

#include "SJH_NaviCell.h"

SJH_FloorMap* SJH_FloorMap::FloorMap = nullptr;

GameEngineFBXMesh* SJH_FloorMap::GetFloorMapMesh()
{
	if (nullptr == FloorMap_)
	{
		return nullptr;
	}

	return FloorMap_->GetMesh();
}

GameEngineFBXRenderer* SJH_FloorMap::GetFloorMapRenderer()
{
	return FloorMap_;
}

void SJH_FloorMap::Start()
{
	// FBX Files 경로 지정
	GameEngineDirectory Directory;
	Directory.MoveParent("FoxTeam");
	Directory.MoveChild("Resources");
	Directory.MoveChild("FBX");
	Directory.MoveChild("YSJ");

	std::string MeshName = "NaviCol.fbx";

	// Mesh Load
	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(Directory.PathToPlusFileName(MeshName)))
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(Directory.PathToPlusFileName(MeshName));
		Mesh->CreateRenderingBuffer();
	}

	// Create Renderer
	FloorMap_ = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	FloorMap_->SetFBXMesh(MeshName, "TextureDeferredLight");

	for (UINT i = 0; i < FloorMap_->GetRenderSetCount(); i++)
	{
		FloorMap_->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Green.png");
	}

	//FloorMap_->GetTransform()->SetLocalRotationDegree({ 0.0f, 70.0f, 0.0f });

#pragma region 네비게이션 테스트
//====================================== Navigation Cell
	// 해당 네비게이션 메쉬의 정보를 Get
	std::vector<FbxExMeshInfo>& AllMeshInfo = FloorMap_->GetMesh()->GetMeshInfos();		// Mesh당 면의 갯수 및 버텍스갯수등 정보
	std::vector<FbxMeshSet>& AllMeshMap = FloorMap_->GetMesh()->GetAllMeshMap();		// Mesh의 모든 정보 집합

	// 해당 Navigation FBX File이 가지는 모든 메쉬를 탐색
	int MeshCount = static_cast<int>(AllMeshInfo.size());
	for (int MeshNumber = 0; MeshNumber < MeshCount; ++MeshNumber)
	{
		// 해당 Mesh를 구성하는 모든면의 갯수만큼 반복문 실행
		int FaceCount = AllMeshInfo[MeshNumber].FaceNum;
		for (int FaceNumber = 0; FaceNumber < FaceCount; ++FaceNumber)
		{
			// 해당 면을 구성하는 정점 및 기본정보를 SJH_NaviCell 정보로 셋팅
			// 메쉬 -> 삼각형(면)들의 집합 -> 세정점들의집합
			std::vector<float4> VertexList;
			for (int VertexNumber = 0; VertexNumber < 3; ++VertexNumber)
			{
				int VertexIndex = 0;
				float4 Vertex = float4::ZERO;
				VertexList.push_back(Vertex);
			}

			// Map에서 해당 맵을 구성하는 모든 셀(삼각형) 정보를 관리
			SJH_NaviCell* NewCellInfo = new SJH_NaviCell();
			NewCellInfo->CreateNavigationCellInfo(MeshNumber, FaceNumber, VertexList);
			NavigationCellInfos_.push_back(NewCellInfo);
		}
	}

	// 각각의 메쉬가 보유한 모든 셀을 탐색하여 지정한 셀과 인접한 셀을 찾아내서 정보저장
	for (int MeshNumber = 0; MeshNumber < MeshCount; ++MeshNumber)
	{

	}

	// GetFaceInfomationFlag()

//====================================== Navigation Cell
#pragma endregion
}

void SJH_FloorMap::Update(float _DeltaTime)
{
}

SJH_FloorMap::SJH_FloorMap()
	: FloorMap_(nullptr)
{
}

SJH_FloorMap::~SJH_FloorMap()
{
	int NavigationCellInfoSize = static_cast<int>(NavigationCellInfos_.size());
	if (0 != NavigationCellInfoSize)
	{
		for (int i = 0; i < NavigationCellInfoSize; ++i)
		{
			delete NavigationCellInfos_[i];
			NavigationCellInfos_[i] = nullptr;
		}
	}
	NavigationCellInfos_.clear();
}
