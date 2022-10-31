#include "PreCompile.h"
#include "UI_HPBars.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"

UI_HPBars::UI_HPBars()
	: Time(1.0f), UIOn(true)
{
}

UI_HPBars::~UI_HPBars()
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


void UI_HPBars::Start()
{
	//리소스들을 SetImage하고 위치를 조정합니다
	//위치를 조정한 리소스들은 UIRenderMap에 String과 같이 insert됩니다.
	//인게임정보(HP나 스태미너 상태, 착용장비여부)등을 받으면, UIRendererMap에서 이미지이름으로 Find해서 리소스를 바꿀 예정업니다.

	HpBarPos = { 0.0f, 0.0f, -1.0f };
	HpBarSize = {246.0f, 11.0f};

	SpBarPos = { 0.0f, 0.0f, -1.0f };
	SpBarSize = { 246.f, 8.f };

		//z값을 이용해 앞에오는 이미지/뒤에오는 이미지 순서를 정하고 있습니다.
		//위치정보가 될 float도 양이 늘어나면 map이나 vector로 관리할 예정입니다.

	{
		HPBar_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIICON);
		HPBar_Renderer->SetImage("HPBar_UI.png", "PointSmp");
		HPBar_Renderer->GetTransform()->SetLocalPosition(HpBarPos);
		HPBar_Renderer->GetTransform()->SetLocalScaling(HpBarSize);
	}

	{
		SPBar_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIICON);
		SPBar_Renderer->SetImage("SPBar_UI.png", "PointSmp");
		//배경보다 항상 앞에 나옴
		SPBar_Renderer->GetTransform()->SetLocalPosition(SpBarPos);
		SPBar_Renderer->GetTransform()->SetLocalScaling(SpBarSize);
	}

}

void UI_HPBars::Update(float _Time)
{
	
	//UI 온오프 체크
	{
		if (false == UIOn)
		{
			HPBar_Renderer->On();
			SPBar_Renderer->On();
		}
		else
		{
			HPBar_Renderer->Off();
			SPBar_Renderer->Off();
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

