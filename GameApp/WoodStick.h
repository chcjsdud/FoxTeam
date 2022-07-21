#pragma once
#include "Item.h"
// 분류 : 
// 용도 : 
// 설명 : 

class Inventory;
class WoodStick : public Item
{
	friend class Inventory;
protected:
	WoodStick();
	~WoodStick();

	WoodStick(const WoodStick& _other) = delete;
	WoodStick(WoodStick&& _other) = delete;
	WoodStick& operator=(const WoodStick& _other) = delete;
	WoodStick& operator=(const WoodStick&& _other) = delete;

protected:
	virtual void Start();
	virtual void Update(float _Time);

// // 부모 class 맴버 변수
//protected: 
//	GameEngineUIRenderer* ItemRenderer_;
//	ItemType ItemType_; //UI 렌더링 위치와 관련
//	bool IsEquiped_; // 장착 여부
//	std::function<void()> Equip_; //장착시 실행
//	std::function<void(float)> Equip_Update_; // 장착시 업데이트
//	std::function<void()> Use_; // 사용 했을때만 실행

};
