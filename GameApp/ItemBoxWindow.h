#pragma once
#include <GameEngine/GameEngineGUI.h>

// Ό³Έν : 
class ItemBoxManager;
class ItemBox;
class ItemBase;
class UI_ItemBox;
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
	//void PushItem();
	//void DeleteItem();

public:
	ItemBoxManager* ItemBoxManager_;
	ItemBox* SelectBox_;
	UI_ItemBox* BoxUI_;
	int ListSelectItem;
	int ListSelectItemBox;
	
	//std::list<ItemBase*> AllItemList;
	//std::vector<const char*> AllItemName;
};

