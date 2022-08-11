#include "PreCompile.h"
#include "SJH_FloorMap.h"

#include <GameEngineBase/GameEngineRandom.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

#include "SJH_Yuki.h"
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

SJH_NaviCell* SJH_FloorMap::SearchCurrentPosToNaviCell(const float4& _Position)
{
	// ��ġ��ǥ�� ���Ź޾� �ش� ��ǥ�� ���Ե� �ﰢ��(��)�� ã�Ƴ��� ��ȯ
	for (int CellNum = 0; CellNum < static_cast<int>(NavigationCellInfos_.size()); ++CellNum)
	{
		if (true == NavigationCellInfos_[CellNum]->CheckPointisIncludedIntheTriangle(_Position))
		{
			return NavigationCellInfos_[CellNum];
		}
	}

	return nullptr;
}

bool SJH_FloorMap::MoveFacePath(const SJH_NaviCell* _StartCell, const SJH_NaviCell* _TargetCell, std::vector<SJH_NaviCell*>& _MovePath)
{
	// ���� �ﰢ�� ~ ��ǥ �ﰢ�������� �̵� �ﰢ������� ��ȯ
	// ��, �̵��Ұ��Ǵܽ� false�� ��ȯ�ϸ� �̵���δ� ���������ʴ´�.







	return false;
}

void SJH_FloorMap::Start()
{
	// FBX Files ��� ����
	GameEngineDirectory Directory;
	Directory.MoveParent("FoxTeam");
	Directory.MoveChild("Resources");
	Directory.MoveChild("FBX");
	Directory.MoveChild("YSJ");

	std::string MeshName = "Bg_NaviMesh.fbx";

	// Mesh Load
	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(Directory.PathToPlusFileName(MeshName)))
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(Directory.PathToPlusFileName(MeshName));
		Mesh->CreateRenderingBuffer();
	}

	// Create Renderer
	FloorMap_ = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	FloorMap_->SetFBXMesh(MeshName, "TextureDeferredLight");

	//for (UINT i = 0; i < FloorMap_->GetRenderSetCount(); i++)
	//{
	//	FloorMap_->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Green.png");
	//}

	// �׺���̼� ������ ����
	CreateAllNaviCellInfo();

	// NavigationCellInfos_�� ������ �ﰢ���� �����Ͽ� �÷��̾� ��ġ��ǥ(������ �ﰢ���� �����߽�) ����
	if (nullptr != SJH_Yuki::MainPlayer)
	{
		GameEngineRandom Random;
		int RandomFace = Random.RandomInt(0, NavigationCellInfos_.size() - 1);
		SJH_Yuki::MainPlayer->Initialize(NavigationCellInfos_[RandomFace], NavigationCellInfos_[RandomFace]->GetCenterToGravity());
	}
}

void SJH_FloorMap::CreateAllNaviCellInfo()
{
	// �ش� �׺���̼� �޽��� ������ Get
	std::vector<FbxExMeshInfo>& AllMeshInfo = FloorMap_->GetMesh()->GetMeshInfos();
	std::vector<FbxMeshSet>& AllMeshMap = FloorMap_->GetMesh()->GetAllMeshMap();

	// �ش� Navigation FBX File�� ������ ��� �޽��� Ž��
	for (int MeshNumber = 0; MeshNumber < static_cast<int>(AllMeshInfo.size()); ++MeshNumber)
	{
		for (int MaterialNumber = 0; MaterialNumber < AllMeshInfo[MeshNumber].MaterialNum; ++MaterialNumber)
		{
			// 3���� ������ �����ϴ� �ε�������� ���� -> ��(�ﰢ��) 1���� �̷�
			for (int FaceNumber = 0; FaceNumber < static_cast<int>(AllMeshMap[MeshNumber].Indexs[0][MaterialNumber].size()) / 3; ++FaceNumber)
			{
				std::vector<GameEngineVertex> VertexList;
				std::vector<UINT> IndexList;
				for (int VertexNumber = 0; VertexNumber < 3; ++VertexNumber)
				{
					UINT VertexIndex = AllMeshMap[MeshNumber].Indexs[0][MaterialNumber][FaceNumber * 3 + VertexNumber];
					GameEngineVertex Vertex = AllMeshMap[MeshNumber].Vertexs[VertexIndex];
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
			NavigationCellInfos_[CurCellNumber]->SearchAdjacentTriangles(NavigationCellInfos_[CompareCellNumber], false);
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
