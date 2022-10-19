#include "PreCompile.h"
#include "UI_Time.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"
#include "GameTimeController.h"

UI_Time::UI_Time()
	: Time(1.0f), UIOn(false)
{
}

UI_Time::~UI_Time()
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


void UI_Time::Start()
{

	UI_Pos = { 0.0f, 336.0f, 1.0f };
	Day_Pos = { -40.0f, 329.0f, 0.0f };
	Time_Pos = { -10.0f, 343.0f, 0.0f };
	Clock_Pos = { -30.f, 300.f, 0.0f };
	ClockTime_Pos = { -10.f, 315.f, 0.0f };

	{
		BackGroundRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		BackGroundRenderer->SetImage("UI_TimeBg.png", "PointSmp");
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


}

void UI_Time::Update(float _Time)
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
		}
		else
		{
			BackGroundRenderer->On();
			DayNightRenderer->On();
			TimeRenderer->On();
			NightChangeClockRenderer->On();
			NightChangeTimeRenderer->On();
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

void UI_Time::TimeSetting()
{
	{
		tm InGameTime = GameTimeController::GetInstance()->GetTotalGameTimeToHour();
		int Minute = InGameTime.tm_min;
		int Second = InGameTime.tm_sec;
		string Time = to_string(Minute) + ":" + to_string(Second);
		TimeRenderer->TextSetting("HMKMRHD", Time, 20);
	}

	{
		float NightChangeTime = GameTimeController::GetInstance()->GetRemainTimeToSec();
		tm ClockTime = GameTimeController::GetInstance()->GetRemainTimeToMinute();
		int Minute = ClockTime.tm_min;
		int Second = ClockTime.tm_sec;
	//	string Time = to_string(Minute) + ":" + to_string(Second);
		string Time = to_string(static_cast<int>(NightChangeTime));
		NightChangeTimeRenderer->TextSetting("HMKMRHD", Time, 20);
	}
}

