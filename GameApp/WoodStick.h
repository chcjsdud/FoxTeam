#pragma once
#include "Item.h"
// �з� : 
// �뵵 : 
// ���� : 

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

// // �θ� class �ɹ� ����
//protected: 
//	GameEngineUIRenderer* ItemRenderer_;
//	ItemType ItemType_; //UI ������ ��ġ�� ����
//	bool IsEquiped_; // ���� ����
//	std::function<void()> Equip_; //������ ����
//	std::function<void(float)> Equip_Update_; // ������ ������Ʈ
//	std::function<void()> Use_; // ��� �������� ����

};
