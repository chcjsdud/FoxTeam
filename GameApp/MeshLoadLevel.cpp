#include "PreCompile.h"
#include "MeshLoadLevel.h"
#include "Player.h"
#include "TopUI.h"
#include <GameEngine/PostFade.h>
#include <GameEngine/PostBlur.h>
#include <GameEngine/SmallPostBlur.h>
#include <GameEngine/CameraComponent.h>
#include <GameEngine/GameEngineTransform.h>
#include <GameEngine/CameraActor.h>
#include <GameEngine/MouseActor.h>
#include <GameEngine/LightActor.h>
#include <GameEngine/GameEngineGUI.h>
#include <GameEngine/GameEngineRenderWindow.h>
#include <GameEngine/SKySphereActor.h>
#include <GameEngine/GameEngineFBXWindow.h>
#include "UserGame.h"

MeshLoadLevel::MeshLoadLevel()
{
}

MeshLoadLevel::~MeshLoadLevel()
{
}

void MeshLoadLevel::LevelStart()
{
	FBXWindow = GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineFBXWindow>("FBXWindow");

	FBXWindow->FBXFolder.MoveParent("FoxTeam");
	FBXWindow->FBXFolder.MoveChild("EngineResources");
	FBXWindow->FBXFolder.MoveChild("FBX");

	GetMainCamera()->SetProjectionMode(ProjectionMode::Perspective);
	GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.0f, 0.0f, -100.0f));
}

void MeshLoadLevel::LevelUpdate(float _DeltaTime)
{
	static bool CreateActorCheck = false;

	if (0 >= UserGame::LoadingFolder
		&& false == CreateActorCheck)
	{
		CreateActorLevel();
		CreateActorCheck = true;
	}

	static bool Check = false;

	if (false == Check && nullptr != GameEngineGUI::GetInst()->FindGUIWindow("RenderWindow"))
	{
		GameEngineRenderWindow* Window = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
		float4 Size = { 128, 72 };
		Window->PushRenderTarget("메인 카메라 타겟", GetMainCamera()->GetCameraRenderTarget(), Size * 3);
		Window->PushRenderTarget("UI 카메라 타겟", GetUICamera()->GetCameraRenderTarget(), Size * 3);
		Check = true;
	}
}

void MeshLoadLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
	

}
void MeshLoadLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	FBXWindow->TestInit();
}

void MeshLoadLevel::CreateActorLevel()
{
	static bool Check = false;

	if (true == Check)
	{
		return;
	}

	{
		MActor = CreateActor<MouseActor>();
		MActor->GetUIRenderer()->SetRenderGroup(1000);
	}

	{
		LightActor* Actor;

		
		Actor = CreateActor<LightActor>();
		Actor->GetLight()->SetDiffusePower(0.3f);
		Actor->GetLight()->SetSpacularLightPow(50.0f);

		Actor = CreateActor<LightActor>();
		Actor->GetLight()->SetDiffusePower(0.3f);
		Actor->GetLight()->SetSpacularLightPow(50.0f);
		Actor->GetTransform()->SetLocalRotationDegree({45.0f, 0.0f, 0.0f});
	}


	{
		Player* Actor = CreateActor<Player>();
	}

	{
		SKySphereActor* Actor = CreateActor<SKySphereActor>();
		// Actor->SetRadius(5000);
	}
	

	{
		TopUI* Actor = CreateActor<TopUI>();
		Actor->GetTransform()->SetWorldPosition(float4(0.0f, 0.0f, 0.0f));
	}

	Check = true;
}