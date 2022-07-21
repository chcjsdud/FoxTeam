#pragma once
#include <GameEngine/GameEngineLevel.h>


// �κ��丮 UI class
// Item List�� ������ �������� ������
// �����ۿ� ���� UI ������
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
	// ItemList_�� �������� ���� �ϴ����� UI�� ����� ������ ���°��� ������ ����.

	std::map<std::string,Item*> ItemList_;
	class Player* Player_;

private:
	void Inventory_EquipItem(std::string _Name);
	void Inventory_UseItem(std::string _Name);

	void Inventory_RemoveItem(std::string _Name);
public:

	template <typename ItemType>
	ItemType* Inventory_Newitem(std::string _Name)
	{
		ItemType* newItem = GetLevel()->CreateActor<ItemType>();

		newItem->SetName(_Name);
		newItem->SetPlayer(Player_);

		ItemList_.insert(std::make_pair(_Name, newItem));
	}

	void SetPlayer(Player* _Player)
	{
		Player_ = _Player;
	}
};

