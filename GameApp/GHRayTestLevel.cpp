#include "PreCompile.h"
#include "GHRayTestLevel.h"
#include <GameEngine/GameEngineCore.h>

#include <GameEngine/GameEngineFBXMeshManager.h>
#include <GameEngine/GameEngineRenderWindow.cpp>
#include <GameApp/GHRio.h>

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
	//this->ClearAll();
	//releaseRenderWindow();
	//releaseResource();
}

void GHRayTestLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	//initRenderWindow();
	//loadResource();
	//createActor();

	//GetMainCameraActor()->GetTransform()->SetLocalPosition({ 0.0f, 10.f, -20.f });
}


void GHRayTestLevel::loadResource()
{
	GameEngineDirectory dir;

	dir.MoveParent("FoxTeam");
	dir / "Resources" / "FBX" / "Rio";

	std::string meshName = "Rio_000.fbx";

	GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName(meshName));

	mesh->CreateRenderingBuffer();

	GameEngineFBXAnimation* Animation = GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Rio_000.fbx"));
}

void GHRayTestLevel::releaseResource()
{
	GameEngineFBXMeshManager::GetInst().Delete("Rio_000.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("Rio_000.fbx");
}

void GHRayTestLevel::createActor()
{
	CreateActor<GHRio>();
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
