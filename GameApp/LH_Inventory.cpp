#include "PreCompile.h"
#include "LH_Inventory.h"
#include "Item.h"

Inventory::Inventory() // default constructer ����Ʈ ������
{
}

Inventory::~Inventory() // default destructer ����Ʈ �Ҹ���
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
			// �������� �ʾ�����
			continue;
		}

		if (true == (*iter0).second->IsEquip_)
		{
			//���� �ϰ� ������ ������Ʈ
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

	//1ȸ������ ��� �˻��ؼ� ����� ���� ����
}

void Inventory::Inventory_RemoveItem(std::string _Name)
{
	std::map<std::string, Item*>::iterator  iter0 = ItemList_.find(_Name);
	iter0->second->Death();
	ItemList_.erase(iter0);

	//UI���� ���¿� ���� ���� ��ɰ����� �߰������� �־�α�
}

void Inventory::Inventory_Item_Loot(Item* _Item)
{
	_Item->SetPlayer(Player_);
	ItemList_.insert(std::make_pair(_Item->GetName(), _Item));
}


