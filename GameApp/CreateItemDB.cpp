#include "PreCompile.h"
#include "ItemBoxManager.h"
#include "ItemBase.h"
#include <GameEngine/GameEngineCore.h>
#include "EquipmentItem.h"
#include "UseableItem.h"
#include "MiscItem.h"

void ItemBoxManager::CreateAllItemList()
{
	// Test용
	EquipmentItem* Item = CreateEquipmentItem("Scissors", "ItemIcon_101101.png");
	Item->SetEquipType(EquipmentType::WEAPON);
	CreateItem("Pen", ItemType::USEABLE, "ItemIcon_101102.png");
	CreateItem("Knite", ItemType::MISC, "ItemIcon_101104.png");


	CreateMiscItemList();		// 재료
	CreateUseableItemList();	// 소모품
	CreateEquipmentItemList();	// 장비
}

ItemBase* ItemBoxManager::CreateItem(const std::string _Name, ItemType _Type,
	const std::string _ImageName /*= ""*/)
{
	ItemBase* NewItem = nullptr;

	switch (_Type)
	{
	case ItemType::NONE:
		NewItem = GameEngineCore::CurrentLevel()->CreateActor<ItemBase>();
		break;
	case ItemType::EQUIPMENT:
		NewItem = GameEngineCore::CurrentLevel()->CreateActor<EquipmentItem>();
		break;
	case ItemType::USEABLE:
		NewItem = GameEngineCore::CurrentLevel()->CreateActor<UseableItem>();
		break;
	case ItemType::MISC:
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

	allItemList_.push_back(NewItem);

	return NewItem;
}

MiscItem* ItemBoxManager::CreateMiscItem(const std::string _Name, const std::string _ImageName)
{
	return reinterpret_cast<MiscItem*>(CreateItem(_Name, ItemType::MISC, _ImageName));
}

UseableItem* ItemBoxManager::CreateUseableItem(const std::string _Name, const std::string _ImageName)
{
	return reinterpret_cast<UseableItem*>(CreateItem(_Name, ItemType::USEABLE, _ImageName));
}

EquipmentItem* ItemBoxManager::CreateEquipmentItem(const std::string _Name, const std::string _ImageName)
{
	return reinterpret_cast<EquipmentItem*>(CreateItem(_Name, ItemType::MISC, _ImageName));
}


void ItemBoxManager::CreateMiscItemList()
{
	// Normal
	CreateItem("FountainPen", ItemType::MISC, "ItemIcon_101102.png");
	CreateItem("Pickaxe", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("Branch", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("Stone", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("GlassBottle", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("Chalk", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("Flower", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("Nail", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("Leather", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("TurtleShell", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("Rubber", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("ScrapMetal", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("Lighter", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("LaserPointer", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("StallionMedal", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("Battery", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("Alcohol", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("Oil", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("Cloth", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("Gemstone", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("Glue", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("Paper", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("IronOre", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("Can", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("Gunpowder", ItemType::MISC, "ItemIcon_101104.png");
	CreateItem("PianoWire", ItemType::MISC, "ItemIcon_101104.png");
}

void ItemBoxManager::CreateUseableItemList()
{
}

void ItemBoxManager::CreateEquipmentItemList()
{
}
