#include "PreCompile.h"
#include "LH_Inventory.h"
#include "Item.h"

Inventory::Inventory() // default constructer 디폴트 생성자
{
}

Inventory::~Inventory() // default destructer 디폴트 소멸자
{
	auto iter0 = ItemList_.begin();
	auto iter1 = ItemList_.end();

	for (;iter0 != iter1;)
	{
		(*iter0).second->Death();
		//ItemList_.erase(iter0);

		iter0++;
	}
	ItemList_.clear();
}

void Inventory::Start()
{
}

void Inventory::Update(float _Time)
{
	auto iter0 = ItemList_.begin();
	auto iter1 = ItemList_.end();

	for (; iter0 != iter1;)
	{
		if (false == (*iter0).second->IsUpdate())
		{
			continue;
		}

		if (true == (*iter0).second->IsDeath())
		{
			iter0 = ItemList_.erase(iter0);

			continue;
		}

		if (false == (*iter0).second->Item_GetEquipState())
		{
			// 장착하지 않았으면
			continue;
		}

		if (true == (*iter0).second->IsEquip_)
		{
			//장착 하고 있으면 업데이트
			(*iter0).second->Equip_Update_(_Time);
		}

		iter0++;
	}
}

void Inventory::Inventory_EquipItem(std::string _Name)
{
	ItemList_.find(_Name)->second->Item_EquipStart();
}

void Inventory::Inventory_UseItem(std::string _Name)
{
	ItemList_.find(_Name)->second->Item_Use();

	//1회용인지 등등 검사해서 지우던 말던 결정
}

void Inventory::Inventory_RemoveItem(std::string _Name)
{
	std::map<std::string, Item*>::iterator  iter0 = ItemList_.find(_Name);
	iter0->second->Death();
	ItemList_.erase(iter0);

	//UI구현 상태에 따라서 정렬 기능같은걸 추가적으로 넣어두기
}

void Inventory::Inventory_Item_Loot(Item* _Item)
{
	_Item->SetPlayer(Player_);
	ItemList_.insert(std::make_pair(_Item->GetName(), _Item));
}


