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

	float4 UI_Pos = { 0.0f, 336.0f, 1.0f };

	{
		BackGroundRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		BackGroundRenderer->SetImage("Time_UI_Bg.png", "PointSmp");
		BackGroundRenderer->GetTransform()->SetLocalPosition(UI_Pos);
		BackGroundRenderer->GetTransform()->SetLocalScaling(BackGroundRenderer->GetCurrentTexture()->GetTextureSize() * 0.8f);
	}

	GameTimeController::GetInstance()->GetCurrentDayType();

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
		}
		else
		{
			BackGroundRenderer->On();
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
}

