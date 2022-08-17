#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include "LH_Mouse.h"

/*
�ϴ��� : �ÿ��̾� ���� ȭ���� ������ Ŭ����,
		1. UI ǥ�ÿ� �÷��̾� ĳ���� ���ۿ� ���� ���
		2. ��� �÷��̾ ������ ����, ����Ī�ϸ� ��Ʈ�� �� ����� ������ �� ����
		3. �÷��̾��� Ű ���۰� ����� ������ �ִ� �÷��̾�� �ϴ��ϴ� �߰� ����
		4. ��� -> �÷��̾� ��Ʈ�ѷ� -> �÷��̾�

		5. ��Ʈ�� ���ۿ� ���� �߻��ϴ°͵��� Player class�� ��������
		���콺 ��ǥ, ������ ����, Ű ���� ���


		������ ����Ʈ�� ���ֿ� ��ɸ� ����
*/
enum class Order
{
	None,
	Attack_Target,
	Attack_Pos,
	Move,
	Stop,
	Hold,
	Pattrol,

	Q,
	W,
	E,
	R
};

enum class ControllerkeyState
{
	None,
	A,
	S,
	M,
	H,
	P,

	Q,
	W,
	E,
	R
};

class Player;
class Unit;
class GameMouse;
class PlayerController : public GameEngineActor
{
	friend Player;
	friend GameMouse;

public:
	PlayerController(); // default constructer ����Ʈ ������
	~PlayerController(); // default destructer ����Ʈ �Ҹ���

	
private:
#pragma region ��Ʈ�ѷ��� �Ѱ������ ������, ��Ʈ�ѷ��� ��������� �� 3������ �����ϱ� ���� ������
	float4 Controller_Target_Pos_; // ���콺�� Ÿ���� ���� ����
	Unit* Controller_Target_Unit_; //Ÿ���� �� ����
	//unsigned int Controller_Target_ID_;
	Order Controller_Order_;

#pragma endregion

#pragma region ��Ʈ�ѷ��� �Ѱ������ ������, ��Ʈ�ѷ��� ��������� �� 3������ �����ϱ� ���� ������
#pragma endregion
public:
	void SetPlayUserName(std::string _UserName)
	{
		PlayUserName_ = _UserName;
	}

	const std::string GetPlayUserName()
	{
		return PlayUserName_;
	}

	const Order PlayerController_GetOrder()
	{
		return Controller_Order_;
	}

	void SetController_Order(Order _Controller_Order)
	{
		Controller_Order_ = _Controller_Order;
	}
	Unit* PlayerController_GetTargetUnit()
	{
		return Controller_Target_Unit_;
	}

	const float4 PlayerController_GetTarget_Pos()
	{
		return Controller_Target_Pos_;
	}

	void SetMainActor(Player* _Actor)
	{
		MainPlayer_ = _Actor;
	}

	//unsigned int GetPlayerID_()
	//{
	//	return CurPlayerUnitID_;
	//}

	//const unsigned int PlayerController_GetTarget_ID()
	//{
	//	return Controller_Target_ID_;
	//}

	//void SetMainPlayerID_(unsigned int _PlayerID)
	//{
	//	CurPlayerUnitID_ = _PlayerID;
	//}


private:
	std::string PlayUserName_;

	ControllerkeyState ControllerkeyState_;
	//unsigned int CurPlayerUnitID_;
	LH_Mouse* GameMouse_;

	GameEngineFSM CameraState_;

	Player* MainPlayer_;

	// �ӽ� bool �ڵ� 
#pragma region ���� ����

	float4 UIMousePos2D_; 
	float4 UIMousePos3D_; 

	//bool IsAttack_;
	//bool IsMove_;

	bool Mouse_NaviCol_; // ���콺�� �׺�޽��� Ŭ�� ������
	bool Mouse_UICol_; // ���콺�� UI�� Ŭ�� ������

	bool Key_LB_;
	bool Key_RB_;

	bool Key_A_;	// ����
	bool Key_S_;	// �̵�, ���� ����
	bool Key_H_;	// �̵� ����

	bool Key_X_;	// �޽�
	bool Key_F_;	// ������
	bool Key_Tab_;	//���ھ� ����
	bool Key_Esc_;	// ESC
	bool Key_C_;	// ĳ���� ����
	bool Key_V_;	// ���õ� Ȯ��
	bool Key_B_;	// ��� ������ Ȯ��
	bool Key_M_;	// ��ü ����
	bool Key_P_;	// ���� ��Ʈ Ȯ��
	bool Key_Y_;	// ī�޶� ���� ����

	//������
	bool Key_1_;
	bool Key_2_;
	bool Key_3_;
	bool Key_4_;
	bool Key_5_;
	bool Key_6_;
	bool Key_7_;
	bool Key_8_;
	bool Key_9_;
	bool Key_0_;

	bool Key_Q_;
	bool Key_W_;
	bool Key_E_;
	bool Key_R_;
	bool Key_D_;	// ���� ��ų

#pragma endregion


private:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void ControllerInit();
	void InitKeyState();
	void MouseUpdate();
	void KeyStateUpdate(float _DeltaTime);
	void CameraUpdate(float _DeltaTime);

private:
	// ���ͳ� ���Ͽ� �߰� : ���ͺ� ����
	void CameraUpdate_EternalReturn(float _DeltaTime);

private:
	PlayerController(const PlayerController& _other) = delete;
	PlayerController(PlayerController&& _other) = delete;
	PlayerController& operator=(const PlayerController& _other) = delete;
	PlayerController& operator=(const PlayerController&& _other) = delete;
};

