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

	void Player_SetPlayController(Controller* _PlayerController)
	{
		Controller_ = _PlayerController;
	}

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);


	void ControllerOrderUpdate();

protected:
#pragma region 명령 State
	virtual void Order_Q_Start() = 0;
	virtual void Order_Q_Update(float _DeltaTime) = 0;
	virtual void Order_Q_End() = 0;

	virtual void Order_W_Start() = 0;
	virtual void Order_W_Update(float _DeltaTime) = 0;
	virtual void Order_W_End() = 0;

	virtual void Order_E_Start() = 0;
	virtual void Order_E_Update(float _DeltaTime) = 0;
	virtual void Order_E_End() = 0;

	virtual void Order_R_Start() = 0;
	virtual void Order_R_Update(float _DeltaTime) = 0;
	virtual void Order_R_End() = 0;

	virtual void Order_D_Start() = 0;
	virtual void Order_D_Update(float _DeltaTime) = 0;
	virtual void Order_D_End() = 0;
#pragma endregion
#pragma region 동작 State
	virtual void Action_Q_Start() = 0;
	virtual void Action_Q_Update(float _DeltaTime) = 0;
	virtual void Action_Q_End() = 0;

	virtual void Action_W_Start() = 0;
	virtual void Action_W_Update(float _DeltaTime) = 0;
	virtual void Action_W_End() = 0;

	virtual void Action_E_Start() = 0;
	virtual void Action_E_Update(float _DeltaTime) = 0;
	virtual void Action_E_End() = 0;

	virtual void Action_R_Start() = 0;
	virtual void Action_R_Update(float _DeltaTime) = 0;
	virtual void Action_R_End() = 0;

	virtual void Action_D_Start() = 0;
	virtual void Action_D_Update(float _DeltaTime) = 0;
	virtual void Action_D_End() = 0;
#pragma endregion
protected:
	//void RockOnUpdate(float _DeltaTime);
	void StaminaRecoverUpdate(float _DeltaTime);

protected:
	Player(const Player& _Other) = delete;
	Player(Player&& _Other) = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) = delete;
};

