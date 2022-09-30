#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngineBase/GameEngineRandom.h>

//#define USERSAVE

struct CombineItem
{
public:
	CombineItem(const std::string& _left, const std::string& _right)
	{
		if (_left > _right)
		{
			left_ = _right;
			right_ = _left;
		}
		else if(_left < _right)
		{
			left_ = _left;
			right_ = _right;
		}
		else
		{
			GameEngineDebug::MsgBoxError("조합식이 잘못되었습니다.");
		}
	}

	bool const operator==(const CombineItem& o)
	{
		return left_ == o.left_ && right_ == o.right_;
	}

	bool const operator<(const CombineItem& o) const
	{
		if (left_ != o.left_)
		{
			return left_ < o.left_;
		}

		if (right_ != o.right_)
		{
			return right_ < o.right_;
		}

		return false;
	}

private:
	std::string left_;
	std::string right_;

	// 정렬을 위해서 막음
	CombineItem() {}
};

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

	ItemBase* CreateItem(const std::string _Name, ItemType _Type, ItemTier _Tier,
		const std::string _ImageName = "");

	// _ImageName 을 지정해주지 않는다면 _Name + ".png" 로 지정
	MiscItem* CreateMiscItem(const std::string _Name, 
		const std::string _ImageName, ItemTier _Tier = ItemTier::COMMON);
	MiscItem* CreateMiscItem(const std::string _Name, 
		ItemTier _Tier = ItemTier::COMMON);

	UseableItem* CreateUseableItem(const std::string _Name,
		const std::string _ImageName, ItemTier _Tier = ItemTier::COMMON);
	UseableItem* CreateUseableItem(const std::string _Name,
		ItemTier _Tier = ItemTier::COMMON);

	EquipmentItem* CreateEquipmentItem(const std::string _Name,
		EquipmentType _EquipType, 
		const std::string _ImageName, ItemTier _Tier = ItemTier::COMMON);
	EquipmentItem* CreateEquipmentItem(const std::string _Name,
		EquipmentType _EquipType, 
		ItemTier _Tier = ItemTier::COMMON);

	void CreateAllItemRecipes();
	void CreateItemRecipe(const std::string& _left, const std::string& _right,
		const std::string& _result);

private:
	std::string itemBoxInfoPath_;
	std::map<std::string, std::vector<ItemBox*>> itemBoxs_;
	ItemBox* selectBox_;
	bool bOpen_;

	std::list<ItemBase*> allItemList_;
	GameEngineRandom randomManager_;
	std::map<CombineItem, std::string> itemRecipes_;

	//이건호 UI아이템 박스 작업
private:
	UI_ItemBox* itemBoxUI_;
};

