#pragma once

#include "LH_Unit.h"
/// <summary>
/// 여우 주인공 액터
/// </summary>


// 플레이어블 캐릭터 부모 클레스

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

	// 플레이어 레벨업 스텟
	Status Status_LevelUp_;
	// 직업 정보
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
	// 캐릭터 State, 가상함수를 통해 캐릭터 클레스가 기본적으로 가지고 있어야 할 state를 지정
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

