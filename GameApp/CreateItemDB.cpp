#include "PreCompile.h"
#include "ItemBoxManager.h"
#include "ItemBase.h"
#include <GameEngine/GameEngineCore.h>
#include "EquipmentItem.h"
#include "UseableItem.h"
#include "MiscItem.h"

#include "LumiaLevel.h"

static int itemIndex = 0;

void ItemBoxManager::CreateAllItemList()
{
	CreateMiscItemList();		// 재료
	CreateUseableItemList();	// 소모품
	CreateEquipmentItemList();	// 장비
	CreateWeaponItemList();		// 무기
}

ItemBase* ItemBoxManager::CreateItem(const std::string _Name, ItemType _Type, ItemTier _Tier,
	const std::string _ImageName /*= ""*/)
{
	ItemBase* NewItem = nullptr;

	// 221005 SJH ADD : LumiaLevel Get
	GameEngineLevel* CurLevel = GetLevel();
	if (nullptr == CurLevel)
	{
		CurLevel = dynamic_cast<LumiaLevel*>(GameEngineCore::LevelFind("LumiaLevel"));
	}

	switch (_Type)
	{
	case ItemType::NONE:
		NewItem = CurLevel->CreateActor<ItemBase>();
		break;
	case ItemType::EQUIPMENT:
		NewItem = CurLevel->CreateActor<EquipmentItem>();
		break;
	case ItemType::USEABLE:
		NewItem = CurLevel->CreateActor<UseableItem>();
		break;
	case ItemType::MISC:
		NewItem = CurLevel->CreateActor<MiscItem>();
		break;
	default:
		break;
	}

	if (nullptr == NewItem)
	{
		GameEngineDebug::MsgBoxError("아이템을 생성하지 못했습니다.");
	}

	NewItem->SetName(_Name);
	NewItem->SetItemType(_Type);
	NewItem->SetItemTier(_Tier);
	NewItem->SetIndex(itemIndex++);

	std::string imageName = _ImageName;
	GameEngineTexture* CurTexture = GameEngineTextureManager::GetInst().Find(imageName);

	if (nullptr == CurTexture)
	{
		imageName = "ItemIcon_101101.png";
	}

	NewItem->SetImage(imageName);

	allItemList_.push_back(NewItem);

	return NewItem;
}

MiscItem* ItemBoxManager::CreateMiscItem(const std::string _Name, 
	const std::string _ImageName,
	ItemTier _Tier/* = ItemTier::COMMON*/)
{
	return reinterpret_cast<MiscItem*>(CreateItem(_Name, ItemType::MISC, _Tier, _ImageName));
}

MiscItem* ItemBoxManager::CreateMiscItem(const std::string _Name, ItemTier _Tier)
{
	std::string imageName = _Name + ".png";
	return reinterpret_cast<MiscItem*>(CreateItem(_Name, ItemType::MISC, _Tier, imageName));
}

UseableItem* ItemBoxManager::CreateUseableItem(const std::string _Name,
	const std::string _ImageName,
	ItemTier _Tier/* = ItemTier::COMMON*/)
{
	return reinterpret_cast<UseableItem*>(CreateItem(_Name, ItemType::USEABLE, _Tier, _ImageName));
}

UseableItem* ItemBoxManager::CreateUseableItem(const std::string _Name,
	ItemTier _Tier)
{
	std::string imageName = _Name + ".png";
	return reinterpret_cast<UseableItem*>(CreateItem(_Name, ItemType::USEABLE, _Tier, imageName));
}

EquipmentItem* ItemBoxManager::CreateEquipmentItem(const std::string _Name,
	EquipmentType _EquipType,
	const std::string _ImageName,
	ItemTier _Tier/* = ItemTier::COMMON*/)
{
	EquipmentItem* item = reinterpret_cast<EquipmentItem*>(
		CreateItem(_Name, ItemType::EQUIPMENT, _Tier, _ImageName));
	item->SetEquipType(_EquipType);
	return item;
}

EquipmentItem* ItemBoxManager::CreateEquipmentItem(const std::string _Name, 
	EquipmentType _EquipType, ItemTier _Tier)
{
	std::string imageName = _Name + ".png";
	EquipmentItem* item = reinterpret_cast<EquipmentItem*>(
		CreateItem(_Name, ItemType::EQUIPMENT, _Tier, imageName));
	item->SetEquipType(_EquipType);
	return item;
}

void ItemBoxManager::CreateAllItemRecipes()
{
	// 조합 Test용
	CreateItemRecipe("Scissors", "FountainPen", "LongRifle");

	// 현우 쿨감 스증 [공 - 숲 - 고 - 묘 - 학]
	
	// 플라즈마 톤파
	CreateItemRecipe("Stone", "Chalk", "WhitePowder");
	CreateItemRecipe("Branch", "Bamboo", "WoodenTonfa");
	CreateItemRecipe("WhitePowder", "WoodenTonfa", "RyukyuTonfa");
	CreateItemRecipe("RyukyuTonfa", "LaserPointer", "PlasmaTonfa");

	// 지휘관의 갑옷
	CreateItemRecipe("FabricArmor", "Leather", "LeatherArmor");
	CreateItemRecipe("LeatherArmor", "SteelChain", "ChainArmor");
	CreateItemRecipe("ChainArmor", "Gold", "CommandersArmor");

	// 황실 부르고넷
	CreateItemRecipe("Hat", "SteelChain", "ChainCoif");
	CreateItemRecipe("Hairband", "Feather", "Mask");
	CreateItemRecipe("ChainCoif", "Mask", "CloseHelm");
	CreateItemRecipe("Pickaxe", "Gemstone", "Gold");
	CreateItemRecipe("CloseHelm", "Gold", "ImperialBurgonet");
	
	// 틴달로스의 팔찌
	CreateItemRecipe("Bracelet", "Gold", "GoldenBracelet");
	CreateItemRecipe("Watch", "Glue", "BrokenWatch");
	CreateItemRecipe("GoldenBracelet", "BrokenWatch", "TindalosBand");
	
	// 타키온 브레이스
	CreateItemRecipe("Tights", "Glue", "KneePads");
	CreateItemRecipe("Battery", "PianoWire", "ElectronicParts");
	CreateItemRecipe("ElectronicParts", "ScrapMetal", "Motor");
	CreateItemRecipe("KneePads", "Motor", "TachyonBrace");
	
	// 백우선
	CreateItemRecipe("Fan", "Nail", "GildedQuillFan");
	CreateItemRecipe("GildedQuillFan", "Feather", "WhiteCraneFan");
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

	// Uncommon
	CreateMiscItem("Steel", ItemTier::UNCOMMON);
	CreateMiscItem("Oilcloth", ItemTier::UNCOMMON);
	CreateMiscItem("HeatedOil", ItemTier::UNCOMMON);
	CreateMiscItem("WhitePowder", ItemTier::UNCOMMON);
	CreateMiscItem("Ash", ItemTier::UNCOMMON);
	CreateMiscItem("ElectronicParts", ItemTier::UNCOMMON);
	CreateMiscItem("Blueprint", ItemTier::UNCOMMON);
	CreateMiscItem("IronSheet", ItemTier::UNCOMMON);
	CreateMiscItem("Gold", ItemTier::UNCOMMON);
	CreateMiscItem("HeatedStone", ItemTier::UNCOMMON);
	CreateMiscItem("BarbedWire", ItemTier::UNCOMMON);
	CreateMiscItem("Ruby", ItemTier::UNCOMMON);
	CreateMiscItem("Hardcover", ItemTier::UNCOMMON);

	// Rare
	CreateMiscItem("Poison", ItemTier::RARE);
	CreateMiscItem("Motor", ItemTier::RARE);
	CreateMiscItem("GlassPanel", ItemTier::RARE);
	CreateMiscItem("IonBattery", ItemTier::RARE);
	CreateMiscItem("CellPhone", ItemTier::RARE);

	// Epic
	CreateMiscItem("GlacialIce", ItemTier::EPIC);
	CreateMiscItem("TrueSamadhiFire", ItemTier::EPIC);
	CreateMiscItem("TreeofLife", ItemTier::EPIC);
	CreateMiscItem("Meteorite", ItemTier::EPIC);
	CreateMiscItem("Moonstone", ItemTier::EPIC);
	CreateMiscItem("Mithril", ItemTier::EPIC);
	CreateMiscItem("VFBloodSample", ItemTier::EPIC);
	CreateMiscItem("ForceCore", ItemTier::EPIC);
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


	// Beverage 음료
	// Common 
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
	// Head
	EquipmentItem* item = CreateEquipmentItem("Hairband", EquipmentType::HEAD, "ItemIcon_101101.png");

	EquipmentType type = item->GetEquipType();
	CreateEquipmentItem("Hat", EquipmentType::HEAD, "ItemIcon_101101.png");
	CreateEquipmentItem("BikeHelmet", EquipmentType::HEAD, "ItemIcon_101101.png");

	CreateEquipmentItem("Mask", EquipmentType::HEAD, ItemTier::UNCOMMON);
	CreateEquipmentItem("Circlet", EquipmentType::HEAD, ItemTier::UNCOMMON);
	CreateEquipmentItem("Beret", EquipmentType::HEAD, ItemTier::UNCOMMON);
	CreateEquipmentItem("ChainCoif", EquipmentType::HEAD, ItemTier::UNCOMMON);
	CreateEquipmentItem("SafetyHelmet", EquipmentType::HEAD, ItemTier::UNCOMMON);
	CreateEquipmentItem("CrownofBuds", EquipmentType::HEAD, ItemTier::UNCOMMON);

	CreateEquipmentItem("BallisticHelmet", EquipmentType::HEAD, ItemTier::RARE);
	CreateEquipmentItem("FireHelmet", EquipmentType::HEAD, ItemTier::RARE);
	CreateEquipmentItem("Tiara", EquipmentType::HEAD, ItemTier::RARE);
	CreateEquipmentItem("Crown", EquipmentType::HEAD, ItemTier::RARE);
	CreateEquipmentItem("CloseHelm", EquipmentType::HEAD, ItemTier::RARE);
	CreateEquipmentItem("GarlandofPetals", EquipmentType::HEAD, ItemTier::RARE);

	CreateEquipmentItem("VirtuousOutlaw", EquipmentType::HEAD, ItemTier::EPIC);
	CreateEquipmentItem("CrystalTiara", EquipmentType::HEAD, ItemTier::EPIC);
	CreateEquipmentItem("MotorcycleHelmet", EquipmentType::HEAD, ItemTier::EPIC);
	CreateEquipmentItem("TacticalOPSHelmet", EquipmentType::HEAD, ItemTier::EPIC);
	CreateEquipmentItem("HelmofBanneret", EquipmentType::HEAD, ItemTier::EPIC);
	CreateEquipmentItem("ImperialCrown", EquipmentType::HEAD, ItemTier::EPIC);
	CreateEquipmentItem("ImperialBurgonet", EquipmentType::HEAD, ItemTier::EPIC);
	CreateEquipmentItem("MohawkHeadgear", EquipmentType::HEAD, ItemTier::EPIC);
	CreateEquipmentItem("Vigilante", EquipmentType::HEAD, ItemTier::EPIC);
	CreateEquipmentItem("Diadem", EquipmentType::HEAD, ItemTier::EPIC);
	CreateEquipmentItem("CrusaderHelmet", EquipmentType::HEAD, ItemTier::EPIC);
	CreateEquipmentItem("HarmonyinFullBloom", EquipmentType::HEAD, ItemTier::EPIC);

	// Chest
	CreateEquipmentItem("Windbreaker", EquipmentType::CHEST, "ItemIcon_101101.png");
	CreateEquipmentItem("MonksRobe", EquipmentType::CHEST, "ItemIcon_101101.png");
	CreateEquipmentItem("Wetsuit", EquipmentType::CHEST, "ItemIcon_101101.png");
	CreateEquipmentItem("FabricArmor", EquipmentType::CHEST, "ItemIcon_101101.png");

	CreateEquipmentItem("LeatherArmor", EquipmentType::CHEST, ItemTier::UNCOMMON);
	CreateEquipmentItem("LeatherJacket", EquipmentType::CHEST, ItemTier::UNCOMMON);
	CreateEquipmentItem("TurtleDobok", EquipmentType::CHEST, ItemTier::UNCOMMON);
	CreateEquipmentItem("MilitarySuit", EquipmentType::CHEST, ItemTier::UNCOMMON);
	CreateEquipmentItem("PatchedRobe", EquipmentType::CHEST, ItemTier::UNCOMMON);
	CreateEquipmentItem("Dress", EquipmentType::CHEST, ItemTier::UNCOMMON);
	CreateEquipmentItem("Bikini", EquipmentType::CHEST, ItemTier::UNCOMMON);
	CreateEquipmentItem("DivingSuit", EquipmentType::CHEST, ItemTier::UNCOMMON);
	CreateEquipmentItem("DeaconRobes", EquipmentType::CHEST, ItemTier::UNCOMMON);

	CreateEquipmentItem("RiderJacket", EquipmentType::CHEST, ItemTier::RARE);
	CreateEquipmentItem("ChainArmor", EquipmentType::CHEST, ItemTier::RARE);
	CreateEquipmentItem("Suit", EquipmentType::CHEST, ItemTier::RARE);
	CreateEquipmentItem("Qipao", EquipmentType::CHEST, ItemTier::RARE);
	CreateEquipmentItem("SheetMetalArmor", EquipmentType::CHEST, ItemTier::RARE);
	CreateEquipmentItem("Hanbok", EquipmentType::CHEST, ItemTier::RARE);
	CreateEquipmentItem("BulletproofVest", EquipmentType::CHEST, ItemTier::RARE);
	CreateEquipmentItem("CovertAgentUniform", EquipmentType::CHEST, ItemTier::RARE);

	CreateEquipmentItem("CardinalRobes", EquipmentType::CHEST, ItemTier::EPIC);
	CreateEquipmentItem("SunsetArmor", EquipmentType::CHEST, ItemTier::EPIC);
	CreateEquipmentItem("OpticalCamouflageSuit", EquipmentType::CHEST, ItemTier::EPIC);
	CreateEquipmentItem("RockersJacket", EquipmentType::CHEST, ItemTier::EPIC);
	CreateEquipmentItem("CrusaderArmor", EquipmentType::CHEST, ItemTier::EPIC);
	CreateEquipmentItem("AmazonessArmor", EquipmentType::CHEST, ItemTier::EPIC);
	CreateEquipmentItem("DragonDobok", EquipmentType::CHEST, ItemTier::EPIC);
	CreateEquipmentItem("CommandersArmor", EquipmentType::CHEST, ItemTier::EPIC);
	CreateEquipmentItem("ButlersSuit", EquipmentType::CHEST, ItemTier::EPIC);
	CreateEquipmentItem("BattleSuit", EquipmentType::CHEST, ItemTier::EPIC);
	CreateEquipmentItem("EODSuit", EquipmentType::CHEST, ItemTier::EPIC);
	CreateEquipmentItem("Tuxedo", EquipmentType::CHEST, ItemTier::EPIC);
	CreateEquipmentItem("HighPriestRobes", EquipmentType::CHEST, ItemTier::EPIC);
	CreateEquipmentItem("Changpao", EquipmentType::CHEST, ItemTier::EPIC);
	
	// Arm
	CreateEquipmentItem("Watch", EquipmentType::ARM, "ItemIcon_101101.png");
	CreateEquipmentItem("Bandage", EquipmentType::ARM, "ItemIcon_101101.png");
	CreateEquipmentItem("Bracelet", EquipmentType::ARM, "ItemIcon_101101.png");

	CreateEquipmentItem("LeatherShield", EquipmentType::ARM, ItemTier::UNCOMMON);
	CreateEquipmentItem("SquadLeaderArmband", EquipmentType::ARM, ItemTier::UNCOMMON);
	CreateEquipmentItem("Bracer", EquipmentType::ARM, ItemTier::UNCOMMON);
	CreateEquipmentItem("BrokenWatch", EquipmentType::ARM, ItemTier::UNCOMMON);

	CreateEquipmentItem("Sheath", EquipmentType::ARM, ItemTier::RARE);
	CreateEquipmentItem("GoldenBracelet", EquipmentType::ARM, ItemTier::RARE);
	CreateEquipmentItem("Bazuband", EquipmentType::ARM, ItemTier::RARE);
	CreateEquipmentItem("CrimsonBracelet", EquipmentType::ARM, ItemTier::RARE);
	CreateEquipmentItem("BarbedBlossom", EquipmentType::ARM, ItemTier::RARE);
	CreateEquipmentItem("SteelShield", EquipmentType::ARM, ItemTier::RARE);
	CreateEquipmentItem("VitalSignSensor", EquipmentType::ARM, ItemTier::RARE);

	CreateEquipmentItem("CorruptingTouch", EquipmentType::ARM, ItemTier::EPIC);
	CreateEquipmentItem("SwordStopper", EquipmentType::ARM, ItemTier::EPIC);
	CreateEquipmentItem("Draupnir", EquipmentType::ARM, ItemTier::EPIC);
	CreateEquipmentItem("CreedoftheKnight", EquipmentType::ARM, ItemTier::EPIC);
	CreateEquipmentItem("SheathofShahJahan", EquipmentType::ARM, ItemTier::EPIC);
	CreateEquipmentItem("BurnishedAegis", EquipmentType::ARM, ItemTier::EPIC);
	CreateEquipmentItem("TindalosBand", EquipmentType::ARM, ItemTier::EPIC);
	CreateEquipmentItem("Nightingale", EquipmentType::ARM, ItemTier::EPIC);
	CreateEquipmentItem("PlasmaArc", EquipmentType::ARM, ItemTier::EPIC);
	CreateEquipmentItem("SmartBand", EquipmentType::ARM, ItemTier::EPIC);
	CreateEquipmentItem("Radar", EquipmentType::ARM, ItemTier::EPIC);

	// Leg
	CreateEquipmentItem("Slippers", EquipmentType::LEG, "ItemIcon_101101.png");
	CreateEquipmentItem("RunningShoes", EquipmentType::LEG, "ItemIcon_101101.png");
	CreateEquipmentItem("Tights", EquipmentType::LEG, "ItemIcon_101101.png");

	CreateEquipmentItem("KneePads", EquipmentType::LEG, ItemTier::UNCOMMON);
	CreateEquipmentItem("ChainLeggings", EquipmentType::LEG, ItemTier::UNCOMMON);
	CreateEquipmentItem("HighHeels", EquipmentType::LEG, ItemTier::UNCOMMON);
	CreateEquipmentItem("Heelys", EquipmentType::LEG, ItemTier::UNCOMMON);
	CreateEquipmentItem("Clogs", EquipmentType::LEG, ItemTier::UNCOMMON);
	CreateEquipmentItem("RepairedSlippers", EquipmentType::LEG, ItemTier::UNCOMMON);
	CreateEquipmentItem("Boots", EquipmentType::LEG, ItemTier::UNCOMMON);

	CreateEquipmentItem("HikingBoots", EquipmentType::LEG, ItemTier::RARE);
	CreateEquipmentItem("GlacierCrampons", EquipmentType::LEG, ItemTier::RARE);
	CreateEquipmentItem("SteelKneePads", EquipmentType::LEG, ItemTier::RARE);
	CreateEquipmentItem("CombatBoots", EquipmentType::LEG, ItemTier::RARE);
	CreateEquipmentItem("KillerHeels", EquipmentType::LEG, ItemTier::RARE);

	CreateEquipmentItem("FeatherBoots", EquipmentType::LEG, ItemTier::EPIC);
	CreateEquipmentItem("MaverickRunner", EquipmentType::LEG, ItemTier::EPIC);
	CreateEquipmentItem("StraitjacketSneakers", EquipmentType::LEG, ItemTier::EPIC);
	CreateEquipmentItem("Bucephalus", EquipmentType::LEG, ItemTier::EPIC);
	CreateEquipmentItem("EODBoots", EquipmentType::LEG, ItemTier::EPIC);
	CreateEquipmentItem("WhiteRhinos", EquipmentType::LEG, ItemTier::EPIC);
	CreateEquipmentItem("TachyonBrace", EquipmentType::LEG, ItemTier::EPIC);
	CreateEquipmentItem("SCV", EquipmentType::LEG, ItemTier::EPIC);

	// Accessory
	CreateEquipmentItem("Feather", EquipmentType::ACCESSORY, "ItemIcon_101101.png");
	CreateEquipmentItem("Ribbon", EquipmentType::ACCESSORY, "ItemIcon_101101.png");
	CreateEquipmentItem("Fan", EquipmentType::ACCESSORY, "ItemIcon_101101.png");
	CreateEquipmentItem("BuddhistScripture", EquipmentType::ACCESSORY, "ItemIcon_101101.png");
	CreateEquipmentItem("Box", EquipmentType::ACCESSORY, "ItemIcon_101101.png");
	CreateEquipmentItem("HolyGrail", EquipmentType::ACCESSORY, "ItemIcon_101101.png");
	CreateEquipmentItem("Cross", EquipmentType::ACCESSORY, "ItemIcon_101101.png");
	CreateEquipmentItem("Binoculars", EquipmentType::ACCESSORY, "ItemIcon_101101.png");

	CreateEquipmentItem("SaintsRelic", EquipmentType::ACCESSORY, ItemTier::UNCOMMON);
	CreateEquipmentItem("FlowerofFate", EquipmentType::ACCESSORY, ItemTier::UNCOMMON);
	CreateEquipmentItem("GlassPieces", EquipmentType::ACCESSORY, ItemTier::UNCOMMON);
	CreateEquipmentItem("Doll", EquipmentType::ACCESSORY, ItemTier::UNCOMMON);
	CreateEquipmentItem("SniperScope", EquipmentType::ACCESSORY, ItemTier::UNCOMMON);
	CreateEquipmentItem("BuddhaSarira", EquipmentType::ACCESSORY, ItemTier::UNCOMMON);
	CreateEquipmentItem("Quiver", EquipmentType::ACCESSORY, ItemTier::UNCOMMON);
	CreateEquipmentItem("FeatherDuster", EquipmentType::ACCESSORY, ItemTier::UNCOMMON);
	CreateEquipmentItem("GildedQuillFan", EquipmentType::ACCESSORY, ItemTier::UNCOMMON);
	CreateEquipmentItem("ShamansBronze", EquipmentType::ACCESSORY, ItemTier::UNCOMMON);
	CreateEquipmentItem("DecorativeFlintlock", EquipmentType::ACCESSORY, ItemTier::UNCOMMON);
	CreateEquipmentItem("MarksmanHandbook", EquipmentType::ACCESSORY, ItemTier::UNCOMMON);

	CreateEquipmentItem("WhiteCraneFan", EquipmentType::ACCESSORY, ItemTier::RARE);
	CreateEquipmentItem("Uchiwa", EquipmentType::ACCESSORY, ItemTier::RARE);
	CreateEquipmentItem("Magazine", EquipmentType::ACCESSORY, ItemTier::RARE);
	CreateEquipmentItem("LacedQuiver", EquipmentType::ACCESSORY, ItemTier::RARE);
	CreateEquipmentItem("RevengeofGoujian", EquipmentType::ACCESSORY, ItemTier::RARE);
	CreateEquipmentItem("BuccaneerDoubloon", EquipmentType::ACCESSORY, ItemTier::RARE);
	CreateEquipmentItem("Hawkeye", EquipmentType::ACCESSORY, ItemTier::RARE);
	CreateEquipmentItem("JollyRoger", EquipmentType::ACCESSORY, ItemTier::RARE);
	CreateEquipmentItem("MusicBox", EquipmentType::ACCESSORY, ItemTier::RARE);
	CreateEquipmentItem("ActiveCamouflage", EquipmentType::ACCESSORY, ItemTier::RARE);
	CreateEquipmentItem("Grimoire", EquipmentType::ACCESSORY, ItemTier::RARE);
	CreateEquipmentItem("AetherFringe", EquipmentType::ACCESSORY, ItemTier::RARE);
	CreateEquipmentItem("PileBunker", EquipmentType::ACCESSORY, ItemTier::RARE);
	CreateEquipmentItem("SchrodingersBox", EquipmentType::ACCESSORY, ItemTier::RARE);
	CreateEquipmentItem("VeritasLuxMea", EquipmentType::ACCESSORY, ItemTier::RARE);
	
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
	CreateEquipmentItem("WoodenTonfa", EquipmentType::WEAPON, ItemTier::UNCOMMON);
	CreateEquipmentItem("PoliceBaton", EquipmentType::WEAPON, ItemTier::RARE);
	CreateEquipmentItem("RyukyuTonfa", EquipmentType::WEAPON, ItemTier::RARE);
	CreateEquipmentItem("TacticalTonfa", EquipmentType::WEAPON, ItemTier::EPIC);
	CreateEquipmentItem("MaiSok", EquipmentType::WEAPON, ItemTier::EPIC);
	CreateEquipmentItem("PlasmaTonfa", EquipmentType::WEAPON, ItemTier::EPIC);

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
