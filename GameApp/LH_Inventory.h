#pragma once
#include <GameEngine/GameEngineLevel.h>


// 인벤토리 UI class
// Item List를 가지고 아이템을 관리함
// 아이템에 따라 UI 렌더링
class Item;
class Inventory : public GameEngineActor
{
	friend class GameEngineLevel;

private:	
	Inventory(); 
	~Inventory(); 
			
	Inventory(const Inventory& _other) = delete;
	Inventory(Inventory&& _other) = delete;
	Inventory& operator=(const Inventory& _other) = delete; 
	Inventory& operator=(const Inventory&& _other) = delete; 

private:
	void Start() override;
	void Update(float _Time) override;

private:
	// ItemList_를 무엇으로 구현 하는지는 UI를 만들며 생각해 보는것이 좋을것 같다.

	std::map<std::string,Item*> ItemList_;
	class Player* Player_;

private:
	void Inventory_EquipItem(std::string _Name);
	void Inventory_UseItem(std::string _Name);

	//map에서 지워버린다. 
	void Inventory_RemoveItem(std::string _Name);

public:
	// 아이템을 줏었을 경우, 해당 아이템을 insert 한다. 플레이어가 사용하게 될것
	void Inventory_Item_Loot(Item* _Item);

	void SetPlayer(Player* _Player)
	{
		Player_ = _Player;
	}
};

