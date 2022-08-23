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
	// ����ī�޶� ����
	GetMainCamera()->SetProjectionMode(ProjectionMode::Perspective);
	GetMainCameraActor()->GetTransform()->SetLocalPosition({ 0.0f, 10.0f, -300.0f });

#pragma region IMGUI Windows
	//// ����Ÿ�� IMGUI Window
	//if (nullptr != GameEngineGUI::GetInst()->FindGUIWindow("RenderTargetWindow"))
	//{
	//	GameEngineRenderWindow* Window = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
	//	Window->PushRenderTarget("���� ī�޶� Ÿ��", GetMainCamera()->GetCameraRenderTarget(), float4(128.f, 72.f) * 3);
	//	Window->PushRenderTarget("UI ī�޶� Ÿ��", GetUICamera()->GetCameraRenderTarget(), float4(128.f, 72.f) * 3);
	//	Window->PushRenderTarget("���� ī�޶� ���۵�", GetMainCamera()->GetCameraDeferredGBufferTarget(), float4(128.f, 72.f) * 3);
	//	Window->PushRenderTarget("���� ī�޶� ���۵� ����Ʈ", GetMainCamera()->GetCameraDeferredLightTarget(), float4(128.f, 72.f) * 3);
	//	Window->PushRenderTarget("���� ī�޶� ���۵� ����Ʈ", GetMainCamera()->GetCameraDeferredTarget(), float4(128.f, 72.f) * 3);
	//}
#pragma endregion

#pragma region Ű���
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
	// ���ҽ� �ε� �Ϸ�� ���ͻ���
	static bool ResourceLoadFlag = false;

	if (0 >= UserGame::LoadingFolder &&
		false == ResourceLoadFlag)
	{
		CreateActorLevel();
		ResourceLoadFlag = true;
	}

	// ����ī�޶���
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

	// FBXFolder : ���� �� FBX �� ��� �ִ� ����
	FBXWindow->FBXFolder.MoveParent("FoxTeam");
	FBXWindow->FBXFolder.MoveChild("Resources");
	FBXWindow->FBXFolder.MoveChild("FBX");
	//FBXWindow->FBXFolder.MoveChild("SJH");

	// UserMeshSaveFolder : �޽� ���̳ʸ� ������ ���̺�� ����
	FBXWindow->UserMeshSaveFolder.MoveParent("FoxTeam");
	FBXWindow->UserMeshSaveFolder.MoveChild("Resources");
	FBXWindow->UserMeshSaveFolder.MoveChild("FBX");
	FBXWindow->UserMeshSaveFolder.MoveChild("UserMesh");
	FBXWindow->UserMeshSaveFolder.MoveChild("Character");
	FBXWindow->UserMeshSaveFolder.MoveChild("Yuki");

	// UserAnimationSaveFolder : �ִϸ��̼� ���̳ʸ� ������ ���̺�� ����
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
#pragma region ���ͻ���
	// ���
	Background_ = CreateActor<SKySphereActor>();

	// �÷��̾�(�����÷��̾�� ����)
	Yuki_ = CreateActor<SJH_Yuki>();
	SJH_Yuki::MainPlayer = Yuki_;

	// ��
	FloorMap_ = CreateActor<SJH_FloorMap>();
	SJH_FloorMap::FloorMap = FloorMap_;

	// ���콺
	InGameMouse_ = CreateActor<SJH_Mouse>();
	SJH_Mouse::MainMouse = InGameMouse_;

	// ����
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
