#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngineBase/GameEngineRandom.h>

//#define USERSAVE

// 설명 : 
class ItemBase;
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
		return selectBox;
	}

	UI_ItemBox* GetItemBoxUI()
	{
		return itemBoxUI_;
	}

	void OpenItemBox();
	void CloseItemBox();
	ItemBase* GetItemFromItemBox(int _index);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void CreateItemBoxInfo(const std::string& _Name);
	void BoxSelectUpdate();
	void DebugRender();

	// Item 관련
	void CreateAllItemList();
	ItemBase* CreateItem(const std::string _Name, ItemType _Type,
		const std::string _ImageName = "");

	void PushRandomItem(const std::string& _area, const std::string& _item, int _amount = 1);

private:
	std::string itemBoxInfoPath;
	std::map<std::string, std::vector<ItemBox*>> itemBoxs;
	ItemBox* selectBox;

	std::list<ItemBase*> allItemList;
	GameEngineRandom randomManager;

	//이건호 UI아이템 박스 작업
private:
	UI_ItemBox* itemBoxUI_;
};

