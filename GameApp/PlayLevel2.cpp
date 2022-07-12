#include "PreCompile.h"
#include "PlayLevel2.h"
#include "Player.h"
#include "Monster.h"
#include "TopUI.h"
#include "Map.h"
#include <GameEngine\PostFade.h>
#include <GameEngine/CameraComponent.h>
#include <GameEngine/GameEngineTransform.h>
#include <GameEngine/CameraActor.h>
#include <GameEngine/MouseActor.h>
#include <GameEngine/GameEngineGUI.h>
#include <GameEngine/GameEngineRenderWindow.h>
#include "UserGame.h"

PlayLevel2::PlayLevel2()
{
}

PlayLevel2::~PlayLevel2()
{
}

void PlayLevel2::LevelStart()
{

	GetMainCamera()->SetProjectionMode(ProjectionMode::Orthographic);
	GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.0f, 0.0f, -100.0f));


	if (false == GameEngineInput::GetInst().IsKey("PlayerMove"))
	{
		GameEngineInput::GetInst().CreateKey("MOn", 'p');
		GameEngineInput::GetInst().CreateKey("MOff", 'o');
		GameEngineInput::GetInst().CreateKey("LevelControl", 'i');
	}

}

void PlayLevel2::LevelUpdate(float _DeltaTime)
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

	if (true == GameEngineInput::GetInst().Down("LevelControl"))
	{
		GameEngineGUIWindow* Window = GameEngineGUI::GetInst()->FindGUIWindow("LevelControlWindow");

		Window->OnOffChange();
	}

	if (true == GameEngineInput::GetInst().Down("MOn"))
	{
		GameEngineGUIWindow* Window = GameEngineGUI::GetInst()->FindGUIWindow("RenderWindow");

		Window->OnOffChange();
	}

	if (true == GameEngineInput::GetInst().Down("MOff"))
	{
		//Window->Off();
		//MActor->WindowCursorOff();

	}
}

// 지금 내가 보스방이야.
// 지금 내가 보스방이야.
void PlayLevel2::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
}

void PlayLevel2::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{

}

void PlayLevel2::CreateActorLevel()
{
	GameEngineTexture* Texture = GameEngineTextureManager::GetInst().Find("Animation.png");
	Texture->Cut(8, 8);

	{
		MActor = CreateActor<MouseActor>();
		MActor->GetUIRenderer()->SetRenderGroup(1000);
	}

	if (nullptr == Player::MainPlayer)
	{
		Player* Actor = CreateActor<Player>();
		GetMainCameraActor()->GetTransform()->SetWorldPosition(Actor->GetTransform()->GetLocalPosition());
	}
}