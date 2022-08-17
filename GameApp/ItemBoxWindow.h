#pragma once
#include <GameEngine/GameEngineGUI.h>

// Ό³Έν : 
class ItemBoxManager;
class ItemBox;
class ItemBase;
class ItemBoxWindow : public GameEngineGUIWindow
{
public:
	ItemBoxWindow();
	~ItemBoxWindow();
	ItemBoxWindow(const ItemBoxWindow& _other) = delete; 
	ItemBoxWindow(ItemBoxWindow&& _other) noexcept = delete;
	ItemBoxWindow& operator=(const ItemBoxWindow& _other) = delete;
	ItemBoxWindow& operator=(const ItemBoxWindow&& _other) = delete;


protected:
	void OnGUI() override;

private:
	void PushItem();
	void DeleteItem();

	void CreateAllItemList();
	ItemBase* CreateItem(const std::string _Name, ItemType _Type,
		const std::string _ImageName = "");

public:
	ItemBoxManager* ItemBoxManager_;
	ItemBox* SelectBox_;
	int ListSelectItem;
	int ListSelectItemBox;
	
	std::list<ItemBase*> AllItemList;
	std::vector<const char*> AllItemName;
};

