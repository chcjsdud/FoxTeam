#include "PreCompile.h"
#include "YSJ_PlayLevel.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngine/LightActor.h>
#include "UserGame.h"
#include <GameEngine/SKySphereActor.h>

#include "YSJ_LumiaMap.h"


YSJ_PlayLevel::YSJ_PlayLevel()
{

}

YSJ_PlayLevel::~YSJ_PlayLevel()
{

}

void YSJ_PlayLevel::LevelStart()
{
	GetMainCamera()->SetProjectionMode(ProjectionMode::Perspective);
	GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.0f, 300.0f, -300.0f));
	GetMainCameraActor()->GetTransform()->AddLocalRotationDegreeX(30.0f);

	GameEngineDirectory tempDir;

	tempDir.MoveParent("FoxTeam");
	tempDir.MoveChild("Resources");
	tempDir.MoveChild("FBX");
	tempDir.MoveChild("YSJ");
	tempDir.MoveChild("LumiaNaviMesh");

	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(tempDir.PathToPlusFileName("Bg_NaviMesh.fbx")))
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(tempDir.PathToPlusFileName("Bg_NaviMesh.fbx"));
		Mesh->CreateRenderingBuffer();
	}

	tempDir.MoveParent("YSJ");
	tempDir.MoveChild("LumiaWallMesh");

	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(tempDir.PathToPlusFileName("NaviCol.fbx")))
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(tempDir.PathToPlusFileName("NaviCol.fbx"));
		Mesh->CreateRenderingBuffer();
	}

	if (false == GameEngineInput::GetInst().IsKey("FreeCam"))
	{
		GameEngineInput::GetInst().CreateKey("FreeCam", 'o');
	}

	GetMainCameraActor()->FreeCameraModeSwitch();
}

void YSJ_PlayLevel::LevelUpdate(float _DeltaTime)
{
	static bool isLoaded = false;

	if (0 >= UserGame::LoadingFolder &&
		false == isLoaded)
	{
		CreateActorLevel();
		isLoaded = true;
	}

	if (true == GameEngineInput::GetInst().Down("FreeCam"))
	{
		GetMainCameraActor()->FreeCameraModeSwitch();
	}
}

void YSJ_PlayLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
}

void YSJ_PlayLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
}

void YSJ_PlayLevel::CreateActorLevel()
{
	LightActor* Light1 = CreateActor<LightActor>();
	Light1->GetLight()->SetDiffusePower(0.3f);
	Light1->GetLight()->SetSpacularLightPow(50.0f);

	LightActor* Light2 = CreateActor<LightActor>();
	Light2->GetLight()->SetDiffusePower(0.3f);
	Light2->GetLight()->SetSpacularLightPow(50.0f);
	Light2->GetTransform()->SetLocalRotationDegree(float4(0.0f, 90.0f, 0.0f));

	SKySphereActor* SkyActor = CreateActor<SKySphereActor>();
	YSJ_LumiaMap* LumiaMap = CreateActor<YSJ_LumiaMap>();

}
