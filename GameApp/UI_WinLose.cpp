#include "PreCompile.h"
#include "UI_WinLose.h"

#include "Enums.h"
#include <GameEngine/GameEngineUIRenderer.h>

UI_WinLose::UI_WinLose() // default constructer 디폴트 생성자
	: portraitRenderer_(nullptr), backRenderer_(nullptr), textRenderer_(nullptr), appearTimer_(0.0f), isAllAppeared_(false), isActivated_(false), bloodStainRendererUp_(nullptr), bloodStainRenderer_(nullptr)
{

}

UI_WinLose::~UI_WinLose() // default destructer 디폴트 소멸자
{

}

UI_WinLose::UI_WinLose(UI_WinLose&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void UI_WinLose::SetPortrait(JobType _jobtype, bool _isWin)
{

	switch (_jobtype)
	{
	case JobType::NONE:
		break;
	case JobType::YUKI:
		if (true == _isWin)
		{
			portraitRenderer_->SetImage("Lobby_CharFull_Yuki.png", "PointSmp");
			break;
		}
		portraitRenderer_->SetImage("Death_CharFull_Yuki.png", "PointSmp");
		break;
	case JobType::FIORA:
		break;
	case JobType::ZAHIR:
		break;
	case JobType::NADINE:
		break;
	case JobType::HYUNWOO:
		if (true == _isWin)
		{
			portraitRenderer_->SetImage("Lobby_CharFull_Hyunwoo.png", "PointSmp");
			break;
		}
		portraitRenderer_->SetImage("Death_CharFull_Hyunwoo.png", "PointSmp");
		break;
	case JobType::JACKIE:
		break;
	case JobType::RIO:
		if (true == _isWin)
		{
			portraitRenderer_->SetImage("Lobby_CharFull_Rio.png", "PointSmp");
			break;
		}
		portraitRenderer_->SetImage("Death_CharFull_Rio.png", "PointSmp");
		break;
	case JobType::AYA:
		break;
	case JobType::DUMMY:
		break;
	case JobType::MAX:
		break;
	default:
		break;
	}
}


void UI_WinLose::SetText(const std::string& _text)
{
	textRenderer_->TextSetting("굴림", _text, 20, FW1_CENTER);
}

void UI_WinLose::Start()
{
	portraitRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	portraitRenderer_->SetImage("Death_CharFull_Hyunwoo.png", "PointSmp");
	//	portraitRenderer_->GetTransform()->SetLocalPosition();
	portraitRenderer_->GetTransform()->SetLocalScaling(portraitRenderer_->GetCurrentTexture()->GetTextureSize());
	portraitRenderer_->SetAlpha(0.0f);
	portraitRenderer_->Off();

	backRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	backRenderer_->SetImage("Img_Result_PortraitBg.png", "PointSmp");
	backRenderer_->GetTransform()->SetLocalScaling(backRenderer_->GetCurrentTexture()->GetTextureSize() * 2);
	backRenderer_->SetAlpha(0.0f);
	backRenderer_->Off();

	bloodStainRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	bloodStainRenderer_->GetTransform()->SetLocalPosition({ -550.0f, -250.0f, 0.0f });
	bloodStainRenderer_->SetImage("FX_UI_Bloodstain.png", "PointSmp");
	bloodStainRenderer_->GetTransform()->SetLocalScaling(bloodStainRenderer_->GetCurrentTexture()->GetTextureSize());
	bloodStainRenderer_->SetAlpha(0.0f);
	bloodStainRenderer_->Off();

	bloodStainRendererUp_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	bloodStainRendererUp_->GetTransform()->SetLocalPosition({ 550.0f, 250.0f, 0.0f });
	bloodStainRendererUp_->SetImage("FX_UI_Bloodstain2.png", "PointSmp");
	bloodStainRendererUp_->GetTransform()->SetLocalScaling(bloodStainRenderer_->GetCurrentTexture()->GetTextureSize());
	bloodStainRendererUp_->SetAlpha(0.0f);
	bloodStainRendererUp_->Off();

	winLosePanelRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	winLosePanelRenderer_->SetImage("losePanel.png", "PointSmp");
	winLosePanelRenderer_->GetTransform()->SetLocalScaling(winLosePanelRenderer_->GetCurrentTexture()->GetTextureSize());
	winLosePanelRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 220.0f, 0.0f });
	winLosePanelRenderer_->SetAlpha(0.0f);
	winLosePanelRenderer_->Off();

	winLoseTextRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	winLoseTextRenderer_->SetTextColor(float4::WHITE);
	winLoseTextRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 220.0f, 0.0f });
	winLoseTextRenderer_->TextSetting("굴림", "실험 종료", 40, FW1_CENTER);
	winLoseTextRenderer_->SetAlpha(0.0f);
	winLoseTextRenderer_->Off();


	textRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	textRenderer_->SetImage("rankPanel.png", "PointSmp");
	textRenderer_->GetTransform()->SetLocalScaling(textRenderer_->GetCurrentTexture()->GetTextureSize());
	textRenderer_->GetTransform()->SetLocalPosition({ 0.0f,-220.0f,0.0f });
	textRenderer_->SetAlpha(0.0f);
	textRenderer_->Off();

	textRenderer_ = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	textRenderer_->SetTextColor(float4::WHITE);
	textRenderer_->TextSetting("굴림", " ", 30, FW1_CENTER);
	textRenderer_->GetTransform()->SetLocalPosition({ 0.0f,-220.0f,0.0f });
	textRenderer_->SetAlpha(0.0f);
	textRenderer_->Off();


	UIstate_.CreateState(MakeState(UI_WinLose, NoAppear));
	UIstate_.CreateState(MakeState(UI_WinLose, Appear));

	UIstate_ << "NoAppear";
}

void UI_WinLose::Update(float _DeltaTime)
{
	UIstate_.Update(_DeltaTime);
}

void UI_WinLose::startNoAppear()
{
	appearTimer_ = 0.0f;
	isActivated_ = false;

}

void UI_WinLose::updateNoAppear(float _DeltaTime)
{
	if (true == isActivated_)
	{
		UIstate_.ChangeState("Appear", true);
		return;
	}
}

void UI_WinLose::startAppear()
{
	portraitRenderer_->On();
	backRenderer_->On();
	winLoseTextRenderer_->On();
	textRenderer_->On();
	bloodStainRenderer_->On();
	bloodStainRendererUp_->On();
	winLosePanelRenderer_->On();
	rankPanelRenderer_->On();
}

void UI_WinLose::updateAppear(float _DeltaTime)
{


	appearTimer_ += _DeltaTime;

	float adjustedRatio = appearTimer_ / TIME_APPEAR_DEFAULT;
	if (adjustedRatio >= 1.0f)
	{
		return;
	}

	
	portraitRenderer_->SetAlpha(adjustedRatio);
	backRenderer_->SetAlpha(adjustedRatio);
	winLoseTextRenderer_->SetAlpha(adjustedRatio);
	textRenderer_->SetAlpha(adjustedRatio);
	bloodStainRenderer_->SetAlpha(adjustedRatio);
	bloodStainRendererUp_->SetAlpha(adjustedRatio);
	winLosePanelRenderer_->SetAlpha(adjustedRatio);
	rankPanelRenderer_->SetAlpha(adjustedRatio);
}

