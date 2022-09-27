#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngineBase/GameEngineRandom.h>

//#define USERSAVE

// 설명 : 
class ItemBase;
class MiscItem;
class UseableItem;
class EquipmentItem;
class ItemBox;
class UI_ItemBox;
class GameEngineFBXRenderer;
class ItemBoxManager : public GameEngineActor
{
public:
	ItemBoxManager();
	~ItemBoxManager();

	ItemBoxManager(const ItemBoxManager& _other) = delete; 
	ItemBoxManager(ItemBoxManager&& _other) noexcept = delete;
	ItemBoxManager& operator=(const ItemBoxManager& _other) = delete;
	ItemBoxManager& operator=(const ItemBoxManager&& _other) = delete;

	void UserSave(const std::string& _Path);
	void UserLoad(const std::string& _Path);
	void UserAllLoad(GameEngineDirectory _Dir);

	void UserSave_ItemListInfo();
	void UserLoad_ItemListInfo();

	ItemBox* GetSelectBox()
	{
		return selectBox_;
	}

	UI_ItemBox* GetItemBoxUI()
	{
		return itemBoxUI_;
	}

	bool isOpen()
	{
		return bOpen_;
	}

	void OpenItemBox();
	void CloseItemBox();
	ItemBase* GetItemFromItemBox(int _index);
	void DeleteItemFromItemBox(int _index);

	void PushRandomItem(const std::string& _area, const std::string& _item, int _amount = 1);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void CreateItemBoxInfo(const std::string& _Name);
	void BoxSelectUpdate();
	void DebugRender();

	// Item 관련
	void CreateAllItemList();
	void CreateMiscItemList();
	void CreateUseableItemList();
	void CreateEquipmentItemList();
	void CreateWeaponItemList();

	ItemBase* CreateItem(const std::string _Name, ItemType _Type,
		const std::string _ImageName = "");

	MiscItem* CreateMiscItem(const std::string _Name,
		const std::string _ImageName = "");
	UseableItem* CreateUseableItem(const std::string _Name,
		const std::string _ImageName = "");
	EquipmentItem* CreateEquipmentItem(const std::string _Name, 
		EquipmentType _EquipType = EquipmentType::NONE, 
		const std::string _ImageName = "");

private:
	std::string itemBoxInfoPath_;
	std::map<std::string, std::vector<ItemBox*>> itemBoxs_;
	ItemBox* selectBox_;
	bool bOpen_;

	std::list<ItemBase*> allItemList_;
	GameEngineRandom randomManager_;

	//이건호 UI아이템 박스 작업
private:
	UI_ItemBox* itemBoxUI_;
};

