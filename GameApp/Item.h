#pragma once
#include <GameEngine/GameEngineActor.h>

enum class ItemType
{
	// 일단 만들어봄, 필요할지 안할지는 모르겠슴 지금은 지워도 무관함

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
	//밑에 std::function 실행 함수
	void Item_EquipStart(); 
	void Item_Use();
	void Item_Update(float _Time);

protected:
	// 아직 미완성 상태로, 필요에 따라 추가 제거
	std::function<void()> Equip_; //장착시 실행
	std::function<void(float)> Equip_Update_; // 장착시 업데이트
	std::function<void()> Use_; // 사용 했을때만 실행 (포션 등등)


	// ItemFBXRenderer_는 인벤토리에 들어오고 나면 나갈 일이 없는 경우를 감안해서 인벤에 넣으면 FBX 렌더러를 쥭여 버리는 것도 고민해 볼것
	// FBX 렌더러를 생성한것 만으로도 용량을 먹나?
	// FBX 렌더러를 죽여버린다면 다시 바닥에 버려질때를 감안해 FBX 생성에 필요한 정보 정도는 들고 있어야 할것
	class GameEngineFBXRenderer* ItemFBXRenderer_;

	//맵상에 뿌려졌을때 플레이어가 줍는 범위 충돌체
	class GameEngineCollision* ItemCollision_;

	//인벤토리 UI상에서 출력될 렌더러
	class GameEngineUIRenderer* ItemUIRenderer_;

	// 초기에 nullptr로 생성되고, 플레이어가 습득시 플레이어가 저장됨,  nullptr일 경우 인벤에 들어가 있지 않다는 뜻이다.
	// 인벤토리에서 버려질 경우, nullptr이 되어야 한다.
	class Player* Player_; 

	//UI 렌더링 위치와 관련해서 넣어봄 필요한진 모르겠씀
	ItemType ItemType_; 

	// 아이템의 사용 횟수,  -1로 초기화 되며, 음수일 경우, 무한정 사용 가능하다 (장비) 라는 뜻이 된다.
	int UseCount_; 



	// 아래 bool 값들은 enum class 를 통해 더 편하게 관리될 수 있을지도 모른다.

	// 플레이어가 장착 했는가
	bool IsEquip_;
	// 퀵슬릇에 넣을 수 있는가
	bool IsQuickslot_;

public:
	// 아이템 함수를 모두 초기화 하고 다시 설정
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
	
	// nullptr이 아닌것만 재설정
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

