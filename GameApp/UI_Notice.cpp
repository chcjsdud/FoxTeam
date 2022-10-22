#include "PreCompile.h"
#include "UI_Notice.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"
#include "GameTimeController.h"

UI_Notice* UI_Notice::Inst = new UI_Notice();

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
	}

	Inst = this;
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
			FontRenderer->Off();
		}
		else
		{
			BackGroundRenderer->On();
			FontRenderer->On();
			FontRenderer->TextSetting("HMKMRHD", "Test", 12);
		}
	}

	//if (true == GameEngineInput::GetInst().Down("Esc"))
	//{
	//	if (UIOn == true)
	//	{
	//		UIOn = false;
	//	}
	//	else
	//	{
	//		UIOn = true;
	//	}
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
