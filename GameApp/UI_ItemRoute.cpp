#include "PreCompile.h"
#include "UI_ItemRoute.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"
#include "ItemBase.h"
#include "UI_ItemSettingHelper.h"

UI_ItemRoute::UI_ItemRoute()
	: Time(1.0f), UIOn(true)
{
}

UI_ItemRoute::~UI_ItemRoute()
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

void UI_ItemRoute::Start()
{
	BackGroundPos = { 248.0f, -319.0f, 0.0f };
	BasicSlotPos = { 141.0f, -313.0f, -1.0f };
	SlotXPivot = { 47.0f, 0.0f, 0.0f };
	SlotYPivot = { 0.0f, -32.0f, 0.0f };
	SlotSize = { 44.0f,27.0f };

	float4 createSlotSize = { 26.0f,58.0f, 1.f };
	float4 createSlotPos = { 370.0f, -328.0f, 0.0f };
	
	{
		Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Renderer->SetImage("Inventory_BackGround.png", "PointSmp");
		Renderer->GetTransform()->SetLocalPosition(BackGroundPos);
		Renderer->GetTransform()->SetLocalScaling(Renderer->GetCurrentTexture()->GetTextureSize());
	}

	{
		//마우스와의 충돌을 위해서 aabbbox3d로 충돌

		Slot0Collision = CreateTransformComponent<GameEngineCollision>();
		Slot0Collision->GetTransform()->SetLocalPosition(BasicSlotPos);
		Slot0Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot0Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);

		Slot1Collision = CreateTransformComponent<GameEngineCollision>();
		Slot1Collision->GetTransform()->SetLocalPosition(BasicSlotPos + SlotXPivot);
		Slot1Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot1Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);

		Slot2Collision = CreateTransformComponent<GameEngineCollision>();
		Slot2Collision->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 2.0f));
		Slot2Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot2Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);

		Slot3Collision = CreateTransformComponent<GameEngineCollision>();
		Slot3Collision->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 3.0f));
		Slot3Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot3Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);

		Slot4Collision = CreateTransformComponent<GameEngineCollision>();
		Slot4Collision->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 4.0f));
		Slot4Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot4Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);

		Slot5Collision = CreateTransformComponent<GameEngineCollision>();
		Slot5Collision->GetTransform()->SetLocalPosition(BasicSlotPos + SlotYPivot);
		Slot5Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot5Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);

		Slot6Collision = CreateTransformComponent<GameEngineCollision>();
		Slot6Collision->GetTransform()->SetLocalPosition(BasicSlotPos + SlotXPivot + SlotYPivot);
		Slot6Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot6Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);

		Slot7Collision = CreateTransformComponent<GameEngineCollision>();
		Slot7Collision->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 2.0f) + SlotYPivot);
		Slot7Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot7Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);

		Slot8Collision = CreateTransformComponent<GameEngineCollision>();
		Slot8Collision->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 3.0f) + SlotYPivot);
		Slot8Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot8Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);

		Slot9Collision = CreateTransformComponent<GameEngineCollision>();
		Slot9Collision->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 4.0f) + SlotYPivot);
		Slot9Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot9Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);
	}
	

	{
		Slot0_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		Slot1_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		Slot2_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		Slot3_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		Slot4_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		Slot5_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		Slot6_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		Slot7_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		Slot8_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		Slot9_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
	}

	{
		Slot0_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot1_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot2_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot3_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot4_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot5_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot6_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot7_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot8_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot9_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
	}

}

void UI_ItemRoute::Update(float _Time)
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

int UI_ItemRoute::SlotMouseCollisionCheck()
{

	if (true == Slot0Collision->Collision(eCollisionGroup::MousePointer))
	{
		return 0;
	}

	if (true == Slot1Collision->Collision(eCollisionGroup::MousePointer))
	{
		return 1;
	}

	if (true == Slot2Collision->Collision(eCollisionGroup::MousePointer))
	{
		return 2;
	}

	if (true == Slot3Collision->Collision(eCollisionGroup::MousePointer))
	{
		return 3;
	}

	if (true == Slot4Collision->Collision(eCollisionGroup::MousePointer))
	{
		return 4;
	}

	if (true == Slot5Collision->Collision(eCollisionGroup::MousePointer))
	{
		return 5;
	}

	if (true == Slot6Collision->Collision(eCollisionGroup::MousePointer))
	{
		return 6;
	}

	if (true == Slot7Collision->Collision(eCollisionGroup::MousePointer))
	{
		return 7;
	}

	if (true == Slot8Collision->Collision(eCollisionGroup::MousePointer))
	{
		return 8;
	}

	if (true == Slot9Collision->Collision(eCollisionGroup::MousePointer))
	{
		return 9;
	}


	//충돌중이 아님
	return -1;

}

