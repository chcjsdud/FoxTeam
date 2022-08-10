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
	// FBX Files ��� ����
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

#pragma region �׺���̼� �׽�Ʈ
//====================================== Navigation Cell
	// �ش� �׺���̼� �޽��� ������ Get
	std::vector<FbxExMeshInfo>& AllMeshInfo = FloorMap_->GetMesh()->GetMeshInfos();		// Mesh�� ���� ���� �� ���ؽ������� ����
	std::vector<FbxMeshSet>& AllMeshMap = FloorMap_->GetMesh()->GetAllMeshMap();		// Mesh�� ��� ���� ����

	// �ش� Navigation FBX File�� ������ ��� �޽��� Ž��
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

	// ��� ���� �˻��Ͽ� ������ ���� ã�Ƴ��� ������ ����
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
			// ��, ���� ���ظ�� ����� ���ظ��� ������ ���̶�� ����
			if (CurCellNumber == CompareCellNumber)
			{
				continue;
			}

			// ���ظ��� ������ ���� ��� ã�Ƴ���.
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
