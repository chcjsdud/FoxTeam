#include "PreCompile.h"
#include "UI_ItemBox.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"

#include "ItemBase.h"
#include "ItemBox.h"


UI_ItemBox::UI_ItemBox()
	: Time(1.0f), UI_On(false),
	Slot0_Item(nullptr), Slot1_Item(nullptr), Slot2_Item(nullptr), Slot3_Item(nullptr),
	Slot4_Item(nullptr), Slot5_Item(nullptr), Slot6_Item(nullptr), Slot7_Item(nullptr)
{
}

UI_ItemBox::~UI_ItemBox()
{
	//for (auto& ItemSlot : SlotList_)
	//{
	//	if (true)
	//	{
	//		delete ItemSlot;
	//		ItemSlot = nullptr;
	//	}
	//}
}

void UI_ItemBox::Start()
{
	//리소스들을 SetImage하고 위치를 조정합니다
	//위치를 조정한 리소스들은 UIRenderMap에 String과 같이 insert됩니다.
	//인게임정보(HP나 스태미너 상태, 착용장비여부)등을 받으면, UIRendererMap에서 이미지이름으로 Find해서 리소스를 바꿀 예정업니다.

	ItemBox_BackgroundPos = { 0.0f, -100.0f, 0.0f };
	ItemBox_BasicSlotPos = { -77.0f, -93.0f, 0.0f };
	ItemBox_SlotXPivot = { 52.0f, 0.0f, 0.0f };
	ItemBox_SlotYPivot = { 0.0f, -37.0f, 0.0f };
	ItemBox_SlotSize = { 44.0f,27.0f };

		//z값을 이용해 앞에오는 이미지/뒤에오는 이미지 순서를 정하고 있습니다.
		//위치정보가 될 float도 양이 늘어나면 map이나 vector로 관리할 예정입니다.
	{
		ItemBoxBackGround_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		ItemBoxBackGround_Renderer->SetImage("ItemBox_BackGround.png", "PointSmp");
		ItemBoxBackGround_Renderer->GetTransform()->SetLocalPosition(ItemBox_BackgroundPos);
		ItemBoxBackGround_Renderer->GetTransform()->SetLocalScaling(ItemBoxBackGround_Renderer->GetCurrentTexture()->GetTextureSize());
	}

	{
		BoxtypeFont_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	//	BoxtypeFont_Renderer->TextSetting("굴림", "상자", 30, float4::WHITE);
	//	BoxtypeFont_Renderer->GetTransform()->SetLocalPosition(ItemBox_BackgroundPos);
	}


	{
		//마우스와의 충돌을 위해서 aabbbox3d로 충돌
		ItemBoxCollision = CreateTransformComponent<GameEngineCollision>();
		ItemBoxCollision->GetTransform()->SetLocalPosition(ItemBox_BackgroundPos);
		ItemBoxCollision->GetTransform()->SetLocalScaling(ItemBoxBackGround_Renderer->GetCurrentTexture()->GetTextureSize());
		ItemBoxCollision->SetCollisionInfo(static_cast<int>(CollisionGroup::UI), CollisionType::AABBBox3D);
	}

	{
		//마우스와의 충돌을 위해서 aabbbox3d로 충돌

		Slot0Collision = CreateTransformComponent<GameEngineCollision>();
		Slot0Collision->GetTransform()->SetLocalPosition(ItemBox_BasicSlotPos);
		Slot0Collision->GetTransform()->SetLocalScaling(ItemBox_SlotSize);
		Slot0Collision->SetCollisionInfo(static_cast<int>(CollisionGroup::UI), CollisionType::AABBBox3D);

		Slot1Collision = CreateTransformComponent<GameEngineCollision>();
		Slot1Collision->GetTransform()->SetLocalPosition(ItemBox_BasicSlotPos + ItemBox_SlotXPivot);
		Slot1Collision->GetTransform()->SetLocalScaling(ItemBox_SlotSize);
		Slot1Collision->SetCollisionInfo(static_cast<int>(CollisionGroup::UI), CollisionType::AABBBox3D);

		Slot2Collision = CreateTransformComponent<GameEngineCollision>();
		Slot2Collision->GetTransform()->SetLocalPosition(ItemBox_BasicSlotPos + (ItemBox_SlotXPivot * 2.0f));
		Slot2Collision->GetTransform()->SetLocalScaling(ItemBox_SlotSize);
		Slot2Collision->SetCollisionInfo(static_cast<int>(CollisionGroup::UI), CollisionType::AABBBox3D);

		Slot3Collision = CreateTransformComponent<GameEngineCollision>();
		Slot3Collision->GetTransform()->SetLocalPosition(ItemBox_BasicSlotPos + (ItemBox_SlotXPivot * 3.0f));
		Slot3Collision->GetTransform()->SetLocalScaling(ItemBox_SlotSize);
		Slot3Collision->SetCollisionInfo(static_cast<int>(CollisionGroup::UI), CollisionType::AABBBox3D);

		Slot4Collision = CreateTransformComponent<GameEngineCollision>();
		Slot4Collision->GetTransform()->SetLocalPosition(ItemBox_BasicSlotPos + ItemBox_SlotYPivot);
		Slot4Collision->GetTransform()->SetLocalScaling(ItemBox_SlotSize);
		Slot4Collision->SetCollisionInfo(static_cast<int>(CollisionGroup::UI), CollisionType::AABBBox3D);

		Slot5Collision = CreateTransformComponent<GameEngineCollision>();
		Slot5Collision->GetTransform()->SetLocalPosition(ItemBox_BasicSlotPos + ItemBox_SlotXPivot + ItemBox_SlotYPivot);
		Slot5Collision->GetTransform()->SetLocalScaling(ItemBox_SlotSize);
		Slot5Collision->SetCollisionInfo(static_cast<int>(CollisionGroup::UI), CollisionType::AABBBox3D);

		Slot6Collision = CreateTransformComponent<GameEngineCollision>();
		Slot6Collision->GetTransform()->SetLocalPosition(ItemBox_BasicSlotPos + (ItemBox_SlotXPivot * 2.0f) + ItemBox_SlotYPivot);
		Slot6Collision->GetTransform()->SetLocalScaling(ItemBox_SlotSize);
		Slot6Collision->SetCollisionInfo(static_cast<int>(CollisionGroup::UI), CollisionType::AABBBox3D);

		Slot7Collision = CreateTransformComponent<GameEngineCollision>();
		Slot7Collision->GetTransform()->SetLocalPosition(ItemBox_BasicSlotPos + (ItemBox_SlotXPivot * 3.0f) + ItemBox_SlotYPivot);
		Slot7Collision->GetTransform()->SetLocalScaling(ItemBox_SlotSize);
		Slot7Collision->SetCollisionInfo(static_cast<int>(CollisionGroup::UI), CollisionType::AABBBox3D);
	}

	{
		Slot0_ItemRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot1_ItemRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot2_ItemRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot3_ItemRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot4_ItemRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot5_ItemRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot6_ItemRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Slot7_ItemRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	}

	//SlotList_.push_back(Slot0_Item);
	//SlotList_.push_back(Slot1_Item);
	//SlotList_.push_back(Slot2_Item);
	//SlotList_.push_back(Slot3_Item);
	//SlotList_.push_back(Slot4_Item);
	//SlotList_.push_back(Slot5_Item);
	//SlotList_.push_back(Slot6_Item);
	//SlotList_.push_back(Slot7_Item);
	


}

void UI_ItemBox::Update(float _Time)
{
	if (0.0f >= (Time -= _Time))
	{
		//TopRenderer->SetAlpha(Time);
	}

	ItemRenderCheck();

	//UI 온오프 체크
	{
		if (false == UI_On)
		{
			ItemBoxBackGround_Renderer->Off();
			BoxtypeFont_Renderer->Off();
		}
		else
		{
			ItemBoxBackGround_Renderer->On();
			BoxtypeFont_Renderer->On();
		//	GetLevel()->PushDebugRenderUI(Slot0Collision->GetTransform(), CollisionType::AABBBox3D);
		//	GetLevel()->PushDebugRenderUI(Slot1Collision->GetTransform(), CollisionType::AABBBox3D);
		//	GetLevel()->PushDebugRenderUI(Slot2Collision->GetTransform(), CollisionType::AABBBox3D);
		//	GetLevel()->PushDebugRenderUI(Slot3Collision->GetTransform(), CollisionType::AABBBox3D);
		//	GetLevel()->PushDebugRenderUI(Slot4Collision->GetTransform(), CollisionType::AABBBox3D);
		//	GetLevel()->PushDebugRenderUI(Slot5Collision->GetTransform(), CollisionType::AABBBox3D);
		//	GetLevel()->PushDebugRenderUI(Slot6Collision->GetTransform(), CollisionType::AABBBox3D);
		//	GetLevel()->PushDebugRenderUI(Slot7Collision->GetTransform(), CollisionType::AABBBox3D);
		}
	}

	if (true == GameEngineInput::GetInst().Down("Esc"))
	{
		if (UI_On == true)
		{
			RenderOff();
		}
		else
		{
			RenderOn();
		}
	}

}

void UI_ItemBox::RenderOff()
{
	UI_On = false;
}

void UI_ItemBox::RenderOn()
{
	UI_On = true;
}

void UI_ItemBox::GetItemBoxInfo(list<ItemBase*> _ItemList)
{
	list<ItemBase*>::iterator listiter = _ItemList.begin();

	for (listiter = _ItemList.begin(); listiter != _ItemList.end(); listiter++)
	{
		PushItem(*listiter);
	}

	return;
}

void UI_ItemBox::GetItemBoxInfo(ItemBox* _ItemBox)
{
	for (size_t i = 0; i < 8; i++)
	{
		if (_ItemBox->GetItem(i) != nullptr)
		{
			PushItem(_ItemBox->GetItem(i));
		}
	}
}

bool UI_ItemBox::MouseCollisionCheck()
{
	if (true == ItemBoxCollision->Collision((int)CollisionGroup::MousePointer))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UI_ItemBox::PushItem(ItemBase* _OriginItemBase, int _SlotNumber)
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
	default:
	{
		GameEngineDebug::MsgBoxError("슬롯번호가 제대로 세팅되지 않았습니다");
		return;
	}
		break;
	}
}

void UI_ItemBox::EmptySlotReturn(ItemBase* _TargetSlot)
{

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

	GameEngineDebug::MsgBox("아이템 박스에 빈 슬롯이 없습니다");
	return;

}

void UI_ItemBox::ItemRenderCheck()
{
	if (Slot0_Item != nullptr)
	{
		Slot0_ItemRenderer->GetTransform()->SetLocalPosition(Slot0Collision->GetTransform()->GetLocalPosition());
		Slot0_ItemRenderer->GetTransform()->SetLocalScaling(ItemBox_SlotSize);
		Slot0_ItemRenderer->SetImage("Item_Bottle_Test.png");
	}
	else
	{
		Slot0_ItemRenderer->Off();
	}


	if (Slot1_Item != nullptr)
	{
		Slot1_ItemRenderer->GetTransform()->SetLocalPosition(Slot1Collision->GetTransform()->GetLocalPosition());
		Slot1_ItemRenderer->GetTransform()->SetLocalScaling(ItemBox_SlotSize);
		Slot1_ItemRenderer->SetImage("Item_Bottle_Test.png");
	}
	else
	{
		Slot1_ItemRenderer->Off();
	}


	if (Slot2_Item != nullptr)
	{
		Slot2_ItemRenderer->GetTransform()->SetLocalPosition(Slot2Collision->GetTransform()->GetLocalPosition());
		Slot2_ItemRenderer->GetTransform()->SetLocalScaling(ItemBox_SlotSize);
		Slot2_ItemRenderer->SetImage("Item_Bottle_Test.png");
	}
	else
	{
		Slot2_ItemRenderer->Off();
	}

	if (Slot3_Item != nullptr)
	{		
		Slot3_ItemRenderer->GetTransform()->SetLocalPosition(Slot2Collision->GetTransform()->GetLocalPosition());
		Slot3_ItemRenderer->GetTransform()->SetLocalScaling(ItemBox_SlotSize);
		Slot3_ItemRenderer->SetImage("Item_Bottle_Test.png");
	}		
	else	
	{		
		Slot3_ItemRenderer->Off();
	}

	if (Slot4_Item != nullptr)
	{
		Slot4_ItemRenderer->GetTransform()->SetLocalPosition(Slot2Collision->GetTransform()->GetLocalPosition());
		Slot4_ItemRenderer->GetTransform()->SetLocalScaling(ItemBox_SlotSize);
		Slot4_ItemRenderer->SetImage("Item_Bottle_Test.png");
	}
	else
	{
		Slot4_ItemRenderer->Off();
	}

	if (Slot5_Item != nullptr)
	{
		Slot5_ItemRenderer->GetTransform()->SetLocalPosition(Slot2Collision->GetTransform()->GetLocalPosition());
		Slot5_ItemRenderer->GetTransform()->SetLocalScaling(ItemBox_SlotSize);
		Slot5_ItemRenderer->SetImage("Item_Bottle_Test.png");
	}
	else
	{
		Slot5_ItemRenderer->Off();
	}

	if (Slot6_Item != nullptr)
	{
		Slot6_ItemRenderer->GetTransform()->SetLocalPosition(Slot2Collision->GetTransform()->GetLocalPosition());
		Slot6_ItemRenderer->GetTransform()->SetLocalScaling(ItemBox_SlotSize);
		Slot6_ItemRenderer->SetImage("Item_Bottle_Test.png");
	}
	else
	{
		Slot6_ItemRenderer->Off();
	}

	if (Slot7_Item != nullptr)
	{
		Slot7_ItemRenderer->GetTransform()->SetLocalPosition(Slot2Collision->GetTransform()->GetLocalPosition());
		Slot7_ItemRenderer->GetTransform()->SetLocalScaling(ItemBox_SlotSize);
		Slot7_ItemRenderer->SetImage("Item_Bottle_Test.png");
	}
	else
	{
		Slot7_ItemRenderer->Off();
	}

}