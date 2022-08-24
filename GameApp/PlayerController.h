#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include "Controller.h"

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

		������ �����ϰ� ���������� �������� ������ �ɰ�
*/

// ���۰� ���ÿ� �ڽ��� �̸��� ���� ip ������ �����ϵ��� �ϰ�,
// ���� �ߺ��� �̸��� �����ϸ� ��������

class Player;
class Unit;
class GameMouse;
//class PlayerController : public GameEngineActor
class PlayerController : public Controller
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

	void PlayerController_SetMainPlayer(Player* _Player)
	{
		MainPlayer_ = _Player;
	}
#pragma endregion
	
private:
	// ��Ʈ�ѷ��� ������ ��� ����� �����ָ�, ������ ��� ����� �޾� ���� ĳ���͵鿡�� ����� �ϴ��� ��, 
	// �� ������� Ŭ���̾�Ʈ�� ������
	// Ŭ���̾�Ʈ�� ������� �ݿ��ϰ� �ٽ� ����� ������ ������
#pragma region �÷��� ���� ���� ����
	//GameEngineFSM ServerState_; // ���� ���, ����, �÷��� ��� 

	std::string PlayUserName_; //���� �̸�
	GameEngineFSM CameraState_; // ī�޶�
	LH_Mouse* GameMouse_; // ���콺 ��ǲ

	Player* MainPlayer_; // ���� �÷��̾� ĳ���� //���� pc���� ���� ������ �Ǵ� ������ ��

	//unsigned int MainPlayerID_; // ���� �÷��̾� ĳ���� ID ���� ��ü�� �����ϴ� ID��

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
	bool Key_Num_[10];

	class PlayerControllerUpdatePaket // �ӽ� ���� ����
	{
		friend PlayerController;

		std::string PlayUserName_; //���� �̸�

		float4 Controller_Target_Pos_; // ���콺�� Ÿ���� ���� ����
		//Unit* Controller_Target_Unit_; //Ÿ���� �� ����
		unsigned int Controller_Target_UnitID_; //Ÿ���� �� ���� ID

		Controller_Order Controller_Order_; //������ �÷��̾� ĳ������ ����� �������ָ� �������� ó���ؼ� ���� ������� �ݿ��ǰ� �ɰ�

		bool Key_Num_[10];
	};

	PlayerControllerUpdatePaket* PlayerControllerUpdatePaket_;

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

