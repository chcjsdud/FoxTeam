#include "PreCompile.h"
#include "SJH_PlayLevel.h"

#include <GameEngine/GameEngineGUI.h>
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
	GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.0f, 0.0f, -300.0f));

#pragma region IMGUI Windows
	// ����Ÿ�� IMGUI Window
	if (nullptr != GameEngineGUI::GetInst()->FindGUIWindow("RenderTargetWindow"))
	{
		GameEngineRenderWindow* Window = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
		Window->PushRenderTarget("���� ī�޶� Ÿ��", GetMainCamera()->GetCameraRenderTarget(), float4(128.f, 72.f) * 3);
		Window->PushRenderTarget("UI ī�޶� Ÿ��", GetUICamera()->GetCameraRenderTarget(), float4(128.f, 72.f) * 3);
		Window->PushRenderTarget("���� ī�޶� ���۵�", GetMainCamera()->GetCameraDeferredGBufferTarget(), float4(128.f, 72.f) * 3);
		Window->PushRenderTarget("���� ī�޶� ���۵� ����Ʈ", GetMainCamera()->GetCameraDeferredLightTarget(), float4(128.f, 72.f) * 3);
		Window->PushRenderTarget("���� ī�޶� ���۵� ����Ʈ", GetMainCamera()->GetCameraDeferredTarget(), float4(128.f, 72.f) * 3);
	}

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

}

void SJH_PlayLevel::CreateActorLevel()
{
#pragma region ���ͻ���
	// ����
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

	// ���
	Background_ = CreateActor<SKySphereActor>();

	// ��
	//FloorMap_ = CreateActor<SJH_FloorMap>();

	// ���콺
	InGameMouse_ = CreateActor<SJH_Mouse>();

	// �÷��̾�(�����÷��̾�� ����)
	Yuki_ = CreateActor<SJH_Yuki>();
	SJH_Yuki::MainPlayer = Yuki_;

	// ���콺��ŷ �׽�Ʈ�� ������Ʈ
	TestBox_ = CreateActor<SJH_TestActor>();
	TestBox_->SetName("test1");
	TestBox_->GetTransform()->SetLocalPosition(float4(50.f, 0.f, 10.f));
	TestBox_->GetTransform()->SetLocalScaling(float4(10.f, 10.f, 10.f));

	// 
	SJH_TestActor* TextBox2 = CreateActor<SJH_TestActor>();
	TextBox2->SetName("test2");
	TextBox2->GetTransform()->SetLocalPosition(float4(-50.f, 10.f, 200.f));
	TextBox2->GetTransform()->SetLocalScaling(float4(10.f, 10.f, 10.f));
	TextBox2->SetResultColor(float4(1.f, 0.f, 1.f));

	SJH_TestActor* TextBox3 = CreateActor<SJH_TestActor>();
	TextBox3->SetName("test3");
	TextBox3->GetTransform()->SetLocalPosition(float4(-50.f, 10.f, 300.f));
	TextBox3->GetTransform()->SetLocalScaling(float4(10.f, 10.f, 10.f));
	TextBox3->SetResultColor(float4(1.f, 1.f, 1.f));
#pragma endregion

}

SJH_PlayLevel::SJH_PlayLevel()
	: Background_(nullptr)
	, FloorMap_(nullptr)
	, InGameMouse_(nullptr)
	, Yuki_(nullptr)
	, TestBox_(nullptr)
{

}

SJH_PlayLevel::~SJH_PlayLevel()
{
}
