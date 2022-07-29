#include "PreCompile.h"
#include "YSJ_LumiaMap.h"

YSJ_LumiaMap::YSJ_LumiaMap()
	: FBXNaviMesh(nullptr)
	, FBXWallMesh(nullptr)
{

}

YSJ_LumiaMap::~YSJ_LumiaMap()
{

}

void YSJ_LumiaMap::Start()
{
	FBXNaviMesh = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	FBXNaviMesh->SetFBXMesh("Bg_NaviMesh.fbx", "Texture");

	FBXNaviMesh->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	FBXNaviMesh->GetTransform()->SetLocalScaling({ 3.0f, 3.0f, 3.0f });
	FBXNaviMesh->GetTransform()->SetLocalRotationDegree({ 180.0f, 0.0f, 0.0f });
	
	FBXWallMesh = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	FBXWallMesh->SetFBXMesh("NaviCol.fbx", "Texture");
	FBXWallMesh->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	FBXWallMesh->GetTransform()->SetLocalScaling({ 3.0f, 3.0f, 3.0f });
	FBXWallMesh->GetTransform()->SetLocalRotationDegree({ 180.0f, 0.0f, 0.0f });
}

void YSJ_LumiaMap::Update(float _DeltaTime)
{
	
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
