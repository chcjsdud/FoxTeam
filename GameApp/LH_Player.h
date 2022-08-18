#pragma once

#include "LH_Unit.h"
/// <summary>
/// ���� ���ΰ� ����
/// </summary>


// �÷��̾�� ĳ���� �θ� Ŭ����

class GameMouse;
class GameEngineLevel;
class GameEngineImageRenderer;
class Player : public Unit
{
public:
	Player();
	~Player();

protected:

	class UI_Skill* SkillUI_;
	class LockOnUI* LockOnUI_;
	//class Inventory* Inventory_;

	// �÷��̾� ������ ����
	Status Status_LevelUp_;
	// ���� ����
	JobType JobType_;

public:
	void Player_OffUI()
	{
		//SkillUI_->Off();
		//LockOnUI_->Off();
	}

	void Player_OnUI()
	{
		//SkillUI_->On();
		//LockOnUI_->On();
	}

	void Player_SetPlayController(PlayerController* _PlayerController)
	{
		PlayerController_ = _PlayerController;
	}

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);


	void ControllerOrderUpdate();

protected:
	// ĳ���� State, �����Լ��� ���� ĳ���� Ŭ������ �⺻������ ������ �־�� �� state�� ����
	//virtual void Action_Idle_Start() = 0;
	//virtual void Action_Idle_Update(float _DeltaTime) = 0;
	//virtual void Action_Idle_End() = 0;

	//virtual void Action_Walk_Start() = 0;
	//virtual void Action_Walk_Update(float _DeltaTime) = 0;
	//virtual void Action_Walk_End() = 0;

	//virtual void Action_Attack_Start() = 0;
	//virtual void Action_Attack_Update(float _DeltaTime) = 0;
	//virtual void Action_Attack_End() = 0;

protected:
	//void RockOnUpdate(float _DeltaTime);
	void StaminaRecoverUpdate(float _DeltaTime);

protected:
	Player(const Player& _Other) = delete;
	Player(Player&& _Other) = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) = delete;
};

