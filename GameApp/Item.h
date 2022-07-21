#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineUIRenderer.h>

enum class ItemType
{
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
	void Item_EquipStart();
	void Item_Use();
	void Item_Update(float _Time);

protected:
	// �κ��丮�� ������ ���� ���� ���� ���� ��츦 �����ؼ� �κ��� ������ FBX �������� 
	// ���� ������ �͵� ����� ����

	class GameEngineFBXRenderer* ItemFBXRenderer_;
	class GameEngineCollision* ItemCollision_;
	GameEngineUIRenderer* ItemUIRenderer_;

	class Player* Player_; // null�� ���, �κ��� �� ���� �ʴٴ� ���� �Ǳ⵵ �Ѵ�.

	ItemType ItemType_; //UI ������ ��ġ�� ����
	//bool IsEquiped_; // ���� ����

	int UseCount_; // -1 == ��� Ƚ�� ����

	std::function<void()> Equip_; //������ ����
	std::function<void(float)> Equip_Update_; // ������ ������Ʈ
	std::function<void()> Use_; // ��� �������� ����


public:
	// ��� �ʱ�ȭ �ϰ� �ٽ� ����
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
	
	// nullptr�� �ƴѰ͸� ����
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
		return Player_ != nullptr;
	}

	void SetPlayer(Player* _Player)
	{
		Player_ = _Player;
	}

private:
};

