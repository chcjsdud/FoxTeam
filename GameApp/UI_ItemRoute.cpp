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
	BackGroundPos = { 0.0f, 0.0f, 0.0f };
	BasicSlotPos = { -100.0f, 0.0f, -1.0f };
	SlotXPivot = { 47.0f, 0.0f, 0.0f };
	SlotSize = { 44.0f,27.0f };
	
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
		Slot5Collision->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 5.0f));
		Slot5Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot5Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);
	}

	{
		RouteRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		RouteRenderer->SetImage("IconBG_UI.png", "PointSmp");
		RouteRenderer->GetTransform()->SetLocalPosition(BackGroundPos);
		RouteRenderer->GetTransform()->SetLocalScaling(RouteRenderer->GetCurrentTexture()->GetTextureSize());
	}


	{
		Slot0_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot0_IconRenderer->SetImage("Inventory_BackGround.png", "PointSmp");
		Slot0_IconRenderer->GetTransform()->SetLocalPosition(BasicSlotPos);

		Slot1_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot1_IconRenderer->SetImage("Inventory_BackGround.png", "PointSmp");
		Slot1_IconRenderer->GetTransform()->SetLocalPosition(BasicSlotPos + SlotXPivot);

		Slot2_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot2_IconRenderer->SetImage("Inventory_BackGround.png", "PointSmp");
		Slot2_IconRenderer->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 2.0f));

		Slot3_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot3_IconRenderer->SetImage("Inventory_BackGround.png", "PointSmp");
		Slot3_IconRenderer->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 3.0f));

		Slot4_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot4_IconRenderer->SetImage("Inventory_BackGround.png", "PointSmp");
		Slot4_IconRenderer->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 4.0f));

		Slot5_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		Slot5_IconRenderer->SetImage("Inventory_BackGround.png", "PointSmp");
		Slot5_IconRenderer->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 5.0f));
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
			RouteRenderer->Off();
			Slot0_IconRenderer->Off();
			Slot1_IconRenderer->Off();
			Slot2_IconRenderer->Off();
			Slot3_IconRenderer->Off();
			Slot4_IconRenderer->Off();
			Slot5_IconRenderer->Off();
		}
		else
		{
			RouteRenderer->On();
			Slot0_IconRenderer->On();
			Slot1_IconRenderer->On();
			Slot2_IconRenderer->On();
			Slot3_IconRenderer->On();
			Slot4_IconRenderer->On();
			Slot5_IconRenderer->On();
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

	//충돌중이 아님
	return -1;

}

void UI_ItemRoute::SetJobType(JobType _myjob)
{
	switch (_myjob)
	{
	case JobType::NONE:
		break;
	case JobType::YUKI:
	{
	}
	break;
	case JobType::HYUNWOO:
	{
	}
	break;
	case JobType::JACKIE:
	{
	}
	break;
	case JobType::RIO:
	{

	}
	break;
	case JobType::AYA:
	{

	}
	break;
	case JobType::DUMMY:
		break;
	case JobType::MAX:
		break;
	default:
		break;
	}
}
