#include "PreCompile.h"
#include "PlayLevel.h"
#include "Player.h"
#include "TopUI.h"

#include "Map.h"

#include <GameEngine/CameraComponent.h>
#include <GameEngine/GameEngineTransform.h>
#include <GameEngine/CameraActor.h>
#include <GameEngine/LightActor.h>
#include <GameEngine/GameEngineGUI.h>
#include <GameEngine/GameEngineRenderWindow.h>
#include <GameEngine/SKySphereActor.h>
#include <GameEngine/GameEngineFBXWindow.h>
#include <GameEngine/GameEngineFBXMesh.h>

#include <GameEngine/GameEngineImageRenderer.h>
#include "UserGame.h"

PlayLevel::PlayLevel() 
{
}

PlayLevel::~PlayLevel() 
{
}

void PlayLevel::LevelStart()
{
	// 레벨 진입전 리소스 로드
	LoadState_.CreateState<PlayLevel>("Load",this,& PlayLevel::Load_Start,&PlayLevel::Load_Update,&PlayLevel::Load_End);
	// 리소스 로드 완료 후 게임 시작
	LoadState_.CreateState<PlayLevel>("Play",this,& PlayLevel::Play_Start,&PlayLevel::Play_Update,&PlayLevel::Play_End);

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
	//{
	//	GameEngineActor* loading = CreateActor<GameEngineActor>();
	//	GameEngineImageRenderer* loadingimage = loading->CreateTransformComponent<GameEngineImageRenderer>();
	//	loadingimage->SetImage("TestLoading.jpg");
	//	loadingimage->GetTransform()->SetLocalScaling({ 1920.f,1080.f,1.f });
	//	loadingimage->GetTransform()->SetWorldPosition({ 0.f,0.f, -400.f });
	//}

	GameEngineDirectory Directory;
	Directory.MoveParent("FoxTeam");
	Directory.MoveChild("EngineResources");
	Directory.MoveChild("FBX");

	GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(Directory.PathToPlusFileName("AnimMan.fbx"));
	Mesh->CreateRenderingBuffer();

	GameEngineFBXAnimationManager::GetInst().Load(Directory.PathToPlusFileName("ALS_N_RUN_F.FBX"));
}

void PlayLevel::Load_Update(float _DeltaTime)
{
	if (0 >= UserGame::LoadingFolder)
	{
		// 로드 완료되면 플레이 상태로 변경
		LoadState_.ChangeState("Play");
	}

}

void PlayLevel::Load_End()
{
}

void PlayLevel::Play_Start()
{
	//로드 완료후 처음 실행할 함수
	// 플레이어 생성 등등...

	/////////////////////////////
	//프리카메라 주석 쳐놓음
	/////////////////////////////


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

void PlayLevel::Play_Update(float _DeltaTime)
{	
	// 로드 완료후 업데이트될 함수

	/////////////////////////////
	//프리카메라 주석 쳐놓음
	/////////////////////////////
	// 
	// 
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
		SKySphereActor* Actor = CreateActor<SKySphereActor>();
		// Actor->SetRadius(5000);
	}

	{
		Player* Actor = CreateActor<Player>();
	}

	{
		Map* Actor = CreateActor<Map>(); 
	}

	{
		//TopUI* Actor = CreateActor<TopUI>();
		//Actor->GetTransform()->SetWorldPosition(float4(0.0f, 0.0f, 0.0f));

		//플레이어로 이동
	}
}