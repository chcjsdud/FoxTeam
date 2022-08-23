#include "PreCompile.h"
#include "SJH_PlayLevel.h"

#include <GameEngine/GameEngineGUI.h>
#include <GameEngine/GameEngineFBXWindow.h>
#include <GameEngine/GameEngineRenderWindow.h>
#include <GameEngine/SKySphereActor.h>
#include <GameEngine/LightActor.h>

#include "UserGame.h"
#include "SJH_FloorMap.h"
#include "SJH_Mouse.h"
#include "SJH_Yuki.h"

#include "SJH_TestActor.h"

void SJH_PlayLevel::LevelStart()
{
	// 메인카메라 설정
	GetMainCamera()->SetProjectionMode(ProjectionMode::Perspective);
	GetMainCameraActor()->GetTransform()->SetLocalPosition({ 0.0f, 10.0f, -300.0f });

#pragma region IMGUI Windows
	//// 렌더타겟 IMGUI Window
	//if (nullptr != GameEngineGUI::GetInst()->FindGUIWindow("RenderTargetWindow"))
	//{
	//	GameEngineRenderWindow* Window = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
	//	Window->PushRenderTarget("메인 카메라 타겟", GetMainCamera()->GetCameraRenderTarget(), float4(128.f, 72.f) * 3);
	//	Window->PushRenderTarget("UI 카메라 타겟", GetUICamera()->GetCameraRenderTarget(), float4(128.f, 72.f) * 3);
	//	Window->PushRenderTarget("메인 카메라 디퍼드", GetMainCamera()->GetCameraDeferredGBufferTarget(), float4(128.f, 72.f) * 3);
	//	Window->PushRenderTarget("메인 카메라 디퍼드 라이트", GetMainCamera()->GetCameraDeferredLightTarget(), float4(128.f, 72.f) * 3);
	//	Window->PushRenderTarget("메인 카메라 디퍼드 라이트", GetMainCamera()->GetCameraDeferredTarget(), float4(128.f, 72.f) * 3);
	//}
#pragma endregion

#pragma region 키등록
	if (false == GameEngineInput::GetInst().IsKey("FreeCam"))
	{
		GameEngineInput::GetInst().CreateKey("FreeCam", 'o');
	}

	if (false == GameEngineInput::GetInst().IsKey("LBUTTON"))
	{
		GameEngineInput::GetInst().CreateKey("LBUTTON", VK_LBUTTON);
	}

	if (false == GameEngineInput::GetInst().IsKey("RBUTTON"))
	{
		GameEngineInput::GetInst().CreateKey("RBUTTON", VK_RBUTTON);
	}

#pragma endregion
}

void SJH_PlayLevel::LevelUpdate(float _DeltaTime)
{
	// 리소스 로드 완료시 액터생성
	static bool ResourceLoadFlag = false;

	if (0 >= UserGame::LoadingFolder &&
		false == ResourceLoadFlag)
	{
		CreateActorLevel();
		ResourceLoadFlag = true;
	}

	// 프리카메라모드
	if (true == GameEngineInput::GetInst().Down("FreeCam"))
	{
		GetMainCameraActor()->FreeCameraModeSwitch();
	}

	if (true == GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}

	// ...

}

void SJH_PlayLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{

}

void SJH_PlayLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	FBXWindow = GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineFBXWindow>("FBXWindow");

	// FBXFolder : 가져 올 FBX 가 들어 있는 폴더
	FBXWindow->FBXFolder.MoveParent("FoxTeam");
	FBXWindow->FBXFolder.MoveChild("Resources");
	FBXWindow->FBXFolder.MoveChild("FBX");
	//FBXWindow->FBXFolder.MoveChild("SJH");

	// UserMeshSaveFolder : 메시 바이너리 파일이 세이브될 폴더
	FBXWindow->UserMeshSaveFolder.MoveParent("FoxTeam");
	FBXWindow->UserMeshSaveFolder.MoveChild("Resources");
	FBXWindow->UserMeshSaveFolder.MoveChild("FBX");
	FBXWindow->UserMeshSaveFolder.MoveChild("UserMesh");
	FBXWindow->UserMeshSaveFolder.MoveChild("Character");
	FBXWindow->UserMeshSaveFolder.MoveChild("Yuki");

	// UserAnimationSaveFolder : 애니메이션 바이너리 파일이 세이브될 폴더
	FBXWindow->UserAnimationSaveFolder.MoveParent("FoxTeam");
	FBXWindow->UserAnimationSaveFolder.MoveChild("Resources");
	FBXWindow->UserAnimationSaveFolder.MoveChild("FBX");
	FBXWindow->UserAnimationSaveFolder.MoveChild("UserAni");
	FBXWindow->UserAnimationSaveFolder.MoveChild("Character");
	FBXWindow->UserAnimationSaveFolder.MoveChild("Yuki");
	FBXWindow->UserAnimationSaveFolder.MoveChild("Common");
}

void SJH_PlayLevel::CreateActorLevel()
{
#pragma region 액터생성
	// 배경
	Background_ = CreateActor<SKySphereActor>();

	// 플레이어(메인플레이어로 지정)
	Yuki_ = CreateActor<SJH_Yuki>();
	SJH_Yuki::MainPlayer = Yuki_;

	// 맵
	FloorMap_ = CreateActor<SJH_FloorMap>();
	SJH_FloorMap::FloorMap = FloorMap_;

	// 마우스
	InGameMouse_ = CreateActor<SJH_Mouse>();
	SJH_Mouse::MainMouse = InGameMouse_;

	// 조명
	LightActor* Light1 = CreateActor<LightActor>();
	Light1->GetLight()->SetDiffusePower(1.f);
	Light1->GetLight()->SetAmbientPower(10.f);
	Light1->GetLight()->SetSpacularLightPow(10.0f);

#pragma endregion

}

SJH_PlayLevel::SJH_PlayLevel()
	: FBXWindow(nullptr)
	, Background_(nullptr)
	, FloorMap_(nullptr)
	, InGameMouse_(nullptr)
	, Yuki_(nullptr)
	, TestBox_(nullptr)
{

}

SJH_PlayLevel::~SJH_PlayLevel()
{
}
