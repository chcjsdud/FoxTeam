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
	BasicSlotPos = { 141.0f, -309.0f, -1.0f };
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
		Slot0_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::BACKDROP);
		Slot1_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::BACKDROP);
		Slot2_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::BACKDROP);
		Slot3_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::BACKDROP);
		Slot4_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::BACKDROP);
		Slot5_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::BACKDROP);
		Slot6_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::BACKDROP);
		Slot7_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::BACKDROP);
		Slot8_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::BACKDROP);
		Slot9_BGRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::BACKDROP);
	}

	{
		Slot0_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIICON);
		Slot1_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIICON);
		Slot2_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIICON);
		Slot3_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIICON);
		Slot4_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIICON);
		Slot5_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIICON);
		Slot6_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIICON);
		Slot7_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIICON);
		Slot8_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIICON);
		Slot9_IconRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIICON);
	}

}

void UI_Inventory::Update(float _Time)
{
	if (0.0f >= (Time -= _Time))
	{
		//TopRenderer->SetAlpha(Time);
	}

	ItemRenderCheck();

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
	if (_TargetSlot == nullptr)
	{
		return;
	}

	if (Slot0_Item == nullptr)
	{
		Slot0_Item = _TargetSlot->Copy();
		return;
	}

	if (Slot1_Item == nullptr)
	{
		Slot1_Item = _TargetSlot->Copy();
		return;
	}

	if (Slot2_Item == nullptr)
	{
		Slot2_Item = _TargetSlot->Copy();
		return;
	}

	if (Slot3_Item == nullptr)
	{
		Slot3_Item = _TargetSlot->Copy();
		return;
	}

	if (Slot4_Item == nullptr)
	{
		Slot4_Item = _TargetSlot->Copy();
		return;
	}

	if (Slot5_Item == nullptr)
	{
		Slot5_Item = _TargetSlot->Copy();
		return;
	}

	if (Slot6_Item == nullptr)
	{
		Slot6_Item = _TargetSlot->Copy();
		return;
	}

	if (Slot7_Item == nullptr)
	{
		Slot7_Item = _TargetSlot->Copy();
		return;
	}

	if (Slot8_Item == nullptr)
	{
		Slot8_Item = _TargetSlot->Copy();
		return;
	}

	if (Slot9_Item == nullptr)
	{
		Slot9_Item = _TargetSlot->Copy();
		return;
	}

	GameEngineDebug::MsgBox("인벤토리에 빈 슬롯이 없습니다");
	return;
}

void UI_Inventory::ItemRenderCheck()
{

	if (Slot0_Item != nullptr)
	{
		Slot0_BGRenderer->GetTransform()->SetLocalPosition(Slot0Collision->GetTransform()->GetLocalPosition());
		Slot0_BGRenderer->GetTransform()->SetLocalScaling(SlotSize);
		UI_ItemSettingHelper::SetItemRenderValue(Slot0_BGRenderer, Slot0_Item);

		Slot0_IconRenderer->SetImage(UI_ItemSettingHelper::GetItemTextureName(Slot0_Item));
		Slot0_IconRenderer->GetTransform()->SetLocalScaling(Slot0_IconRenderer->GetCurrentTexture()->GetTextureSize());
		Slot0_IconRenderer->GetTransform()->SetLocalPosition((Slot0_BGRenderer->GetTransform()->GetLocalPosition() + float4{ 0.0f, 0.0f, -1.0f, 0.0f }));

		Slot0_BGRenderer->On();
		Slot0_IconRenderer->On();
	}
	else
	{
		Slot0_BGRenderer->Off();
		Slot0_IconRenderer->Off();
	}


	if (Slot1_Item != nullptr)
	{
		Slot1_BGRenderer->GetTransform()->SetLocalPosition(Slot1Collision->GetTransform()->GetLocalPosition());
		Slot1_BGRenderer->GetTransform()->SetLocalScaling(SlotSize);
		UI_ItemSettingHelper::SetItemRenderValue(Slot1_BGRenderer, Slot1_Item);

		Slot1_IconRenderer->SetImage(UI_ItemSettingHelper::GetItemTextureName(Slot1_Item));
		Slot1_IconRenderer->GetTransform()->SetLocalScaling(Slot1_IconRenderer->GetCurrentTexture()->GetTextureSize());
		Slot1_IconRenderer->GetTransform()->SetLocalPosition((Slot1_BGRenderer->GetTransform()->GetLocalPosition() + float4{ 0.0f, 0.0f, -1.0f, 0.0f }));

		Slot1_BGRenderer->On();
		Slot1_IconRenderer->On();
	}
	else
	{
		Slot1_BGRenderer->Off();
		Slot1_IconRenderer->Off();
	}


	if (Slot2_Item != nullptr)
	{
		Slot2_BGRenderer->GetTransform()->SetLocalPosition(Slot2Collision->GetTransform()->GetLocalPosition());
		Slot2_BGRenderer->GetTransform()->SetLocalScaling(SlotSize);
		UI_ItemSettingHelper::SetItemRenderValue(Slot2_BGRenderer, Slot2_Item);

		Slot2_IconRenderer->SetImage(UI_ItemSettingHelper::GetItemTextureName(Slot2_Item));
		Slot2_IconRenderer->GetTransform()->SetLocalScaling(Slot2_IconRenderer->GetCurrentTexture()->GetTextureSize());
		Slot2_IconRenderer->GetTransform()->SetLocalPosition((Slot2_BGRenderer->GetTransform()->GetLocalPosition() + float4{ 0.0f, 0.0f, -1.0f, 0.0f }));

		Slot2_BGRenderer->On();
		Slot2_IconRenderer->On();
	}
	else
	{
		Slot2_BGRenderer->Off();
		Slot2_IconRenderer->Off();
	}

	if (Slot3_Item != nullptr)
	{
		Slot3_BGRenderer->GetTransform()->SetLocalPosition(Slot3Collision->GetTransform()->GetLocalPosition());
		Slot3_BGRenderer->GetTransform()->SetLocalScaling(SlotSize);
		UI_ItemSettingHelper::SetItemRenderValue(Slot3_BGRenderer, Slot3_Item);

		Slot3_IconRenderer->SetImage(UI_ItemSettingHelper::GetItemTextureName(Slot3_Item));
		Slot3_IconRenderer->GetTransform()->SetLocalScaling(Slot3_IconRenderer->GetCurrentTexture()->GetTextureSize());
		Slot3_IconRenderer->GetTransform()->SetLocalPosition((Slot3_BGRenderer->GetTransform()->GetLocalPosition() + float4{ 0.0f, 0.0f, -1.0f, 0.0f }));

		Slot3_BGRenderer->On();
		Slot3_IconRenderer->On();
	}
	else
	{
		Slot3_BGRenderer->Off();
		Slot3_IconRenderer->Off();
	}

	if (Slot4_Item != nullptr)
	{
		Slot4_BGRenderer->GetTransform()->SetLocalPosition(Slot4Collision->GetTransform()->GetLocalPosition());
		Slot4_BGRenderer->GetTransform()->SetLocalScaling(SlotSize);
		UI_ItemSettingHelper::SetItemRenderValue(Slot4_BGRenderer, Slot4_Item);

		Slot4_IconRenderer->SetImage(UI_ItemSettingHelper::GetItemTextureName(Slot4_Item));
		Slot4_IconRenderer->GetTransform()->SetLocalScaling(Slot4_IconRenderer->GetCurrentTexture()->GetTextureSize());
		Slot4_IconRenderer->GetTransform()->SetLocalPosition((Slot4_BGRenderer->GetTransform()->GetLocalPosition() + float4{ 0.0f, 0.0f, -1.0f, 0.0f }));

		Slot4_BGRenderer->On();
		Slot4_IconRenderer->On();
	}
	else
	{
		Slot4_BGRenderer->Off();
		Slot4_IconRenderer->Off();
	}

	if (Slot5_Item != nullptr)
	{
		Slot5_BGRenderer->GetTransform()->SetLocalPosition(Slot5Collision->GetTransform()->GetLocalPosition());
		Slot5_BGRenderer->GetTransform()->SetLocalScaling(SlotSize);
		UI_ItemSettingHelper::SetItemRenderValue(Slot5_BGRenderer, Slot5_Item);

		Slot5_IconRenderer->SetImage(UI_ItemSettingHelper::GetItemTextureName(Slot5_Item));
		Slot5_IconRenderer->GetTransform()->SetLocalScaling(Slot5_IconRenderer->GetCurrentTexture()->GetTextureSize());
		Slot5_IconRenderer->GetTransform()->SetLocalPosition((Slot5_BGRenderer->GetTransform()->GetLocalPosition() + float4{ 0.0f, 0.0f, -1.0f, 0.0f }));

		Slot5_BGRenderer->On();
		Slot5_IconRenderer->On();
	}
	else
	{
		Slot5_BGRenderer->Off();
		Slot5_IconRenderer->Off();
	}

	if (Slot6_Item != nullptr)
	{
		Slot6_BGRenderer->GetTransform()->SetLocalPosition(Slot6Collision->GetTransform()->GetLocalPosition());
		Slot6_BGRenderer->GetTransform()->SetLocalScaling(SlotSize);
		UI_ItemSettingHelper::SetItemRenderValue(Slot6_BGRenderer, Slot6_Item);

		Slot6_IconRenderer->SetImage(UI_ItemSettingHelper::GetItemTextureName(Slot6_Item));
		Slot6_IconRenderer->GetTransform()->SetLocalScaling(Slot6_IconRenderer->GetCurrentTexture()->GetTextureSize());
		Slot6_IconRenderer->GetTransform()->SetLocalPosition((Slot6_BGRenderer->GetTransform()->GetLocalPosition() + float4{ 0.0f, 0.0f, -1.0f, 0.0f }));

		Slot6_BGRenderer->On();
		Slot6_IconRenderer->On();
	}
	else
	{
		Slot6_BGRenderer->Off();
		Slot6_IconRenderer->Off();
	}

	if (Slot7_Item != nullptr)
	{
		Slot7_BGRenderer->GetTransform()->SetLocalPosition(Slot7Collision->GetTransform()->GetLocalPosition());
		Slot7_BGRenderer->GetTransform()->SetLocalScaling(SlotSize);
		UI_ItemSettingHelper::SetItemRenderValue(Slot7_BGRenderer, Slot7_Item);

		Slot7_IconRenderer->SetImage(UI_ItemSettingHelper::GetItemTextureName(Slot7_Item));
		Slot7_IconRenderer->GetTransform()->SetLocalScaling(Slot7_IconRenderer->GetCurrentTexture()->GetTextureSize());
		Slot7_IconRenderer->GetTransform()->SetLocalPosition((Slot7_BGRenderer->GetTransform()->GetLocalPosition() + float4{ 0.0f, 0.0f, -1.0f, 0.0f }));

		Slot7_BGRenderer->On();
		Slot7_IconRenderer->On();
	}
	else
	{
		Slot7_BGRenderer->Off();
		Slot7_IconRenderer->Off();
	}

	if (Slot8_Item != nullptr)
	{
		Slot8_BGRenderer->GetTransform()->SetLocalPosition(Slot8Collision->GetTransform()->GetLocalPosition());
		Slot8_BGRenderer->GetTransform()->SetLocalScaling(SlotSize);
		UI_ItemSettingHelper::SetItemRenderValue(Slot8_BGRenderer, Slot8_Item);

		Slot8_IconRenderer->SetImage(UI_ItemSettingHelper::GetItemTextureName(Slot8_Item));
		Slot8_IconRenderer->GetTransform()->SetLocalScaling(Slot8_IconRenderer->GetCurrentTexture()->GetTextureSize());
		Slot8_IconRenderer->GetTransform()->SetLocalPosition((Slot8_BGRenderer->GetTransform()->GetLocalPosition() + float4{ 0.0f, 0.0f, -1.0f, 0.0f }));

		Slot8_BGRenderer->On();
		Slot8_IconRenderer->On();
	}
	else
	{
		Slot8_BGRenderer->Off();
		Slot8_IconRenderer->Off();
	}


	if (Slot9_Item != nullptr)
	{
		Slot9_BGRenderer->GetTransform()->SetLocalPosition(Slot9Collision->GetTransform()->GetLocalPosition());
		Slot9_BGRenderer->GetTransform()->SetLocalScaling(SlotSize);
		UI_ItemSettingHelper::SetItemRenderValue(Slot9_BGRenderer, Slot9_Item);

		Slot9_IconRenderer->SetImage(UI_ItemSettingHelper::GetItemTextureName(Slot9_Item));
		Slot9_IconRenderer->GetTransform()->SetLocalScaling(Slot9_IconRenderer->GetCurrentTexture()->GetTextureSize());
		Slot9_IconRenderer->GetTransform()->SetLocalPosition((Slot9_BGRenderer->GetTransform()->GetLocalPosition() + float4{ 0.0f, 0.0f, -1.0f, 0.0f }));

		Slot9_BGRenderer->On();
		Slot9_IconRenderer->On();
	}
	else
	{
		Slot9_BGRenderer->Off();
		Slot9_IconRenderer->Off();
	}
}

void UI_Inventory::GetInventoryInfo(vector<ItemBase*> _ItemVector)
{
	vector<ItemBase*>::iterator listiter = _ItemVector.begin();

	for (listiter = _ItemVector.begin(); listiter != _ItemVector.end(); listiter++)
	{
		PushItem(*listiter);
	}

	return;
}

void UI_Inventory::PushItem(ItemBase* _OriginItemBase, int _SlotNumber)
{
	switch (_SlotNumber)
	{
	case 0:
	{
		EmptySlotReturn(_OriginItemBase);
		return;
	}
	break;
	case 1:
	{
		if (Slot1_Item == nullptr)
		{
			Slot1_Item = _OriginItemBase->Copy();
			return;
		}
		else
		{
			GameEngineDebug::MsgBoxError("이미 세팅된 슬롯에 아이템을 세팅하려 했습니다");
			return;
		}
	}
	break;
	case 2:
	{
		if (Slot2_Item == nullptr)
		{
			Slot2_Item = _OriginItemBase->Copy();
			return;
		}
		else
		{
			GameEngineDebug::MsgBoxError("이미 세팅된 슬롯에 아이템을 세팅하려 했습니다");
			return;
		}
	}
	break;
	case 3:
	{
		if (Slot3_Item == nullptr)
		{
			Slot3_Item = _OriginItemBase->Copy();
			return;
		}
		else
		{
			GameEngineDebug::MsgBoxError("이미 세팅된 슬롯에 아이템을 세팅하려 했습니다");
			return;
		}
	}
	break;
	case 4:
	{
		if (Slot4_Item == nullptr)
		{
			Slot4_Item = _OriginItemBase->Copy();
			return;
		}
		else
		{
			GameEngineDebug::MsgBoxError("이미 세팅된 슬롯에 아이템을 세팅하려 했습니다");
			return;
		}
	}
	break;
	case 5:
	{
		if (Slot5_Item == nullptr)
		{
			Slot5_Item = _OriginItemBase->Copy();
			return;
		}
		else
		{
			GameEngineDebug::MsgBoxError("이미 세팅된 슬롯에 아이템을 세팅하려 했습니다");
			return;
		}
	}
	break;
	case 6:
	{
		if (Slot6_Item == nullptr)
		{
			Slot6_Item = _OriginItemBase->Copy();
			return;
		}
		else
		{
			GameEngineDebug::MsgBoxError("이미 세팅된 슬롯에 아이템을 세팅하려 했습니다");
			return;
		}
	}
	break;
	case 7:
	{
		if (Slot7_Item == nullptr)
		{
			Slot7_Item = _OriginItemBase->Copy();
			return;
		}
		else
		{
			GameEngineDebug::MsgBoxError("이미 세팅된 슬롯에 아이템을 세팅하려 했습니다");
			return;
		}
	}
	break;
	case 8:
	{
		if (Slot8_Item == nullptr)
		{
			Slot8_Item = _OriginItemBase->Copy();
			return;
		}
		else
		{
			GameEngineDebug::MsgBoxError("이미 세팅된 슬롯에 아이템을 세팅하려 했습니다");
			return;
		}
	}
	break;
	case 9:
	{
		if (Slot9_Item == nullptr)
		{
			Slot9_Item = _OriginItemBase->Copy();
			return;
		}
		else
		{
			GameEngineDebug::MsgBoxError("이미 세팅된 슬롯에 아이템을 세팅하려 했습니다");
			return;
		}
	}
	break;
	default:
	{
		GameEngineDebug::MsgBoxError("슬롯번호가 제대로 세팅되지 않았습니다");
		return;
	}
	break;
	}
}

void UI_Inventory::EmptySlot()
{
	//아이템정보를 비워주는 함수

	if (Slot0_Item != nullptr)
	{
		Slot0_Item = nullptr;
	}

	if (Slot1_Item != nullptr)
	{
		Slot1_Item = nullptr;
	}

	if (Slot2_Item != nullptr)
	{
		Slot2_Item = nullptr;
	}

	if (Slot3_Item != nullptr)
	{
		Slot3_Item = nullptr;
	}

	if (Slot4_Item != nullptr)
	{
		Slot4_Item = nullptr;
	}

	if (Slot5_Item != nullptr)
	{
		Slot5_Item = nullptr;
	}

	if (Slot6_Item != nullptr)
	{
		Slot6_Item = nullptr;
	}

	if (Slot7_Item != nullptr)
	{
		Slot7_Item = nullptr;
	}

	if (Slot8_Item != nullptr)
	{
		Slot8_Item = nullptr;
	}

	if (Slot9_Item != nullptr)
	{
		Slot9_Item = nullptr;
	}
}

