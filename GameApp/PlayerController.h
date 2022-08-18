#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include "LH_Mouse.h"

#include "ItemBase.h"

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
enum class Controller_Order
{
	None_Idle,
	A_RB_Attack_Target,
	A_Attack_Pos,
	RB_Move,
	S_Stop,
	H_Hold,
	X_Rest,
	F_ReLoad,

	Q_Skill,
	W_Skill,
	E_Skill,
	R_Skill,
	D_Weapon_Skill
};

// ���۰� ���ÿ� �ڽ��� �̸��� ���� ip ������ �����ϵ��� �ϰ�,
// ���� �ߺ��� �̸��� �����ϸ� ��������

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

#pragma region Public:
public:
	void PlayerController_SetPlayUserName(std::string _UserName)
	{
		PlayUserName_ = _UserName;
	}

	const std::string PlayerController_GetPlayUserName()
	{
		return PlayUserName_;
	}

	const Controller_Order PlayerController_GetOrder()
	{
		return Controller_Order_;
	}

	void PlayerController_SetController_Order(Controller_Order _Controller_Order)
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

	void PlayerController_SetMainPlayer(Player* _Player)
	{
		MainPlayer_ = _Player;
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

#pragma endregion
	
private:
	// ��Ʈ�ѷ��� ������ ��� ����� �����ָ�, ������ ��� ����� �޾� ���� ĳ���͵鿡�� ����� �ϴ��� ��, 
	// �� ������� Ŭ���̾�Ʈ�� ������
	// Ŭ���̾�Ʈ�� ������� �ݿ��ϰ� �ٽ� ����� ������ ������

#pragma region ��Ʈ�ѷ��� �Ѱ������ ������, ��Ʈ�ѷ��� ��������� �� 3������ �����ϱ� ���� ������
	float4 Controller_Target_Pos_; // ���콺�� Ÿ���� ���� ����
	Unit* Controller_Target_Unit_; //Ÿ���� �� ����
	Controller_Order Controller_Order_;
	//unsigned int Controller_Target_ID_;
#pragma endregion

#pragma region �÷��� ���� ���� ����
	//GameEngineFSM ServerState_; // ���� ���, ����, �÷��� ��� 

	std::string PlayUserName_; //���� �̸�
	GameEngineFSM CameraState_; // ī�޶�
	LH_Mouse* GameMouse_; // ���콺 ��ǲ
	Player* MainPlayer_; // ���� �÷��̾� ĳ����

	ItemBase* Controller_ItemList[10];
	// �������� �������� �ϴ°�
	// 1. std::fuction�� ����ȿ��, �ߵ� ȿ��
	// 2. ���� ����ü
	// 3. ������ Ÿ��
	// ����
	// ������ ���ձ��


#pragma endregion

#pragma region ���� ����
	float4 UIMousePos2D_; 
	float4 UIMousePos3D_; 

	bool Mouse_NaviCol_; // ���콺�� �׺�޽��� Ŭ�� ������
	bool Mouse_UICol_; // ���콺�� UI�� Ŭ�� ������

	bool Key_LB_;
	bool Key_RB_;

	//Order
	bool Key_A_;	// ����
	bool Key_S_;	// �̵�, ���� ����
	bool Key_H_;	// �̵� ����
	bool Key_X_;	// �޽�
	bool Key_F_;	// ������

	bool Key_Q_;
	bool Key_W_;
	bool Key_E_;
	bool Key_R_;
	bool Key_D_;	// ���� ��ų

	//UI
	bool Key_Tab_;	//���ھ� ����
	bool Key_C_;	// ĳ���� ����
	bool Key_V_;	// ���õ� Ȯ��
	bool Key_B_;	// ��� ������ Ȯ��
	bool Key_M_;	// ��ü ����
	bool Key_P_;	// ���� ��Ʈ Ȯ��
	bool Key_Y_;	// ī�޶� ���� ����

	bool Key_Esc_;	// ESC

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

#pragma endregion

private:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void PlayerController_Init();
	void PlayerController_KeyState_Init();
	void PlayerController_KeyState_Update(float _DeltaTime);
	void PlayerController_Mouse_Update();
	void PlayerController_Camera_Update(float _DeltaTime);

private:
	// ���ͳ� ���Ͽ� �߰� : ���ͺ� ����
	void CameraUpdate_EternalReturn(float _DeltaTime);

private:
	PlayerController(const PlayerController& _other) = delete;
	PlayerController(PlayerController&& _other) = delete;
	PlayerController& operator=(const PlayerController& _other) = delete;
	PlayerController& operator=(const PlayerController&& _other) = delete;
};

