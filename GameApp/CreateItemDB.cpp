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

	// Normal Misc Àç·á
	CreateItem("FountainPen", ItemType::Misc, "ItemIcon_101102.png");
	CreateItem("Pickaxe", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("Branch", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("Stone", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("GlassBottle", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("Chalk", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("Flower", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("Nail", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("Leather", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("TurtleShell", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("Rubber", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("ScrapMetal", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("Lighter", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("LaserPointer", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("StallionMedal", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("Battery", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("Alcohol", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("Oil", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("Cloth", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("Gemstone", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("Glue", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("Paper", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("IronOre", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("Can", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("Gunpowder", ItemType::Misc, "ItemIcon_101104.png");
	CreateItem("PianoWire", ItemType::Misc, "ItemIcon_101104.png");
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

	allItemList_.push_back(NewItem);

	return NewItem;
}
