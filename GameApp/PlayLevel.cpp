#include "PreCompile.h"
#include "PlayLevel.h"
#include "Player.h"
//#include "TestACtor.h"
#include "Monster.h"
#include "TopUI.h"
#include "Map.h"
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

PlayLevel::PlayLevel()
{
}

PlayLevel::~PlayLevel()
{
}

void PlayLevel::LevelStart()
{
	LoadState_.CreateState("Load",std::bind(& PlayLevel::Load_Update,this),std::bind(&PlayLevel::Load_Start,this),std::bind(&PlayLevel::Load_End, this));
	LoadState_.CreateState("Play",std::bind(& PlayLevel::Play_Update,this),std::bind(&PlayLevel::Play_Start,this), std::bind(&PlayLevel::Play_End, this));
	LoadState_.ChangeState("Load");

	//FBXWindow = GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineFBXWindow>("FBXWindow");
	//FBXWindow->FBXFolder.MoveParent("FoxTeam");
	//FBXWindow->FBXFolder.MoveChild("EngineResources");
	//FBXWindow->FBXFolder.MoveChild("FBX");	
	//GameEngineGUIWindow::inst
	//GameEngineDirectory Dir;
	//Dir.MoveParent("AR38");
	//Dir.MoveChild("EngineResources");
	//Dir.MoveChild("FBX");
	//std::vector<GameEngineFile> Files = Dir.GetAllFile("FBX");
	//for (auto& File : Files)
	//{
	//	GameEngineFBXMeshManager::GetInst().Load(File.GetFullPath());
	//}
}

void PlayLevel::LevelUpdate(float _DeltaTime)
{
	LoadState_.Update(_DeltaTime);	
}

void PlayLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
}
void PlayLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	//FBXWindow->TestInit();
}

void PlayLevel::Load_Start()
{
	GameEngineDirectory Directory;
	Directory.MoveParent("FoxTeam");
	Directory.MoveChild("EngineResources");
	Directory.MoveChild("FBX");

	GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(Directory.PathToPlusFileName("AnimMan.fbx"));
	Mesh->CreateRenderingBuffer();

	GameEngineFBXAnimationManager::GetInst().Load(Directory.PathToPlusFileName("ALS_N_RUN_F.FBX"));
}

void PlayLevel::Load_Update()
{
	if (0 >= UserGame::LoadingFolder)
	{
		LoadState_.ChangeState("Play");
	}

}

void PlayLevel::Load_End()
{
}

void PlayLevel::Play_Start()
{

	//GetMainCamera()->SetProjectionMode(ProjectionMode::Perspective);
	//GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.0f, 0.0f, -100.0f));


	//if (false == GameEngineInput::GetInst().IsKey("PlayerMove"))
	//{
	//	GameEngineInput::GetInst().CreateKey("MOn", 'p');
	//	GameEngineInput::GetInst().CreateKey("MOff", 'o');
	//	GameEngineInput::GetInst().CreateKey("LevelControl", 'i');
	//}

	CreateActorLevel();
}

void PlayLevel::Play_Update()
{
	//static bool Check = false;

	//if (false == Check && nullptr != GameEngineGUI::GetInst()->FindGUIWindow("RenderWindow"))
	//{
	//	GameEngineRenderWindow* Window = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
	//	float4 Size = { 128, 72 };
	//	Window->PushRenderTarget("메인 카메라 타겟", GetMainCamera()->GetCameraRenderTarget(), Size * 3);
	//	Window->PushRenderTarget("UI 카메라 타겟", GetUICamera()->GetCameraRenderTarget(), Size * 3);
	//	Check = true;
	//}

	//if (true == GameEngineInput::GetInst().Down("LevelControl"))
	//{
	//	GameEngineGUIWindow* Window = GameEngineGUI::GetInst()->FindGUIWindow("LevelControlWindow");

	//	Window->OnOffChange();
	//}

	//if (true == GameEngineInput::GetInst().Down("MOn"))
	//{
	//	GameEngineGUIWindow* Window = GameEngineGUI::GetInst()->FindGUIWindow("RenderWindow");

	//	Window->OnOffChange();
	//}

	//if (true == GameEngineInput::GetInst().Down("MOff"))
	//{
	//	//Window->Off();
	//	//MActor->WindowCursorOff();

	//}
}

void PlayLevel::Play_End()
{
}

void PlayLevel::CreateActorLevel()
{
	//{
	//	MActor = CreateActor<MouseActor>();
	//	MActor->GetUIRenderer()->SetRenderGroup(1000);
	//}

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
		Map* Actor = CreateActor<Map>();
	}

	{
		SKySphereActor* Actor = CreateActor<SKySphereActor>();
		// Actor->SetRadius(5000);
	}
	


	{
		Player* Actor = CreateActor<Player>();
	}

	{
		TopUI* Actor = CreateActor<TopUI>();
		Actor->GetTransform()->SetWorldPosition(float4(0.0f, 0.0f, 0.0f));
	}
}