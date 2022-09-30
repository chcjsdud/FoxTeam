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
	CreateItem("test2", ItemType::USEABLE, ItemTier::COMMON, "ItemIcon_101102.png");
	CreateItem("test3", ItemType::MISC, ItemTier::COMMON, "ItemIcon_101104.png");


	CreateMiscItemList();		// 재료
	CreateUseableItemList();	// 소모품
	CreateEquipmentItemList();	// 장비
	CreateWeaponItemList();		// 무기
}

ItemBase* ItemBoxManager::CreateItem(const std::string _Name, ItemType _Type, ItemTier _Tier,
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
	NewItem->SetItemTier(_Tier);

	if ("" != _ImageName)
	{
		NewItem->SetImage(_ImageName);
	}

	allItemList_.push_back(NewItem);

	return NewItem;
}

MiscItem* ItemBoxManager::CreateMiscItem(const std::string _Name, 
	const std::string _ImageName,
	ItemTier _Tier/* = ItemTier::COMMON*/)
{
	return reinterpret_cast<MiscItem*>(CreateItem(_Name, ItemType::MISC, _Tier, _ImageName));
}

UseableItem* ItemBoxManager::CreateUseableItem(const std::string _Name,
	const std::string _ImageName,
	ItemTier _Tier/* = ItemTier::COMMON*/)
{
	return reinterpret_cast<UseableItem*>(CreateItem(_Name, ItemType::USEABLE, _Tier, _ImageName));
}

EquipmentItem* ItemBoxManager::CreateEquipmentItem(const std::string _Name,
	EquipmentType _EquipType,
	const std::string _ImageName,
	ItemTier _Tier/* = ItemTier::COMMON*/)
{
	EquipmentItem* item = reinterpret_cast<EquipmentItem*>(CreateItem(_Name, ItemType::EQUIPMENT, _Tier, _ImageName));
	item->SetEquipType(_EquipType);
	return item;
}

void ItemBoxManager::CreateAllItemRecipes()
{
	/*itemRecipes_.insert(std::make_pair(CombineItem("재료1", "재료2"), "조합1"));
	itemRecipes_.insert(std::make_pair(CombineItem("재료4", "재료3"), "조합2"));
	itemRecipes_.insert(std::make_pair(CombineItem("재료5", "재료6"), "조합3"));

	std::map<CombineItem, std::string>::iterator iter = itemRecipes_.end();

	iter = itemRecipes_.find(CombineItem("재료2", "재료1"));

	std::string getstr;

	if (itemRecipes_.end() != iter)
	{
		getstr = (*iter).second;
	}*/

	// 플라즈마 톤파
	CreateItemRecipe("Stone", "Chalk", "WhitePowder");
	CreateItemRecipe("Branch", "Bamboo", "WoodenTonfa");
	CreateItemRecipe("WhitePowder", "WoodenTonfa", "RyukyuTonfa");
	CreateItemRecipe("RyukyuTonfa", "LaserPointer", "PlasmaTonfa");

	int a = 0;
}

void ItemBoxManager::CreateMiscItemList()
{
	// Common
	CreateMiscItem("FountainPen", "ItemIcon_101102.png");
	CreateMiscItem("Pickaxe", "ItemIcon_101104.png");
	CreateMiscItem("Branch", "ItemIcon_101104.png");
	CreateMiscItem("Stone", "ItemIcon_101104.png");
	CreateMiscItem("GlassBottle", "ItemIcon_101104.png");
	CreateMiscItem("Chalk", "ItemIcon_101104.png");
	CreateMiscItem("Flower", "ItemIcon_101104.png");
	CreateMiscItem("Nail", "ItemIcon_101104.png");
	CreateMiscItem("Leather", "ItemIcon_101104.png");
	CreateMiscItem("TurtleShell", "ItemIcon_101104.png");
	CreateMiscItem("Rubber", "ItemIcon_101104.png");
	CreateMiscItem("ScrapMetal", "ItemIcon_101104.png");
	CreateMiscItem("Lighter", "ItemIcon_101104.png");
	CreateMiscItem("LaserPointer", "ItemIcon_101104.png");
	CreateMiscItem("StallionMedal", "ItemIcon_101104.png");
	CreateMiscItem("Battery", "ItemIcon_101104.png");
	CreateMiscItem("Alcohol", "ItemIcon_101104.png");
	CreateMiscItem("Oil", "ItemIcon_101104.png");
	CreateMiscItem("Cloth", "ItemIcon_101104.png");
	CreateMiscItem("Gemstone", "ItemIcon_101104.png");
	CreateMiscItem("Glue", "ItemIcon_101104.png");
	CreateMiscItem("Paper", "ItemIcon_101104.png");
	CreateMiscItem("IronOre", "ItemIcon_101104.png");
	CreateMiscItem("Can", "ItemIcon_101104.png");
	CreateMiscItem("Gunpowder", "ItemIcon_101104.png");
	CreateMiscItem("PianoWire", "ItemIcon_101104.png");
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
