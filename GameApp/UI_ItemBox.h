#pragma once
#include <GameEngine/GameEngineActor.h>


using namespace std;
// 설명 :
class GameEngineUIRenderer;
class Player;
class ItemBase;

class UI_ItemBox : public GameEngineActor
{
public:
	// constrcuter destructer
	UI_ItemBox();
	~UI_ItemBox();

	// delete Function
	UI_ItemBox(const UI_ItemBox& _Other) = delete;
	UI_ItemBox(UI_ItemBox&& _Other) noexcept = delete;
	UI_ItemBox& operator=(const UI_ItemBox& _Other) = delete;
	UI_ItemBox& operator=(UI_ItemBox&& _Other) noexcept = delete;




public:
	void SetPlayer(Player* _Player)
	{
		Player_ = _Player;
	}

	void RenderOff();
	
	void RenderOn();

	bool MouseCollisionCheck();

	void PushItem(ItemBase* _OriginItemBase, int _SlotNumber = 0);

protected:
	GameEngineUIRenderer* ItemBoxBackGround_Renderer;
	
	//박스냐 시체냐 보급이냐에 따라 달라지는 것
	//종류가 몇개 없어서 통 이미지로 대체될 수도 있음
	GameEngineUIRenderer* BoxtypeFont_Renderer;
	
private:
	float Time;
	bool UI_On;
	GameEngineCollision* ItemBoxCollision;

private:
	void Start() override;
	void Update(float _Time) override;

	//자동으로 빈슬롯을 찾아서 아이템을 넣어주게 하는 함수, 만드는중
	void EmptySlotCheck();

	//이현
private:
	Player* Player_;

#pragma region 아이템 출력용 아이템베이스 모음

	ItemBase* Slot0_Item;
	ItemBase* Slot1_Item;
	ItemBase* Slot2_Item;
	ItemBase* Slot3_Item;
	ItemBase* Slot4_Item;
	ItemBase* Slot5_Item;
	ItemBase* Slot6_Item;
	ItemBase* Slot7_Item;
//	GameEngineUIRenderer* DefenseValue_Renderer;
//	GameEngineUIRenderer* AttackSpeedValue_Renderer;
//	GameEngineUIRenderer* MoveSpeedValue_Renderer;
//	GameEngineUIRenderer* CriticalValue_Renderer;

#pragma endregion 


#pragma region 참고사항
	/*

	아이템 박스의 위치는 항상 고정, 같은 종류의 박스면 항상 고정이다


	받아야 하는 것들

	*아이템 박스를 열었냐 아니냐의 여부

	*아이템 박스에 들어 있는 아이템들의 정보
	-> 정보를 토대로 아이템이 무슨 아이템이 들어있는지 체크

	*아이템 박스의 종류->기본상자/시체/보급
	->시체의 경우 장비칸 8칸+인벤토리 8칸해서 16칸짜리 아이템 정보가 담긴 박스가 나옴
	-> 우선순위 낮음, 일단은 8칸짜리 필드에서 루팅하는 8칸 짜리 박스부터 제작

	*UI가 가지고 있어야할 정보의 상한선
	1. 아이템 정보를 받아서 UI에서 출력할걸 찾아서 정하는 경우
	이경우는 SendItemData_Slot1(아이템정보)
	이런식의 함수로 데이터를 보내주면
	UI에서 데이터를 받아서 데이터내 멤버변수들을 이용해서 아이템 박스에 출력


	2. UI가 받는건 단순 enum이나 index, bool
	모든 데이터는 ItemBoxManager가 관리하고,
	UI가 가진건 오직 enum/switch등을 이용한 단순처리과정
	->현재는 아이템베이스 자체를 받는게 아니라, 정보들만 받게 해놨음

	ex)
	enum class item
	{
		bottle,
		needle
	}

	PushItem_Slot1(item::bottle)
	이런식으로 넣어주면
	enum을 받아서 UI에서 렌더링을 해준다
	이런방식


	들어갈 기능

	* UI_ItemBox의 ItemRemove() 함수
	박스에서 아이템렌더러를 Off한다


	* UI_Inventory의 ItemAttach() 함수
	인벤토리에 아이템 렌더러를 On한다


	* Box_to_Inventory() 함수?
	박스에서 아이템이 이동햇을때
	이건 이제 UI끼리 서로 알게 하느냐 아니면 ItemBoxManager가 관리하느냐의 여부
	
	*마우스와의 충돌여부를 확인할수 있는 충돌체 만들기


	
	*/
#pragma endregion 

};

