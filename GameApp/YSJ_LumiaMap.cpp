#include "PreCompile.h"
#include "YSJ_LumiaMap.h"

YSJ_LumiaMap::YSJ_LumiaMap()
	: FBXNaviMesh(nullptr)
	, FBXWallMesh(nullptr)
	, FBXSchoolMap(nullptr)
{

}

YSJ_LumiaMap::~YSJ_LumiaMap()
{

}

void YSJ_LumiaMap::Start()
{
	FBXNaviMesh = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	FBXNaviMesh->SetFBXMesh("Bg_NaviMesh.fbx", "TextureDeferredLight");

	//for (UINT i = 0; i < FBXNaviMesh->GetRenderSetCount(); i++)
	//{
	//	FBXNaviMesh->GetRenderSet(i).ShaderHelper->SettingConstantBufferSet("ResultColor", float4::RED);
	//}

	for (UINT i = 0; i < FBXNaviMesh->GetRenderSetCount(); i++)
	{
		FBXNaviMesh->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Red.png");
	}

	FBXNaviMesh->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	FBXNaviMesh->GetTransform()->SetLocalScaling({ 3.0f, 3.0f, 3.0f });
	FBXNaviMesh->GetTransform()->SetLocalRotationDegree({ 0.0f, 0.0f, 0.0f });
	
	FBXWallMesh = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	FBXWallMesh->SetFBXMesh("NaviCol.fbx", "TextureDeferredLight");

	//for (UINT i = 0; i < FBXWallMesh->GetRenderSetCount(); i++)
	//{
	//	FBXWallMesh->GetRenderSet(i).ShaderHelper->SettingConstantBufferSet("ResultColor", float4::RED);
	//}

	for (UINT i = 0; i < FBXWallMesh->GetRenderSetCount(); i++)
	{
		FBXWallMesh->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Green.png");
	}

	FBXWallMesh->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	FBXWallMesh->GetTransform()->SetLocalScaling({ 3.0f, 3.0f, 3.0f });
	FBXWallMesh->GetTransform()->SetLocalRotationDegree({ 0.0f, 0.0f, 0.0f });

	// SCHOOL ONLY
	FBXSchoolMap = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	FBXSchoolMap->SetFBXMesh("School.fbx", "TextureDeferredLight");

	FBXSchoolMap->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	FBXSchoolMap->GetTransform()->SetLocalScaling({ 3.0f, 3.0f, 3.0f });
	FBXSchoolMap->GetTransform()->SetLocalRotationDegree({ 0.0f, 0.0f, 0.0f });

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
