#include "PreCompile.h"
#include "ItemBoxManager.h"
#include "ItemBase.h"
#include <GameEngine/GameEngineCore.h>
#include "EquipmentItem.h"
#include "UseableItem.h"
#include "MiscItem.h"

void ItemBoxManager::CreateAllItemList()
{
	EquipmentItem* Item = reinterpret_cast<EquipmentItem*>(
		CreateItem("Scissors", ItemType::Equipment, "ItemIcon_101101.png"));
	CreateItem("Pen", ItemType::Useable, "ItemIcon_101102.png");
	CreateItem("Knite", ItemType::Misc, "ItemIcon_101104.png");
}

ItemBase* ItemBoxManager::CreateItem(const std::string _Name, ItemType _Type,
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

	allItemList.push_back(NewItem);

	return NewItem;
}
