#include "PreCompile.h"
#include "YSJ_PlayLevel.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngine/LightActor.h>
#include "UserGame.h"
#include <GameEngine/SKySphereActor.h>
#include <GameEngine/GameEngineRenderWindow.h>

#include "YSJ_LumiaMap.h"
#include "YSJ_Player.h"
#include "YSJ_Char.h"
#include "YSJ_Mouse.h"

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

	GameEngineInput::GetInst().CreateKey("MoveLeft", 'A');
	GameEngineInput::GetInst().CreateKey("MoveRight", 'D');
	GameEngineInput::GetInst().CreateKey("MoveForward", 'W');
	GameEngineInput::GetInst().CreateKey("MoveBack", 'S');
	GameEngineInput::GetInst().CreateKey("MoveUp", 'Q');
	GameEngineInput::GetInst().CreateKey("MoveDown", 'E');
	GameEngineInput::GetInst().CreateKey("FreeCameraOn", 'o');
	GameEngineInput::GetInst().CreateKey("LBUTTON", VK_LBUTTON);

	YSJ_Player* Player = CreateActor<YSJ_Player>();
	Player->GetTransform()->SetWorldMove(float4::RIGHT * 15.0f);

	GameEngineDirectory tempDir;

	tempDir.MoveParent("FoxTeam");
	tempDir.MoveChild("Resources");
	tempDir.MoveChild("FBX");
	tempDir.MoveChild("YSJ");

	//std::vector<GameEngineFile> vecFile = tempDir.GetAllFile("FBX");

	//for (size_t i = 0; i < vecFile.size(); i++)
	//{
	//	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(vecFile[i].GetFullPath()))
	//	{
	//		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(vecFile[i].GetFullPath());
	//		Mesh->CreateRenderingBuffer();
	//	}
	//}

	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(tempDir.PathToPlusFileName("Bg_NaviMesh.fbx")))
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(tempDir.PathToPlusFileName("Bg_NaviMesh.fbx"));
		Mesh->CreateRenderingBuffer();
	}

	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(tempDir.PathToPlusFileName("NaviCol.fbx")))
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(tempDir.PathToPlusFileName("NaviCol.fbx"));
		Mesh->CreateRenderingBuffer();
	}

	//if (nullptr == GameEngineFBXMeshManager::GetInst().Find(tempDir.PathToPlusFileName("School.fbx")))
	//{
	//	GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(tempDir.PathToPlusFileName("School.fbx"));
	//	Mesh->CreateRenderingBuffer();
	//}
	//
	//if (nullptr == GameEngineFBXMeshManager::GetInst().Find(tempDir.PathToPlusFileName("Downtown.fbx")))
	//{
	//	GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(tempDir.PathToPlusFileName("Downtown.fbx"));
	//	Mesh->CreateRenderingBuffer();
	//}

	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(tempDir.PathToPlusFileName("TestBox.fbx")))
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(tempDir.PathToPlusFileName("TestBox.fbx"));
		Mesh->CreateRenderingBuffer();
	}

	tempDir.MoveParent("FoxTeam");
	tempDir.MoveChild("EngineResources");
	tempDir.MoveChild("FBX");

	//if (nullptr == GameEngineFBXMeshManager::GetInst().Find(tempDir.PathToPlusFileName("AnimMan.fbx")))
	//{
	//	GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(tempDir.PathToPlusFileName("AnimMan.fbx"));
	//	Mesh->CreateRenderingBuffer();
	//	GameEngineFBXAnimationManager::GetInst().Load(tempDir.PathToPlusFileName("ALS_N_RUN_F.FBX"));
	//}


	//YSJ_Char* Player = CreateActor<YSJ_Char>();
	//Player->GetTransform()->SetWorldScaling({0.5f,0.5f, 0.5f });

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

void YSJ_PlayLevel::CreateActorLevel()
{
	LightActor* Light1 = CreateActor<LightActor>();
	Light1->GetLight()->SetDiffusePower(0.3f);
	Light1->GetLight()->SetSpacularLightPow(50.0f);

	LightActor* Light2 = CreateActor<LightActor>();
	Light2->GetLight()->SetDiffusePower(0.3f);
	Light2->GetLight()->SetSpacularLightPow(50.0f);
	Light2->GetTransform()->SetLocalRotationDegree(float4(0.0f, 90.0f, 0.0f));

	LightActor* Light3 = CreateActor<LightActor>();
	Light3->GetLight()->SetDiffusePower(0.3f);
	Light3->GetLight()->SetSpacularLightPow(50.0f);
	Light3->GetTransform()->SetLocalRotationDegree(float4(45.0f, 0.0f, 0.0f));

	SKySphereActor* SkyActor = CreateActor<SKySphereActor>();
	YSJ_LumiaMap* LumiaMap = CreateActor<YSJ_LumiaMap>();
	YSJ_Mouse* Mouse = CreateActor<YSJ_Mouse>();
	Mouse->SetPickingRenderer(LumiaMap->GetFBXNaviRenderer());
}
