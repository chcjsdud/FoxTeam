#include "PreCompile.h"
#include "PJW_Map.h"
#include "PJW_Enum.h"
#include <GameEngine/GameEngineFBXRenderer.h>
#include "PJW_Hyunwoo.h"
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineImageRenderer.h>

PJW_Map::PJW_Map() // default constructer 디폴트 생성자
{

}

PJW_Map::~PJW_Map() // default destructer 디폴트 소멸자
{

}

PJW_Map::PJW_Map(PJW_Map&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{
		
}


void PJW_Map::Start()
{
	GameEngineDirectory Directory;
	Directory.MoveParent("FoxTeam");
	Directory.MoveChild("Resources");
	Directory.MoveChild("FBX");
	Directory.MoveChild("YSJ");

	//=============================================== NaviMesh Load & Create NaviCell Info
	std::string NaviMeshName = "Bg_NaviMesh.fbx";

	// Mesh Load
	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(Directory.PathToPlusFileName(NaviMeshName)))
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(Directory.PathToPlusFileName(NaviMeshName));
		Mesh->CreateRenderingBuffer();
	}

	// Create Renderer
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	renderer_->SetFBXMesh(NaviMeshName, "TextureDeferredLight");

	for (UINT i = 0; i < renderer_->GetRenderSetCount(); i++)
	{
		renderer_->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Red.png");
		renderer_->GetRenderSet(i).PipeLine_->SetRasterizer("EngineBaseRasterizerNone");
	}

	// 네비게이션 셀정보 생성
//	CreateAllNaviCellInfo();

	// NavigationCellInfos_의 랜덤한 삼각형을 선택하여 플레이어 위치좌표(랜덤한 삼각형의 무게중심) 셋팅
	//	GameEngineRandom Random;
	//	int RandomFace = Random.RandomInt(0, static_cast<int>(NavigationCellInfos_.size()) - 1);
	//	mainPlayer_->Initialize(NavigationCellInfos_[RandomFace], NavigationCellInfos_[RandomFace]->GetCenterToGravity());
	//
	////=============================================== NaviColMesh Load
	//std::string NaviColMeshName = "NaviCol.fbx";
	//
	//if (nullptr == GameEngineFBXMeshManager::GetInst().Find(Directory.PathToPlusFileName(NaviColMeshName)))
	//{
	//	GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(Directory.PathToPlusFileName(NaviColMeshName));
	//	Mesh->CreateRenderingBuffer();
	//}
	//
	//// Create Renderer
	//NaviColMeshRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	//NaviColMeshRenderer_->SetFBXMesh(NaviColMeshName, "TextureDeferredLight");
	//
	//for (UINT i = 0; i < NaviColMeshRenderer_->GetRenderSetCount(); i++)
	//{
	//	NaviColMeshRenderer_->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Green.png");
	//}
}