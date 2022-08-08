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








	// 정보 추가
	SJH_NaviCell* NewCellInfo = new SJH_NaviCell();
	NavigationCellInfos_.push_back(NewCellInfo);







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
