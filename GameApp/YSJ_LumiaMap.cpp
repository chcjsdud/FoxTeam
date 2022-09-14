#include "PreCompile.h"
#include "YSJ_LumiaMap.h"
#include <GameEngine/GameEngineFBXRenderer.h>
#include "YSJ_Player.h"
#include "YSJ_Mouse.h"
#include "NavMesh.h"
#include "ItemBoxManager.h"

YSJ_LumiaMap* YSJ_LumiaMap::MainMap = nullptr;

YSJ_LumiaMap::YSJ_LumiaMap()
	: FBXNaviRenderer(nullptr)
	, FBXWallRenderer(nullptr)
	, FBXSchoolMap(nullptr)
{
	MainMap = this;
}

YSJ_LumiaMap::~YSJ_LumiaMap()
{

}

void YSJ_LumiaMap::Start()
{
	FBXNaviRenderer = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	FBXNaviRenderer->SetFBXMesh("Bg_NaviMesh.fbx", "TextureDeferredLight");

	for (UINT i = 0; i < FBXNaviRenderer->GetRenderSetCount(); i++)
	{
		FBXNaviRenderer->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Red.png");
		FBXNaviRenderer->GetRenderSet(i).PipeLine_->SetRasterizer("EngineBaseRasterizerWireFrame");
	}

	/*FBXWallRenderer = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	FBXWallRenderer->SetFBXMesh("NaviCol.fbx", "TextureDeferredLight");

	for (UINT i = 0; i < FBXWallRenderer->GetRenderSetCount(); i++)
	{
		FBXWallRenderer->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Green.png");
	}

	FBXWallRenderer->GetTransform()->SetLocalScaling({ 3.0f, 3.0f, 3.0f });*/

	GameEngineDirectory tempDir;

	tempDir.MoveParent("FoxTeam");
	tempDir.MoveChild("Resources");
	tempDir.MoveChild("FBX");
	tempDir.MoveChild("UserMesh");
	tempDir.MoveChild("Map");

	std::vector<GameEngineFile> vecFile = tempDir.GetAllFile(".UserMesh");

	//for (size_t i = 0; i < vecFile.size(); i++)
	//{
	//	FBXSchoolMap = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	//	FBXSchoolMap->SetFBXMesh(vecFile[i].GetFileName(), "TextureDeferredLight");
	//}

	tempDir.MoveParent("UserMesh");
	tempDir.MoveChild("ItemBox");

	vecFile = tempDir.GetAllFile(".UserMesh");

	ItemBoxManager ItemBox;

	for (size_t i = 0; i < vecFile.size(); i++)
	{
		FBXSchoolMap = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
		FBXSchoolMap->SetFBXMesh(vecFile[i].GetFileName(), "TextureDeferredLight");
	}
}

void YSJ_LumiaMap::Update(float _DeltaTime)
{
	if (nullptr != YSJ_Player::MainPlayer)
	{
		float dist = 0.0f;

		if (true == FBXNaviRenderer->CheckIntersects(
			YSJ_Player::MainPlayer->GetTransform()->GetWorldPosition() + float4{0.0f, 100.0f, 0.0f}, float4::DOWN, dist))
		{
			YSJ_Player::MainPlayer->SetColor(float4::GREEN);
		}
		else
		{
			YSJ_Player::MainPlayer->SetColor(float4::RED);
		}
	}
}

void YSJ_LumiaMap::ReleaseEvent()
{
}

void YSJ_LumiaMap::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
}

void YSJ_LumiaMap::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
}
