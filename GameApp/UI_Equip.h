#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;
// 설명 :
class GameEngineUIRenderer;
class ItemBase;
class ItemBox;

class UI_Equip : public GameEngineActor
{
public:
	// constrcuter destructer
	UI_Equip();
	~UI_Equip();

	// delete Function
	UI_Equip(const UI_Equip& _Other) = delete;
	UI_Equip(UI_Equip&& _Other) noexcept = delete;
	UI_Equip& operator=(const UI_Equip& _Other) = delete;
	UI_Equip& operator=(UI_Equip&& _Other) noexcept = delete;

	bool UIOn;

public:

	void EmptySlot();
	void PushItem(ItemBase* _OriginItemBase, int _SlotNumber = 0);
	void GetInventoryInfo(vector<ItemBase*> _ItemVector);

	//슬롯에 아이템이 들어있을때 이미지 렌더를 위한 함수
	void ItemRenderCheck();

protected:
	GameEngineUIRenderer* Renderer;
	float Time;


private:
	void Start() override;
	void Update(float _Time) override;

	//자동으로 빈슬롯을 찾아서 ItemBase를 세팅해주는 함수
	void EmptySlotReturn(ItemBase* _TargetSlot);
	
};

