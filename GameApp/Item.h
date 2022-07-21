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
	// 인벤토리에 들어오고 나면 나갈 일이 없는 경우를 감안해서 인벤에 넣으면 FBX 렌더러를 
	// 쥭여 버리는 것도 고민해 볼것

	class GameEngineFBXRenderer* ItemFBXRenderer_;
	class GameEngineCollision* ItemCollision_;
	GameEngineUIRenderer* ItemUIRenderer_;

	class Player* Player_; // null일 경우, 인벤에 들어가 있지 않다는 뜻이 되기도 한다.

	ItemType ItemType_; //UI 렌더링 위치와 관련
	//bool IsEquiped_; // 장착 여부

	int UseCount_; // -1 == 사용 횟수 무한

	std::function<void()> Equip_; //장착시 실행
	std::function<void(float)> Equip_Update_; // 장착시 업데이트
	std::function<void()> Use_; // 사용 했을때만 실행


public:
	// 모두 초기화 하고 다시 설정
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
	
	// nullptr이 아닌것만 설정
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

