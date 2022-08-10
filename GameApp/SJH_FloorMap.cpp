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

SJH_NaviCell* SJH_FloorMap::GetNaviCellInfo(const float4& _Vertex0, const float4& _Vertex1, const float4& _Vertex2)
{
	int CellInfoSize = static_cast<int>(NavigationCellInfos_.size());
	for (int CellNumber = 0; CellNumber < CellInfoSize; ++CellNumber)
	{
		if(nullptr != NavigationCellInfos_[CellNumber]->CompareVertices(_Vertex0, _Vertex1, _Vertex2))
		{
			return NavigationCellInfos_[CellNumber];
		}
	}

	return nullptr;
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

#pragma region 네비게이션 테스트
//====================================== Navigation Cell
	// 해당 네비게이션 메쉬의 정보를 Get
	std::vector<FbxExMeshInfo>& AllMeshInfo = FloorMap_->GetMesh()->GetMeshInfos();		// Mesh당 면의 갯수 및 버텍스갯수등 정보
	std::vector<FbxMeshSet>& AllMeshMap = FloorMap_->GetMesh()->GetAllMeshMap();		// Mesh의 모든 정보 집합

	// 해당 Navigation FBX File이 가지는 모든 메쉬를 탐색
	for (int MeshNumber = 0; MeshNumber < static_cast<int>(AllMeshInfo.size()); ++MeshNumber)
	{
		for (int MaterialNumber = 0; MaterialNumber < AllMeshInfo[MeshNumber].MaterialNum; ++MaterialNumber)
		{
			for (int FaceNumber = 0; FaceNumber < AllMeshInfo[MeshNumber].FaceNum; ++FaceNumber)
			{
				std::vector<float4> VertexList;
				std::vector<UINT> IndexList;
				for (int VertexNumber = 0; VertexNumber < 3; ++VertexNumber)
				{
					UINT VertexIndex = AllMeshMap[MeshNumber].Indexs[0][MaterialNumber][FaceNumber * 3 + VertexNumber];
					float4 Vertex = AllMeshMap[MeshNumber].Vertexs[VertexIndex].POSITION;
					VertexList.push_back(Vertex);
					IndexList.push_back(VertexIndex);
				}

				SJH_NaviCell* NewCellInfo = new SJH_NaviCell();
				NewCellInfo->CreateNavigationCellInfo(MeshNumber, FaceNumber, VertexList, IndexList);
				NavigationCellInfos_.push_back(NewCellInfo);
			}
		}
	}

	// 모든 면을 검사하여 인접한 면을 찾아내서 정보에 저장
	FindAdjacentFaces();

	int a = 0;

//====================================== Navigation Cell
#pragma endregion
}

void SJH_FloorMap::FindAdjacentFaces()
{
	int CellInfoSize = static_cast<int>(NavigationCellInfos_.size());
	for (int CurCellNumber = 0; CurCellNumber < CellInfoSize; ++CurCellNumber)
	{
		for (int CompareCellNumber = 0; CompareCellNumber < CellInfoSize; ++CompareCellNumber)
		{
			// 단, 현재 기준면과 상대의 기준면이 동일한 면이라면 무시
			if (CurCellNumber == CompareCellNumber)
			{
				continue;
			}

			// 기준면의 인접한 면을 모두 찾아낸다.
			NavigationCellInfos_[CurCellNumber]->SearchAdjacentTriangles(NavigationCellInfos_[CompareCellNumber]);
		}
	}
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
	for (int CellInfoNum = 0; CellInfoNum < static_cast<int>(NavigationCellInfos_.size()); ++CellInfoNum)
	{
		delete NavigationCellInfos_[CellInfoNum];
		NavigationCellInfos_[CellInfoNum] = nullptr;
	}
	NavigationCellInfos_.clear();
}
