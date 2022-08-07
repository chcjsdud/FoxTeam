#include "PreCompile.h"
#include "YSJ_LumiaMap.h"
#include <GameEngine/GameEngineFBXRenderer.h>
#include "YSJ_Player.h"

YSJ_LumiaMap::YSJ_LumiaMap()
	: FBXNaviRenderer(nullptr)
	, FBXWallRenderer(nullptr)
	, FBXSchoolMap(nullptr)
{

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
		FBXNaviRenderer->GetRenderSet(i).PipeLine_->SetRasterizer("EngineBaseRasterizerNone");
	}

	FBXNaviRenderer->GetTransform()->SetLocalScaling({ 3.0f, 3.0f, 3.0f });
	
	FBXWallRenderer = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	FBXWallRenderer->SetFBXMesh("NaviCol.fbx", "TextureDeferredLight");

	for (UINT i = 0; i < FBXWallRenderer->GetRenderSetCount(); i++)
	{
		FBXWallRenderer->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Green.png");
	}

	FBXWallRenderer->GetTransform()->SetLocalScaling({ 3.0f, 3.0f, 3.0f });

	//// SCHOOL ONLY
	//FBXSchoolMap = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	//FBXSchoolMap->SetFBXMesh("School.fbx", "TextureDeferredLight");
	//FBXSchoolMap->GetTransform()->SetLocalScaling({ 3.0f, 3.0f, 3.0f });

	//{
	//	GameEngineFBXRenderer* FBX = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	//	FBX->SetFBXMesh("Downtown.fbx", "TextureDeferredLight");
	//	FBX->GetTransform()->SetLocalScaling({ 3.0f, 3.0f, 3.0f });
	//}

}

void YSJ_LumiaMap::Update(float _DeltaTime)
{
	if (nullptr != YSJ_Player::MainPlayer)
	{
		if (true == FBXWallRenderer->CheckMeshToPointCollision(
			YSJ_Player::MainPlayer->GetTransform()->GetWorldPosition(),float4::DOWN, 5.0f))
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
