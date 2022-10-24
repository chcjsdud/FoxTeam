#include "PreCompile.h"
#include "UI_Notice.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"
#include "GameTimeController.h"



UI_Notice::UI_Notice()
	: FadeWaitTime(0.0f), FadeTime(0.0f), UIOn(false)
{
}

UI_Notice::~UI_Notice()
{
}


void UI_Notice::Start()
{

	UI_Pos = { 0.0f, 170.0f, 0.0f };
	Font_Pos = { 0.0f, 170.0f, -1.0f };
	BasicAlpha = 0.4f;

	{
		BackGroundRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		BackGroundRenderer->SetImage("UI_HalfAlphaRect.png", "PointSmp");
		BackGroundRenderer->GetTransform()->SetLocalPosition(UI_Pos);
		BackGroundRenderer->GetTransform()->SetLocalScaling(float4{ 620.0f, 30.0f });
		BackGroundRenderer->SetAlpha(BasicAlpha);
	}

	{
		//폰트출력용
		FontRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		FontRenderer->GetTransform()->SetLocalPosition(UI_Pos);
		FontRenderer->TextSetting("HMKMRHD", "Test", 12, FW1_CENTER);
	}

	BackGroundRenderer->Off();
	FontRenderer->Off();
}

void UI_Notice::Update(float _Time)
{
	
	TimeCount(_Time);

	//UI 온오프 체크
	{
		if (false == UIOn)
		{
			BackGroundRenderer->Off();
			FontRenderer->Off();
			//BackGroundRenderer->On();
			//FontRenderer->On();
		}
		else
		{
			BackGroundRenderer->On();
			FontRenderer->On();
			FontRenderer->TextSetting("HMKMRHD", "Test", 12, FW1_CENTER);
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

	//if (FadeWaitTime >= 0.0f)
	//{
	//	BackGroundRenderer->SetAlpha(Time);
	//}
	//else
	//{
	//	BackGroundRenderer->Off();
	//}

}

void UI_Notice::UISwitch()
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

void UI_Notice::SetText(string _Text, float _Time)
{
	FadeWaitTime = (_Time - 2.0f);

	BackGroundRenderer->On();
	FontRenderer->On();


	FontRenderer->TextSetting("HMKMRHD", _Text, 12, FW1_CENTER);
}

void UI_Notice::TimeCount(float _Time)
{

	if (FadeWaitTime > 0.0f)
	{
		FadeWaitTime -= _Time;
	}
	else if (FadeWaitTime <= 0.0f)
	{
		if (FadeTime <= 0.0f)
		{
			FadeTime = 0.0f;
		}
		else
		{
			//FadeTime은 2초
			FadeTime -= (_Time * 0.5f);
		}
	}

	
}
