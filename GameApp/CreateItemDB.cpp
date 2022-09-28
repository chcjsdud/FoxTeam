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
	EquipmentItem* Item = CreateEquipmentItem("test1", 
		EquipmentType::WEAPON, "ItemIcon_101101.png");
	CreateItem("test2", ItemType::USEABLE, "ItemIcon_101102.png");
	CreateItem("test3", ItemType::MISC, "ItemIcon_101104.png");


	CreateMiscItemList();		// 재료
	CreateUseableItemList();	// 소모품
	CreateEquipmentItemList();	// 장비
	CreateWeaponItemList();		// 무기
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

EquipmentItem* ItemBoxManager::CreateEquipmentItem(const std::string _Name,
	EquipmentType _EquipType /*= EquipmentType::NONE*/ ,
	const std::string _ImageName /*= ""*/)
{
	EquipmentItem* item = reinterpret_cast<EquipmentItem*>(CreateItem(_Name, ItemType::EQUIPMENT, _ImageName));
	item->SetEquipType(_EquipType);
	return item;
}

void ItemBoxManager::CreateAllItemRecipes()
{
	itemRecipes_.insert(std::make_pair(CombineItem("재료1", "재료2"), "조합1"));
	itemRecipes_.insert(std::make_pair(CombineItem("재료4", "재료3"), "조합2"));
	itemRecipes_.insert(std::make_pair(CombineItem("재료5", "재료6"), "조합3"));

	std::map<CombineItem, std::string>::iterator iter = itemRecipes_.end();

	iter = itemRecipes_.find(CombineItem("재료2", "재료1"));

	std::string getstr;

	if (itemRecipes_.end() != iter)
	{
		getstr = (*iter).second;
	}

	int a = 0;
}

void ItemBoxManager::CreateMiscItemList()
{
	// Common
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
	// Common Food
	CreateUseableItem("Potato", "ItemIcon_101101.png");
	CreateUseableItem("Cod", "ItemIcon_101101.png");
	CreateUseableItem("Lemon", "ItemIcon_101101.png");
	CreateUseableItem("Garlic", "ItemIcon_101101.png");
	CreateUseableItem("Carp", "ItemIcon_101101.png");
	CreateUseableItem("Bread", "ItemIcon_101101.png");
	CreateUseableItem("Meat", "ItemIcon_101101.png");
	CreateUseableItem("Egg", "ItemIcon_101101.png");
	CreateUseableItem("Ramen", "ItemIcon_101101.png");
	CreateUseableItem("OrientalHerb", "ItemIcon_101101.png");
	CreateUseableItem("Chocolate", "ItemIcon_101101.png");

	// Common Beverage 음료
	CreateUseableItem("Honey", "ItemIcon_101101.png");
	CreateUseableItem("Water", "ItemIcon_101101.png");
	CreateUseableItem("Ice", "ItemIcon_101101.png");
	CreateUseableItem("Whiskey", "ItemIcon_101101.png");
	CreateUseableItem("Coffee", "ItemIcon_101101.png");
	CreateUseableItem("CarbonatedWater", "ItemIcon_101101.png");
	CreateUseableItem("Milk", "ItemIcon_101101.png");

}

void ItemBoxManager::CreateEquipmentItemList()
{
	// Common Head
	CreateEquipmentItem("Hairband", EquipmentType::HEAD, "ItemIcon_101101.png");
	CreateEquipmentItem("Hat", EquipmentType::HEAD, "ItemIcon_101101.png");
	CreateEquipmentItem("BikeHelmet", EquipmentType::HEAD, "ItemIcon_101101.png");

	// Common Chest
	CreateEquipmentItem("Windbreaker", EquipmentType::CHEST, "ItemIcon_101101.png");
	CreateEquipmentItem("MonksRobe", EquipmentType::CHEST, "ItemIcon_101101.png");
	CreateEquipmentItem("Wetsuit", EquipmentType::CHEST, "ItemIcon_101101.png");
	CreateEquipmentItem("FabricArmor", EquipmentType::CHEST, "ItemIcon_101101.png");
	
	// Common Arm
	CreateEquipmentItem("Watch", EquipmentType::ARM, "ItemIcon_101101.png");
	CreateEquipmentItem("Bandage", EquipmentType::ARM, "ItemIcon_101101.png");
	CreateEquipmentItem("Bracelet", EquipmentType::ARM, "ItemIcon_101101.png");

	// Common Leg
	CreateEquipmentItem("Slippers", EquipmentType::LEG, "ItemIcon_101101.png");
	CreateEquipmentItem("RunningShoes", EquipmentType::LEG, "ItemIcon_101101.png");
	CreateEquipmentItem("Tights", EquipmentType::LEG, "ItemIcon_101101.png");

	// Common Accessory
	CreateEquipmentItem("Feather", EquipmentType::ACCESSORY, "ItemIcon_101101.png");
	CreateEquipmentItem("Ribbon", EquipmentType::ACCESSORY, "ItemIcon_101101.png");
	CreateEquipmentItem("Fan", EquipmentType::ACCESSORY, "ItemIcon_101101.png");
	CreateEquipmentItem("BuddhistScripture", EquipmentType::ACCESSORY, "ItemIcon_101101.png");
	CreateEquipmentItem("Box", EquipmentType::ACCESSORY, "ItemIcon_101101.png");
	CreateEquipmentItem("HolyGrail", EquipmentType::ACCESSORY, "ItemIcon_101101.png");
	CreateEquipmentItem("Cross", EquipmentType::ACCESSORY, "ItemIcon_101101.png");
	CreateEquipmentItem("Binoculars", EquipmentType::ACCESSORY, "ItemIcon_101101.png");

	
}

void ItemBoxManager::CreateWeaponItemList()
{
	// Common Dagger
	CreateEquipmentItem("Scissors", EquipmentType::WEAPON, "ItemIcon_101101.png");
	CreateEquipmentItem("KitchenKnife", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Common Two-handed Sword
	CreateEquipmentItem("RustySword", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Common Axe
	CreateEquipmentItem("Hatchet", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Common Dual Swords
	CreateEquipmentItem("TwinBlades", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Common Pistol
	CreateEquipmentItem("WaltherPPK", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Common Assault Rifle
	CreateEquipmentItem("Fedorova", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Common Sniper Rifle
	CreateEquipmentItem("LongRifle", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Common Rapier
	CreateEquipmentItem("Needle", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Common Spear
	CreateEquipmentItem("ShortSpear", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Common Hammer
	CreateEquipmentItem("Hammer", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Common Bat
	CreateEquipmentItem("ShortRod", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Common Throw
	CreateEquipmentItem("IronBall", EquipmentType::WEAPON, "ItemIcon_101101.png");
	CreateEquipmentItem("Baseball", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Common Shuriken
	CreateEquipmentItem("Razor", EquipmentType::WEAPON, "ItemIcon_101101.png");
	CreateEquipmentItem("PlayingCards", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Common Bow
	CreateEquipmentItem("Bow", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Common Crossbow
	CreateEquipmentItem("ShortCrossbow", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Common Glove
	CreateEquipmentItem("BrassKnuckles", EquipmentType::WEAPON, "ItemIcon_101101.png");
	CreateEquipmentItem("CottonGloves", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Common Tonfa
	CreateEquipmentItem("Bamboo", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Common Guitar
	CreateEquipmentItem("StarterGuitar", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Common Nunchaku
	CreateEquipmentItem("SteelChain", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Common Whip
	CreateEquipmentItem("Whip", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Common Camera
	CreateEquipmentItem("Lens", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Common Arcana
	CreateEquipmentItem("GlassBead", EquipmentType::WEAPON, "ItemIcon_101101.png");
}
