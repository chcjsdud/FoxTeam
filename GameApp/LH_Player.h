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

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

protected:
	// ĳ���� State, �����Լ��� ���� ĳ���� Ŭ������ �⺻������ ������ �־�� �� state�� ����
	virtual void Idle_Start() = 0;
	virtual void Idle_Update(float _DeltaTime) = 0;
	virtual void Idle_End() = 0;

	virtual void Walk_Start() = 0;
	virtual void Walk_Update(float _DeltaTime) = 0;
	virtual void Walk_End() = 0;

	virtual void Run_Start() = 0;
	virtual void Run_Update(float _DeltaTime) = 0;
	virtual void Run_End() = 0;

	virtual void Attack_Start() = 0;
	virtual void Attack_Update(float _DeltaTime) = 0;
	virtual void Attack_End() = 0;

protected:
	void RockOnUpdate(float _DeltaTime);
	void StaminaRecoverUpdate(float _DeltaTime);

protected:
	Player(const Player& _Other) = delete;
	Player(Player&& _Other) = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) = delete;
};

