#include "PreCompile.h"
#include "ItemBoxWindow.h"
#include "ItemBoxManager.h"
#include "ItemBase.h"
#include <GameEngine/GameEngineCore.h>
#include "EquipmentItem.h"
#include "UseableItem.h"
#include "MiscItem.h"

void ItemBoxWindow::CreateAllItemList()
{
	EquipmentItem* Item = reinterpret_cast<EquipmentItem*>(
		CreateItem("°¡À§", ItemType::Equipment, "ItemIcon_101101.png"));
	CreateItem("º¼Ææ", ItemType::Useable, "ItemIcon_101102.png");
	CreateItem("Ä®", ItemType::Misc, "ItemIcon_101104.png");
}

ItemBase* ItemBoxWindow::CreateItem(const std::string _Name, ItemType _Type,
	const std::string _ImageName /*= ""*/)
{
	ItemBase* NewItem = nullptr;

	switch (_Type)
	{
	case ItemType::None:
		NewItem = GameEngineCore::CurrentLevel()->CreateActor<ItemBase>();
		break;
	case ItemType::Equipment:
		NewItem = GameEngineCore::CurrentLevel()->CreateActor<EquipmentItem>();
		break;
	case ItemType::Useable:
		NewItem = GameEngineCore::CurrentLevel()->CreateActor<UseableItem>();
		break;
	case ItemType::Misc:
		NewItem = GameEngineCore::CurrentLevel()->CreateActor<MiscItem>();
		break;
	default:
		break;
	}

	NewItem->SetName(_Name);
	NewItem->SetItemType(_Type);

	if ("" != _ImageName)
	{
		NewItem->SetImage(_ImageName);
	}

	AllItemList.push_back(NewItem);

	AllItemName.push_back(NewItem->GetName().c_str());

	return NewItem;
}
