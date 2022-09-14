#include "Precompile.h"
#include "TitleLevel.h"

#include "TitleIDInput.h"
#include "UI_TestMouse.h"
#include "TitleUIController.h"
#include "TitleStartButton.h"
#include "TitlePrologueBG.h"

#include "PlayerInfoManager.h"
#include "UserGame.h"

TitleLevel::TitleLevel()
	:isTyped_(false), fadeWaitTime_(3.0f), mandatoryFadeTime_(0.0f), fadeTime_(1.5f)
{
}

TitleLevel::~TitleLevel()
{
}

void TitleLevel::LevelStart()
{
	{
		GetMainCamera()->SetProjectionMode(ProjectionMode::Orthographic);
		GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.0f, 0.0f, -400.0f));
	}

	{

		state_.CreateState<TitleLevel>("Booting", this, &TitleLevel::StartBooting, &TitleLevel::UpdateBooting, &TitleLevel::EndBooting);
		state_.CreateState<TitleLevel>("Idle", this, &TitleLevel::StartIdle, &TitleLevel::UpdateIdle, &TitleLevel::EndIdle);
		state_.CreateState<TitleLevel>("Select", this, &TitleLevel::StartSelect, &TitleLevel::UpdateSelect, &TitleLevel::EndSelect);

		state_.ChangeState("Booting");
	}

	if (false == GameEngineInput::GetInst().IsKey("PassLevel"))
	{
		GameEngineInput::GetInst().CreateKey("PassLevel", 'P');
	}
}

void TitleLevel::LevelUpdate(float _DeltaTime)
{
	state_.Update(_DeltaTime);

	if (true == GameEngineInput::GetInst().Down("PassLevel"))
	{
		UserGame::LevelChange("LobbyLevel");
	}
}

void TitleLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
	UIController_->GetTitlePrologueBG()->SetAlpha(1.0f);
}

void TitleLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	GameEngineFontManager::GetInst().Load("±¼¸²");
	UI_TestMouse* Mouse = CreateActor<UI_TestMouse>();
	UIController_ = CreateActor<TitleUIController>();

}

void TitleLevel::StartBooting()
{

}

void TitleLevel::UpdateBooting(float _DeltaTime)
{
	fadeWaitTime_ -= GameEngineTime::GetInst().GetDeltaTime();
	mandatoryFadeTime_ += GameEngineTime::GetInst().GetDeltaTime();

	if (0.0f >= fadeWaitTime_)
	{
		fadeTime_ -= GameEngineTime::GetInst().GetDeltaTime();

		UIController_->GetTitlePrologueBG()->SetAlpha(fadeTime_ / 1.5f);
	}

	if (DEFAULT_PROLOGUE_FADETIME <= mandatoryFadeTime_)
	{
		fadeWaitTime_ = 3.0f;
		fadeTime_ = 1.5f;
		mandatoryFadeTime_ = 0.0f;

		UIController_->GetTitlePrologueBG()->SetAlpha(0.0f);
		state_.ChangeState("Idle");
	}

}

void TitleLevel::EndBooting()
{
}

void TitleLevel::StartIdle()
{
}

void TitleLevel::UpdateIdle(float _DeltaTime)
{
	if ("" != UIController_->GetIDInput()->GetInput())
	{
		isTyped_ = true;
		UIController_->GetStartButton()->SetImageByIndex(1);
	}
	else if (true == isTyped_ && "" == UIController_->GetIDInput()->GetInput())
	{
		isTyped_ = false;
		UIController_->GetStartButton()->SetImageByIndex(0);
	}


	if (true == isTyped_ && true == UIController_->GetStartButton()->MouseCollisionCheck())
	{
		if (true == GameEngineInput::GetInst().Press("LBUTTON"))
		{
			UIController_->GetStartButton()->SetImageByIndex(2);

			state_.ChangeState("Select");

		}
		else
		{
			UIController_->GetStartButton()->SetImageByIndex(1);
		}
	}
}

void TitleLevel::EndIdle()
{
}

void TitleLevel::StartSelect()
{
	PlayerInfoManager::GetInstance()->SetPlayerNickname(UIController_->GetIDInput()->GetInput());
}

void TitleLevel::UpdateSelect(float _DeltaTime)
{
	if (true == GameEngineInput::GetInst().Up("LBUTTON"))
	{
		UIController_->GetStartButton()->SetImageByIndex(1);
	}
}

void TitleLevel::EndSelect()
{
}
