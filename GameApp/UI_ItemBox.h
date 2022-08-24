#pragma once
#include <GameEngine/GameEngineActor.h>


using namespace std;
// ���� :
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
	
	//�ڽ��� ��ü�� �����̳Ŀ� ���� �޶����� ��
	//������ � ��� �� �̹����� ��ü�� ���� ����
	GameEngineUIRenderer* BoxtypeFont_Renderer;
	
private:
	float Time;
	bool UI_On;
	GameEngineCollision* ItemBoxCollision;

private:
	void Start() override;
	void Update(float _Time) override;

	//�ڵ����� �󽽷��� ã�Ƽ� �������� �־��ְ� �ϴ� �Լ�, �������
	void EmptySlotCheck();

	//����
private:
	Player* Player_;

#pragma region ������ ��¿� �����ۺ��̽� ����

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


#pragma region �������
	/*

	������ �ڽ��� ��ġ�� �׻� ����, ���� ������ �ڽ��� �׻� �����̴�


	�޾ƾ� �ϴ� �͵�

	*������ �ڽ��� ������ �ƴϳ��� ����

	*������ �ڽ��� ��� �ִ� �����۵��� ����
	-> ������ ���� �������� ���� �������� ����ִ��� üũ

	*������ �ڽ��� ����->�⺻����/��ü/����
	->��ü�� ��� ���ĭ 8ĭ+�κ��丮 8ĭ�ؼ� 16ĭ¥�� ������ ������ ��� �ڽ��� ����
	-> �켱���� ����, �ϴ��� 8ĭ¥�� �ʵ忡�� �����ϴ� 8ĭ ¥�� �ڽ����� ����

	*UI�� ������ �־���� ������ ���Ѽ�
	1. ������ ������ �޾Ƽ� UI���� ����Ұ� ã�Ƽ� ���ϴ� ���
	�̰��� SendItemData_Slot1(����������)
	�̷����� �Լ��� �����͸� �����ָ�
	UI���� �����͸� �޾Ƽ� �����ͳ� ����������� �̿��ؼ� ������ �ڽ��� ���


	2. UI�� �޴°� �ܼ� enum�̳� index, bool
	��� �����ʹ� ItemBoxManager�� �����ϰ�,
	UI�� ������ ���� enum/switch���� �̿��� �ܼ�ó������
	->����� �����ۺ��̽� ��ü�� �޴°� �ƴ϶�, �����鸸 �ް� �س���

	ex)
	enum class item
	{
		bottle,
		needle
	}

	PushItem_Slot1(item::bottle)
	�̷������� �־��ָ�
	enum�� �޾Ƽ� UI���� �������� ���ش�
	�̷����


	�� ���

	* UI_ItemBox�� ItemRemove() �Լ�
	�ڽ����� �����۷������� Off�Ѵ�


	* UI_Inventory�� ItemAttach() �Լ�
	�κ��丮�� ������ �������� On�Ѵ�


	* Box_to_Inventory() �Լ�?
	�ڽ����� �������� �̵�������
	�̰� ���� UI���� ���� �˰� �ϴ��� �ƴϸ� ItemBoxManager�� �����ϴ����� ����
	
	*���콺���� �浹���θ� Ȯ���Ҽ� �ִ� �浹ü �����


	
	*/
#pragma endregion 

};

