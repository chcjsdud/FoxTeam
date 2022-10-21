#include "PreCompile.h"
#include "UI_Notice.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"
#include "GameTimeController.h"

UI_Notice::UI_Notice()
	: Time(1.0f), UIOn(false)
{
}

UI_Notice::~UI_Notice()
{
	//for (auto& UIRenderer : UIRendererMap)
	//{
	//	if (true)
	//	{
	//		delete UIRenderer.second;
	//		UIRenderer.second = nullptr;
	//	}
	//}
}


void UI_Notice::Start()
{

	UI_Pos = { 0.0f, 326.0f, 1.0f };
	Day_Pos = { -35.0f, 330.0f, 0.0f };
	Time_Pos = { -10.0f, 344.0f, 0.0f };
	Clock_Pos = { -30.f, 303.f, 0.0f };
	ClockTime_Pos = { -10.f, 318.f, 0.0f };
	DayCount_Pos = { -20.f, 361.f, 0.0f };

	{
		BackGroundRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		BackGroundRenderer->SetImage("UI_TimeBg_Test.png", "PointSmp");
		BackGroundRenderer->GetTransform()->SetLocalPosition(UI_Pos);
		//BackGroundRenderer->GetTransform()->SetLocalScaling(BackGroundRenderer->GetCurrentTexture()->GetTextureSize() * 0.8f);
		BackGroundRenderer->GetTransform()->SetLocalScaling(BackGroundRenderer->GetCurrentTexture()->GetTextureSize());

		DayNightRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		DayNightRenderer->SetImage("UI_Sun.png", "PointSmp");
		DayNightRenderer->GetTransform()->SetLocalPosition(Day_Pos);
		DayNightRenderer->GetTransform()->SetLocalScaling(DayNightRenderer->GetCurrentTexture()->GetTextureSize());

		TimeRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		TimeRenderer->GetTransform()->SetLocalPosition(Time_Pos);
	}

	{
		NightChangeClockRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		NightChangeClockRenderer->SetImage("UI_Clock.png", "PointSmp");
		NightChangeClockRenderer->GetTransform()->SetLocalPosition(Clock_Pos);
		NightChangeClockRenderer->GetTransform()->SetLocalScaling({ 21.f,21.f });

		NightChangeTimeRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		NightChangeTimeRenderer->GetTransform()->SetLocalPosition(ClockTime_Pos);
	}

	{
		DayCountRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		DayCountRenderer->GetTransform()->SetLocalPosition(DayCount_Pos);
	}

}

void UI_Notice::Update(float _Time)
{
	if (0.0f >= (Time -= _Time))
	{
		//TopRenderer->SetAlpha(Time);
	}

	//UI 온오프 체크
	{
		if (false == UIOn)
		{
			BackGroundRenderer->Off();
			DayNightRenderer->Off();
			TimeRenderer->Off();
			NightChangeClockRenderer->Off();
			NightChangeTimeRenderer->Off();
			DayCountRenderer->Off();
		}
		else
		{
			BackGroundRenderer->On();
			DayNightRenderer->On();
			TimeRenderer->On();
			NightChangeClockRenderer->On();
			NightChangeTimeRenderer->On();
			DayCountRenderer->On();
		}
	}

	if (true == GameEngineInput::GetInst().Down("Esc"))
	{
		if (UIOn == true)
		{
			UIOn = false;
		}
		else
		{
			UIOn = true;
		}
	}

	DayAndNightType SunMoon = GameTimeController::GetInstance()->GetCurrentDayType();

	switch (SunMoon)
	{
	case DayAndNightType::NONE:
		DayNightRenderer->SetImage("UI_Sun.png", "PointSmp");
		break;
	case DayAndNightType::DAY:
		DayNightRenderer->SetImage("UI_Sun.png", "PointSmp");
		break;
	case DayAndNightType::NIGHT:
		DayNightRenderer->SetImage("UI_Moon.png", "PointSmp");
		break;
	case DayAndNightType::MAX:
		DayNightRenderer->SetImage("UI_Sun.png", "PointSmp");
		break;
	default:
		break;
	}

	TimeSetting();

}

