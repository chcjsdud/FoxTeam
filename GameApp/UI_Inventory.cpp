#include "PreCompile.h"
#include "UI_Inventory.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"
#include "ItemBase.h"
#include "UI_ItemSettingHelper.h"

UI_Inventory::UI_Inventory()
	: Time(1.0f), UIOn(false),
	Slot0_Item(nullptr), Slot1_Item(nullptr), Slot2_Item(nullptr), Slot3_Item(nullptr),
	Slot4_Item(nullptr), Slot5_Item(nullptr), Slot6_Item(nullptr), Slot7_Item(nullptr),
	Slot8_Item(nullptr), Slot9_Item(nullptr)
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


	BackGroundPos = { 248.0f, -315.0f, 0.0f };
	BasicSlotPos = { 142.0f, -309.0f, 0.0f };
	SlotXPivot = { 47.0f, 0.0f, 0.0f };
	SlotYPivot = { 0.0f, -33.0f, 0.0f };
	SlotSize = { 44.0f,27.0f };
	
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
		Slot0Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::AABBBox3D);

		Slot1Collision = CreateTransformComponent<GameEngineCollision>();
		Slot1Collision->GetTransform()->SetLocalPosition(BasicSlotPos + SlotXPivot);
		Slot1Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot1Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::AABBBox3D);

		Slot2Collision = CreateTransformComponent<GameEngineCollision>();
		Slot2Collision->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 2.0f));
		Slot2Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot2Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::AABBBox3D);

		Slot3Collision = CreateTransformComponent<GameEngineCollision>();
		Slot3Collision->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 3.0f));
		Slot3Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot3Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::AABBBox3D);

		Slot4Collision = CreateTransformComponent<GameEngineCollision>();
		Slot4Collision->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 4.0f));
		Slot4Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot4Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::AABBBox3D);

		Slot5Collision = CreateTransformComponent<GameEngineCollision>();
		Slot5Collision->GetTransform()->SetLocalPosition(BasicSlotPos + SlotYPivot);
		Slot5Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot5Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::AABBBox3D);

		Slot6Collision = CreateTransformComponent<GameEngineCollision>();
		Slot6Collision->GetTransform()->SetLocalPosition(BasicSlotPos + SlotXPivot + SlotYPivot);
		Slot6Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot6Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::AABBBox3D);

		Slot7Collision = CreateTransformComponent<GameEngineCollision>();
		Slot7Collision->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 2.0f) + SlotYPivot);
		Slot7Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot7Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::AABBBox3D);

		Slot8Collision = CreateTransformComponent<GameEngineCollision>();
		Slot8Collision->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 3.0f) + SlotYPivot);
		Slot8Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot8Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::AABBBox3D);

		Slot9Collision = CreateTransformComponent<GameEngineCollision>();
		Slot9Collision->GetTransform()->SetLocalPosition(BasicSlotPos + (SlotXPivot * 4.0f) + SlotYPivot);
		Slot9Collision->GetTransform()->SetLocalScaling(SlotSize);
		Slot9Collision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::AABBBox3D);
	}
	

	{
		Slot0_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot1_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot2_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot3_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot4_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot5_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot6_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot7_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot8_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot9_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	}

	{
		Slot0_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot1_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot2_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot3_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot4_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot5_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot6_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot7_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot8_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot9_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
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
			//GetLevel()->PushDebugRenderUI(Slot0Collision->GetTransform(), CollisionType::AABBBox3D);
			//GetLevel()->PushDebugRenderUI(Slot1Collision->GetTransform(), CollisionType::AABBBox3D);
			//GetLevel()->PushDebugRenderUI(Slot2Collision->GetTransform(), CollisionType::AABBBox3D);
			//GetLevel()->PushDebugRenderUI(Slot3Collision->GetTransform(), CollisionType::AABBBox3D);
			//GetLevel()->PushDebugRenderUI(Slot4Collision->GetTransform(), CollisionType::AABBBox3D);
			//GetLevel()->PushDebugRenderUI(Slot5Collision->GetTransform(), CollisionType::AABBBox3D);
			//GetLevel()->PushDebugRenderUI(Slot6Collision->GetTransform(), CollisionType::AABBBox3D);
			//GetLevel()->PushDebugRenderUI(Slot7Collision->GetTransform(), CollisionType::AABBBox3D);
			//GetLevel()->PushDebugRenderUI(Slot8Collision->GetTransform(), CollisionType::AABBBox3D);
			//GetLevel()->PushDebugRenderUI(Slot9Collision->GetTransform(), CollisionType::AABBBox3D);
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

void UI_Inventory::EmptySlotReturn(ItemBase* _TargetSlot)
{
}

void UI_Inventory::ItemRenderCheck()
{
}

void UI_Inventory::GetInventoryInfo(list<ItemBase*> _ItemList)
{
}

void UI_Inventory::PushItem(ItemBase* _OriginItemBase, int _SlotNumber)
{
}

void UI_Inventory::EmptySlot()
{
}

