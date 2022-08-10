#include "PreCompile.h"
#include "GHRayTestLevel.h"
#include <GameEngine/GameEngineCore.h>

#include <GameEngine/GameEngineFBXMeshManager.h>
#include <GameEngine/GameEngineRenderWindow.cpp>
#include <GameApp/GHRio.h>
#include <GameEngine/LightActor.h>
#include <GameEngine/SKySphereActor.h>
#include <GameApp/GHMap.h>
#include <GameApp/GHRayTestBox.h>
#include <GameApp/GHMousePointer.h>
#include <GameApp/GHRayTestOBBBox.h>
#include <GameApp/GHRayTestSphere.h>

GHRayTestLevel::GHRayTestLevel()
{

}

GHRayTestLevel::~GHRayTestLevel()
{

}

void GHRayTestLevel::LevelStart()
{
	GameEngineInput::GetInst().CreateKey("A", 'A');
	GameEngineInput::GetInst().CreateKey("D", 'D');
	GameEngineInput::GetInst().CreateKey("W", 'W');
	GameEngineInput::GetInst().CreateKey("S", 'S');
	GameEngineInput::GetInst().CreateKey("Q", 'Q');
	GameEngineInput::GetInst().CreateKey("E", 'E');
	GameEngineInput::GetInst().CreateKey("O", 'o');

}

void GHRayTestLevel::LevelUpdate(float _DeltaTime)
{
	if (GameEngineInput::GetInst().Down("O"))
	{
		GetMainCameraActor()->FreeCameraModeSwitch();
	}
}

void GHRayTestLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
	this->ClearAll();
	releaseRenderWindow();
	releaseResource();
}

void GHRayTestLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	initRenderWindow();
	loadResource();
	createActor();
	adjustCamera();
}


void GHRayTestLevel::loadResource()
{
	GameEngineDirectory dir;

	dir.MoveParent("FoxTeam");
	dir / "Resources" / "FBX" / "GH";

	std::string meshName = "Rio_Run.fbx";

	GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Rio_Run.fbx"));
	mesh->CreateRenderingBuffer();

	mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Bg_NaviMesh_Cobalt.fbx"));
	mesh->CreateRenderingBuffer();

	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Rio_Run.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Rio_Wait.fbx"));
}

void GHRayTestLevel::releaseResource()
{
	std::string meshName = "Rio_Run.fbx";
	GameEngineFBXMeshManager::GetInst().Delete(meshName);
	GameEngineFBXAnimationManager::GetInst().Delete(meshName);
}

void GHRayTestLevel::createActor()
{
	//CreateActor<GHRio>();
	CreateActor<GHMap>();
	CreateActor<GHMousePointer>();

	for (int z = 0; z < 10; z++)
	{
		for (int x = 0; x < 10; x++)
		{
			GHRayTestBox* box = CreateActor<GHRayTestBox>();
			box->GetTransform()->SetWorldPosition({ 200.f * x, 0.0f, 200.f * -z });
		}
	}

	for (int z = 0; z < 10; z++)
	{
		for (int x = 0; x < 10; x++)
		{
			GHRayTestOBBBox* box = CreateActor<GHRayTestOBBBox>();
			box->GetTransform()->SetWorldPosition({ 200.f * x, 0.0f, 300 + 200.f * z });
		}
	}

	for (int z = 0; z < 10; z++)
	{
		for (int x = 0; x < 10; x++)
		{
			GHRayTestSphere* box = CreateActor<GHRayTestSphere>();
			box->GetTransform()->SetWorldPosition({ -200.f - 200.f * x, 0.0f, 200.f * z });
		}
	}

	SKySphereActor* Actor = CreateActor<SKySphereActor>();


	{
		LightActor* Actor;


		Actor = CreateActor<LightActor>();
		Actor->GetLight()->SetDiffusePower(1.f);
		Actor->GetLight()->SetAmbientPower(10.f);
		Actor->GetLight()->SetSpacularLightPow(10.f);

		//Actor = CreateActor<LightActor>();
		//Actor->GetLight()->SetDiffusePower(0.3f);
		//Actor->GetLight()->SetSpacularLightPow(50.0f);
		////Actor->GetTransform()->SetLocalRotationDegree({ 45.0f, 0.0f, 0.0f });

		//Actor = CreateActor<LightActor>();
		//Actor->GetLight()->SetDiffusePower(0.3f);
		//Actor->GetLight()->SetSpacularLightPow(50.0f);
		////Actor->GetTransform()->SetLocalRotationDegree({ 0.0f, 90.0f, 0.0f });
	}
}

void GHRayTestLevel::adjustCamera()
{
	GetMainCameraActor()->GetCamera()->SetFov(60.f);
	GetMainCameraActor()->FreeCameraModeSwitch();
	GetMainCameraActor()->GetTransform()->SetWorldPosition({ 0.0f, 100.f, -200.f });
}

void GHRayTestLevel::initRenderWindow()
{
	if (nullptr != GameEngineGUI::GetInst()->FindGUIWindow("RenderWindow"))
	{
		GameEngineRenderWindow* Window = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
		float4 Size = { 128, 72 };
		Window->PushRenderTarget("메인 카메라 타겟", GetMainCamera()->GetCameraRenderTarget(), Size * 3);
		Window->PushRenderTarget("UI 카메라 타겟", GetUICamera()->GetCameraRenderTarget(), Size * 3);
		Window->PushRenderTarget("메인 카메라 디퍼드", GetMainCamera()->GetCameraDeferredGBufferTarget(), Size * 3);
		Window->PushRenderTarget("메인 카메라 디퍼드 라이트", GetMainCamera()->GetCameraDeferredLightTarget(), Size * 3);
		Window->PushRenderTarget("메인 카메라 디퍼드 라이트", GetMainCamera()->GetCameraDeferredTarget(), Size * 3);
	}
}

void GHRayTestLevel::releaseRenderWindow()
{
	if (nullptr != GameEngineGUI::GetInst()->FindGUIWindow("RenderWindow"))
	{
		GameEngineRenderWindow* Window = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
		Window->ClaerRenderTarget();
	}
}
