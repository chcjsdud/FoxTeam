#pragma once
#include <GameEngine/GameEngineActor.h>

enum class ItemType
{
	// �ϴ� ����, �ʿ����� �������� �𸣰ڽ� ������ ������ ������

	None,
	Equipment,
	Useable,
	Misc
};

class Inventory;
class Item : public GameEngineActor
{
	friend class Inventory;
protected:		
	Item();
	~Item(); 
		
	Item(const Item& _other) = delete;
	Item(Item&& _other) = delete; 
	Item& operator=(const Item& _other) = delete; 
	Item& operator=(const Item&& _other) = delete;

protected:
	virtual void Start();
	virtual void Update(float _Time);

protected:
	//�ؿ� std::function ���� �Լ�
	void Item_EquipStart(); 
	void Item_Use();
	void Item_Update(float _Time);

protected:
	// ���� �̿ϼ� ���·�, �ʿ信 ���� �߰� ����
	std::function<void()> Equip_; //������ ����
	std::function<void(float)> Equip_Update_; // ������ ������Ʈ
	std::function<void()> Use_; // ��� �������� ���� (���� ���)


	// ItemFBXRenderer_�� �κ��丮�� ������ ���� ���� ���� ���� ��츦 �����ؼ� �κ��� ������ FBX �������� ���� ������ �͵� ����� ����
	// FBX �������� �����Ѱ� �����ε� �뷮�� �Գ�?
	// FBX �������� �׿������ٸ� �ٽ� �ٴڿ� ���������� ������ FBX ������ �ʿ��� ���� ������ ��� �־�� �Ұ�
	class GameEngineFBXRenderer* ItemFBXRenderer_;

	//�ʻ� �ѷ������� �÷��̾ �ݴ� ���� �浹ü
	class GameEngineCollision* ItemCollision_;

	//�κ��丮 UI�󿡼� ��µ� ������
	class GameEngineUIRenderer* ItemUIRenderer_;

	// �ʱ⿡ nullptr�� �����ǰ�, �÷��̾ ����� �÷��̾ �����,  nullptr�� ��� �κ��� �� ���� �ʴٴ� ���̴�.
	// �κ��丮���� ������ ���, nullptr�� �Ǿ�� �Ѵ�.
	class Player* Player_; 

	//UI ������ ��ġ�� �����ؼ� �־ �ʿ����� �𸣰ھ�
	ItemType ItemType_; 

	// �������� ��� Ƚ��,  -1�� �ʱ�ȭ �Ǹ�, ������ ���, ������ ��� �����ϴ� (���) ��� ���� �ȴ�.
	int UseCount_; 



	// �Ʒ� bool ������ enum class �� ���� �� ���ϰ� ������ �� �������� �𸥴�.

	// �÷��̾ ���� �ߴ°�
	bool IsEquip_;
	// �������� ���� �� �ִ°�
	bool IsQuickslot_;

public:
	// ������ �Լ��� ��� �ʱ�ȭ �ϰ� �ٽ� ����
	template <typename ItemClass>
	void Item_SetItemFunc(ItemClass *ptr,void (ItemClass::* _Equip)(), void (ItemClass::* _Equip_Update)(float), void (ItemClass::* _Use)())
	{
		Equip_ = nullptr;
		Equip_Update_ = nullptr;
		Use_ = nullptr;

		if (_Equip != nullptr)
		{
			Equip_ = std::bind(_Equip, ptr);
		}

		if (_Equip_Update != nullptr)
		{
			Equip_Update_ = std::bind(_Equip_Update, ptr, std::placeholders::_1);
		}

		if (_Use != nullptr)
		{
			Use_ = std::bind(_Use, ptr);
		}
	}
	
	// nullptr�� �ƴѰ͸� �缳��
	template <typename ItemClass>
	void Item_SetChangeItemFunc(ItemClass* ptr,void (ItemClass::* _Equip)(), void (ItemClass::* _Equip_Update)(float), void (ItemClass::* _Use)())
	{
		if (_Equip != nullptr)
		{
			Equip_ = std::bind(_Equip, ptr);
		}

		if (_Equip_Update != nullptr)
		{
			Equip_Update_ = std::bind(_Equip_Update, ptr, std::placeholders::_1);
		}

		if (_Use != nullptr)
		{
			Use_ = std::bind(_Use, ptr);
		}
	}

	void Item_SetItemType(ItemType _ItemType)
	{
		ItemType_ = _ItemType;
	}

	GameEngineUIRenderer* Item_GetUIRenderer()
	{
		return ItemUIRenderer_;
	}

	const bool Item_GetEquipState()
	{
		return IsEquip_;
	}

	const bool Item_IsInventory()
	{
		return Player_ != nullptr;
	}

	void SetPlayer(Player* _Player)
	{
		Player_ = _Player;
	}

private:

	//void Item_Loot();
};

