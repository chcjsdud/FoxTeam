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



	// 유키 [ 호 - 연 - 병 - 번 ]
	
	// 모노호시자오
	CreateItemRecipe("RustySword", "IronSheet", "Katana");
	CreateItemRecipe("Katana", "Gemstone", "Muramasa");
	CreateItemRecipe("FountainPen", "Paper", "Blueprint");
	CreateItemRecipe("Muramasa", "Blueprint", "Monohoshizao");

	// 창파오
	CreateItemRecipe("Cloth", "Scissors", "Dress");
	CreateItemRecipe("Dress", "Razor", "Qipao");
	CreateItemRecipe("Alcohol", "GlassBottle", "Baijiu");
	CreateItemRecipe("Baijiu", "Flower", "FlowerLiquor");
	CreateItemRecipe("Qipao", "FlowerLiquor", "Changpao");

	// 전술-OPS 헬멧
	CreateItemRecipe("Hat", "Scissors", "Beret");
	CreateItemRecipe("Beret", "BikeHelmet", "BallisticHelmet");
	CreateItemRecipe("BallisticHelmet", "ElectronicParts", "TacticalOPSHelmet");

	// 레이더
	CreateItemRecipe("Watch", "ElectronicParts", "VitalSignSensor");
	CreateItemRecipe("GlassBottle", "Stone", "GlassPieces");
	CreateItemRecipe("GlassPieces", "Glue", "GlassPanel");
	CreateItemRecipe("VitalSignSensor", "GlassPanel", "Radar");

	// 부케팔로스
	CreateItemRecipe("Slippers", "ScrapMetal", "HighHeels");
	CreateItemRecipe("HighHeels", "GlassPieces", "KillerHeels");
	CreateItemRecipe("Tights", "SteelChain", "ChainLeggings");
	CreateItemRecipe("KillerHeels", "ChainLeggings", "Bucephalus");

	// 해적의 증표
	CreateItemRecipe("WaltherPPK", "Ribbon", "DecorativeFlintlock");
	CreateItemRecipe("DecorativeFlintlock", "KitchenKnife", "BuccaneerDoubloon");



	// 재키 [ 절 - 호 - 골 - 번 - 연 ]
	
	// 저거너트
	CreateItemRecipe("Hatchet", "Bamboo", "BattleAxe");
	CreateItemRecipe("BattleAxe", "Feather", "LightHatchet");
	CreateItemRecipe("LightHatchet", "Motor", "TheJuggernaut");

	// 배틀슈트
	CreateItemRecipe("Windbreaker", "Branch", "MilitarySuit");
	CreateItemRecipe("ScrapMetal", "Hammer", "IronSheet");
	CreateItemRecipe("MilitarySuit", "IronSheet", "BulletproofVest");
	CreateItemRecipe("Wetsuit", "Rubber", "DivingSuit");
	CreateItemRecipe("BulletproofVest", "DivingSuit", "BattleSuit");

	// 전술 OPS 헬멧

	// 샤자한의 검집
	CreateItemRecipe("Leather", "IronSheet", "Sheath");
	CreateItemRecipe("Hammer", "Gemstone", "Ruby");
	CreateItemRecipe("Sheath", "Ruby", "SheathofShahJahan");

	// 클링온 부츠
	CreateItemRecipe("RunningShoes", "Oilcloth", "Boots");
	CreateItemRecipe("Boots", "Leather", "CombatBoots");
	CreateItemRecipe("CombatBoots", "Nail", "WhiteRhinos");

	// 오르골
	CreateItemRecipe("Box", "Motor", "MusicBox");



	// 아야 [ 학 - 호 - 모 - 고 - 절 ]

	// 엘레강스
	CreateItemRecipe("WaltherPPK", "Leather", "BerettaM92F");
	CreateItemRecipe("BerettaM92F", "LaserPointer", "FN57");
	CreateItemRecipe("Paper", "Lighter", "Ash");
	CreateItemRecipe("FN57", "Ash", "Elegance");

	// 지휘관의 갑옷

	// 황실 부르고넷

	// 포이즌드
	CreateItemRecipe("Ash", "Water", "Poison");
	CreateItemRecipe("CottonGloves", "Poison", "CorruptingTouch");

	// 풍화륜
	CreateItemRecipe("Slippers", "Cloth", "RepairedSlippers");
	CreateItemRecipe("RepairedSlippers", "Ash", "StraitjacketSneakers");

	// 마도서
	CreateItemRecipe("Paper", "Leather", "Hardcover");
	CreateItemRecipe("Flower", "PlayingCards", "FlowerofFate");
	CreateItemRecipe("Hardcover", "FlowerofFate", "Grimoire");



	// 리오 [ 절 - 항 - 고 - 병 ]

	// 제베의 활 + 궁기병의 화살통
	CreateItemRecipe("Bow", "Rubber", "Longbow");
	CreateItemRecipe("Longbow", "Gunpowder", "MightyBow");
	CreateItemRecipe("Feather", "Bamboo", "Quiver");
	CreateItemRecipe("Quiver", "ShortRod", "LacedQuiver");
	CreateItemRecipe("MightyBow", "LacedQuiver", "JebesAlteredBow");

	// 광학미채 슈트
	CreateItemRecipe("DivingSuit", "GlassPanel", "OpticalCamouflageSuit");

	// 수정 티아라
	CreateItemRecipe("Hairband", "Branch", "Circlet");
	CreateItemRecipe("Circlet", "StallionMedal", "Tiara");
	CreateItemRecipe("Tiara", "GlassPieces", "CrystalTiara");

	// 레이더

	// 부케팔로스

	// 궁기병의 화살통
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
	CreateUseableItem("Baijiu", "ItemIcon_101101.png");
	CreateUseableItem("FlowerLiquor", "ItemIcon_101101.png");

}

void ItemBoxManager::CreateEquipmentItemList()
{
	// Head
	CreateEquipmentItem("Hairband", EquipmentType::HEAD, "ItemIcon_101101.png");
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
	EquipmentItem* item = nullptr;

	// Dagger
	CreateEquipmentItem("Scissors", EquipmentType::WEAPON, "ItemIcon_101101.png");
	CreateEquipmentItem("KitchenKnife", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Two-handed Sword
	item = CreateEquipmentItem("RustySword", EquipmentType::WEAPON, "ItemIcon_101101.png");
	item->stat_.AttackPower = 11.0f;

	item = CreateEquipmentItem("Katana", EquipmentType::WEAPON, "ItemIcon_101101.png", ItemTier::RARE);
	item->stat_.AttackPower = 35.0f;

	item = CreateEquipmentItem("Muramasa", EquipmentType::WEAPON, "ItemIcon_101101.png", ItemTier::RARE);
	item->stat_.AttackPower = 43.0f;

	item = CreateEquipmentItem("Monohoshizao", EquipmentType::WEAPON, "ItemIcon_101101.png", ItemTier::EPIC);
	item->stat_.AttackPower = 60.0f;
	item->stat_.LifeSteel = 0.17f;

	// Axe
	item = CreateEquipmentItem("Hatchet", EquipmentType::WEAPON, "ItemIcon_101101.png");
	item->stat_.AttackPower = 18.0f;

	item = CreateEquipmentItem("BattleAxe", EquipmentType::WEAPON, "ItemIcon_101101.png", ItemTier::UNCOMMON);
	item->stat_.AttackPower = 35.0f;

	item = CreateEquipmentItem("LightHatchet", EquipmentType::WEAPON, "ItemIcon_101101.png", ItemTier::RARE);
	item->stat_.AttackPower = 44.0f;
	item->stat_.MovementSpeed = 0.04f;

	item = CreateEquipmentItem("TheJuggernaut", EquipmentType::WEAPON, "ItemIcon_101101.png", ItemTier::EPIC);
	item->stat_.AttackPower = 60.0f;
	item->stat_.MovementSpeed = 0.04f;
	item->stat_.AttackSpeed = 0.20f;

	// Dual Swords
	CreateEquipmentItem("TwinBlades", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Pistol
	item = CreateEquipmentItem("WaltherPPK", EquipmentType::WEAPON, "ItemIcon_101101.png");
	item->stat_.AttackPower = 14.0f;

	item = CreateEquipmentItem("BerettaM92F", EquipmentType::WEAPON, "ItemIcon_101101.png", ItemTier::UNCOMMON);
	item->stat_.AttackPower = 23.0f;
	item->stat_.MovementSpeed = 0.02f;

	item = CreateEquipmentItem("FN57", EquipmentType::WEAPON, "ItemIcon_101101.png", ItemTier::RARE);
	item->stat_.AttackPower = 29.0f;
	item->stat_.MovementSpeed = 0.04f;
	item->stat_.VisionRange = 100.0f;

	item = CreateEquipmentItem("Elegance", EquipmentType::WEAPON, "ItemIcon_101101.png", ItemTier::EPIC);
	item->stat_.AttackPower = 40.0f;
	item->stat_.MovementSpeed = 0.06f;
	item->stat_.VisionRange = 100.0f;
	item->stat_.SkillDamageAmplification = 48.0f;

	// Assault Rifle
	CreateEquipmentItem("Fedorova", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Sniper Rifle
	CreateEquipmentItem("LongRifle", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Rapier
	CreateEquipmentItem("Needle", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Spear
	CreateEquipmentItem("ShortSpear", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Hammer
	CreateEquipmentItem("Hammer", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Bat
	CreateEquipmentItem("ShortRod", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Throw
	CreateEquipmentItem("IronBall", EquipmentType::WEAPON, "ItemIcon_101101.png");
	CreateEquipmentItem("Baseball", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Shuriken
	CreateEquipmentItem("Razor", EquipmentType::WEAPON, "ItemIcon_101101.png");
	CreateEquipmentItem("PlayingCards", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Bow
	item = CreateEquipmentItem("Bow", EquipmentType::WEAPON, "ItemIcon_101101.png");
	item->stat_.AttackPower = 11.0f;

	item = CreateEquipmentItem("Longbow", EquipmentType::WEAPON, "ItemIcon_101101.png", ItemTier::UNCOMMON);
	item->stat_.AttackPower = 17.0f;

	item = CreateEquipmentItem("MightyBow", EquipmentType::WEAPON, "ItemIcon_101101.png", ItemTier::RARE);
	item->stat_.AttackPower = 35.0f;

	item = CreateEquipmentItem("JebesAlteredBow", EquipmentType::WEAPON, "ItemIcon_101101.png", ItemTier::EPIC);
	item->stat_.AttackPower = 62.0f;
	item->stat_.AttackSpeed = 0.33f;
	item->stat_.MovementSpeed = 0.06f;

	// Crossbow
	CreateEquipmentItem("ShortCrossbow", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Glove
	CreateEquipmentItem("BrassKnuckles", EquipmentType::WEAPON, "ItemIcon_101101.png");
	CreateEquipmentItem("CottonGloves", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Tonfa
	item = CreateEquipmentItem("Bamboo", EquipmentType::WEAPON, "ItemIcon_101101.png");
	item->stat_.AttackPower = 12.0f;

	item = CreateEquipmentItem("WoodenTonfa", EquipmentType::WEAPON, ItemTier::UNCOMMON);
	item->stat_.AttackPower = 23.0f;

	CreateEquipmentItem("PoliceBaton", EquipmentType::WEAPON, ItemTier::RARE);
	item = CreateEquipmentItem("RyukyuTonfa", EquipmentType::WEAPON, ItemTier::RARE);
	item->stat_.AttackPower = 38.0f;
	item->stat_.Defence = 3.0f;

	CreateEquipmentItem("TacticalTonfa", EquipmentType::WEAPON, ItemTier::EPIC);
	CreateEquipmentItem("MaiSok", EquipmentType::WEAPON, ItemTier::EPIC);
	item = CreateEquipmentItem("PlasmaTonfa", EquipmentType::WEAPON, ItemTier::EPIC);
	item->stat_.AttackPower = 40.0f;
	item->stat_.SkillDamageAmplification = 48.0f;
	item->stat_.Defence = 6.0f;
	item->stat_.VisionRange = 100.0f;

	// Guitar
	CreateEquipmentItem("StarterGuitar", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Nunchaku
	CreateEquipmentItem("SteelChain", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Whip
	CreateEquipmentItem("Whip", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Camera
	CreateEquipmentItem("Lens", EquipmentType::WEAPON, "ItemIcon_101101.png");

	// Arcana
	CreateEquipmentItem("GlassBead", EquipmentType::WEAPON, "ItemIcon_101101.png");
}
