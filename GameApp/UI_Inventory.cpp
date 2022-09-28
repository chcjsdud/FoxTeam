#include "PreCompile.h"
#include "UI_Inventory.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"

UI_Inventory::UI_Inventory()
	: Time(1.0f), UIOn(false)
{
}

UI_Inventory::~UI_Inventory()
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

void UI_Inventory::Start()
{

	{
		BackGroundPos = { 248.0f, -315.0f, 0.0f };
		BasicSlotPos = { -77.0f, -93.0f, 0.0f };
		SlotXPivot = { 52.0f, 0.0f, 0.0f };
		SlotYPivot = { 0.0f, -37.0f, 0.0f };
		SlotSize = { 44.0f,27.0f };
		
		{
			Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
			Renderer->SetImage("Inventory_BackGround.png", "PointSmp");
			Renderer->GetTransform()->SetLocalPosition(BackGroundPos);
			Renderer->GetTransform()->SetLocalScaling(Renderer->GetCurrentTexture()->GetTextureSize());
		}

	}

}

void UI_Inventory::Update(float _Time)
{
	if (0.0f >= (Time -= _Time))
	{
		//TopRenderer->SetAlpha(Time);
	}
	
	//UI 온오프 체크
	{
		if (false == UIOn)
		{
			Renderer->Off();
		}
		else
		{

			Renderer->On();
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

