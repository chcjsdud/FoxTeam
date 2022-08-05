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
	bool Iscontrolled_;

	class PlayController* PlayController_;
	class UI_Skill* SkillUI_;
	class LockOnUI* LockOnUI_;
	//class Inventory* Inventory_;

	// 플레이어 레벨업 스텟
	Status Status_LevelUp_;
	// 직업 정보
	JobType JobType_;

public:
	const bool Player_Iscontrolled()
	{
		return Iscontrolled_;
	}

	void Player_SetIscontrolled(bool _Iscontrolled)
	{
		Iscontrolled_ = _Iscontrolled;
	}

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

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

protected:
	// 캐릭터 State, 가상함수를 통해 캐릭터 클레스가 기본적으로 가지고 있어야 할 state를 지정
	virtual void Idle_Start() = 0;
	virtual void Idle_Update(float _DeltaTime) = 0;
	virtual void Idle_End() = 0;

	virtual void Walk_Start() = 0;
	virtual void Walk_Update(float _DeltaTime) = 0;
	virtual void Walk_End() = 0;

	//virtual void Run_Start() = 0;
	//virtual void Run_Update(float _DeltaTime) = 0;
	//virtual void Run_End() = 0;

	virtual void Attack_Start() = 0;
	virtual void Attack_Update(float _DeltaTime) = 0;
	virtual void Attack_End() = 0;

	// 공격과 공격 사이 에니메이션 잠깐 재생하는 구간, 공격이 완료된 직후이며, 이 단계에서 Attack를 끝낼지 게속할지 판별한다.
	virtual void Attack_Ready_Start() = 0;
	virtual void Attack_Ready_Update(float _DeltaTime) = 0;
	virtual void Attack_Ready_End() = 0;

protected:
	//void RockOnUpdate(float _DeltaTime);
	void StaminaRecoverUpdate(float _DeltaTime);

protected:
	Player(const Player& _Other) = delete;
	Player(Player&& _Other) = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) = delete;
};

