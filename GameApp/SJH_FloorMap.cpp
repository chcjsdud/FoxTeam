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

	//FloorMap_->GetTransform()->SetLocalRotationDegree({ 0.0f, 70.0f, 0.0f });

#pragma region �׺���̼� �׽�Ʈ
//====================================== Navigation Cell
	// �ش� �׺���̼� �޽��� ������ Get
	std::vector<FbxExMeshInfo>& AllMeshInfo = FloorMap_->GetMesh()->GetMeshInfos();		// Mesh�� ���� ���� �� ���ؽ������� ����
	std::vector<FbxMeshSet>& AllMeshMap = FloorMap_->GetMesh()->GetAllMeshMap();		// Mesh�� ��� ���� ����

	// �ش� Navigation FBX File�� ������ ��� �޽��� Ž��
	int MeshCount = static_cast<int>(AllMeshInfo.size());
	for (int MeshNumber = 0; MeshNumber < MeshCount; ++MeshNumber)
	{
		// �ش� Mesh�� �����ϴ� ������ ������ŭ �ݺ��� ����
		int FaceCount = AllMeshInfo[MeshNumber].FaceNum;
		for (int FaceNumber = 0; FaceNumber < FaceCount; ++FaceNumber)
		{
			// �ش� ���� �����ϴ� ���� �� �⺻������ SJH_NaviCell ������ ����
			// �޽� -> �ﰢ��(��)���� ���� -> ��������������
			std::vector<float4> VertexList;
			for (int VertexNumber = 0; VertexNumber < 3; ++VertexNumber)
			{
				int VertexIndex = 0;
				float4 Vertex = float4::ZERO;
				VertexList.push_back(Vertex);
			}

			// Map���� �ش� ���� �����ϴ� ��� ��(�ﰢ��) ������ ����
			SJH_NaviCell* NewCellInfo = new SJH_NaviCell();
			NewCellInfo->CreateNavigationCellInfo(MeshNumber, FaceNumber, VertexList);
			NavigationCellInfos_.push_back(NewCellInfo);
		}
	}

	// ������ �޽��� ������ ��� ���� Ž���Ͽ� ������ ���� ������ ���� ã�Ƴ��� ��������
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
