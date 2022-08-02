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

	Player(const Player& _Other) = delete;
	Player(Player&& _Other) noexcept = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) noexcept = delete;

private:
	// �ν��ؾ� �ϴ� ���콺 ���� ������
	GameMouse* targetMouse_;
	bool IsRockon_;
	// ���ͳ� ���� �߰� ����
	float4 targetPos_;
	// �ϴ޹��� ���콺 Ŀ�� ��Ŭ�� ��ǥ
	float4 arrivalPos_;
	// ���� ���� ��ǥ

protected:
	//UI
	bool IsPlayer_;

	class UI_Skill* SkillUI_;
	class LockOnUI* LockOnUI_;
	//class Inventory* Inventory_;
#pragma region Player Status

	// �÷��̾� ������ ����
	Status Status_LevelUp_;
	// ���� ����
	JobType JobType_;

#pragma endregion

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

#pragma region State
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

	//void Stand_Start();
	//void Stand_Update(float _DeltaTime);
	//void Stand_End();

	//void Move_Start();
	//void Move_Update(float _DeltaTime);
	//void Move_End();

#pragma endregion

#pragma region MemberUpdateFunc
protected:

	// update�� �Լ��� ���� �� ī�޶� ������Ʈ, �ٷ� �ڿ��� �ٶ󺸴� �Լ�
	void CameraUpdate_BackPosition(float _DeltaTime);
	void CameraUpdate_UpPosition(float _DeltaTime);

	// ���ͳ� ���Ͽ� �߰� : ���ͺ� ����
	void CameraUpdate_EternalReturn(float _DeltaTime);


	//ĳ���Ͱ� �ٶ󺸴� ������ �ٷ� �ٷ� �Ⱥ��ϰ� õõ�� ����(�̵����� �����ϰ�), 
	//07.18 �ٶ󺸴� ������ �־������
	void MoveRotateUpdate(float _DeltaTime);

	//RockOn ���϶� ���� ������Ʈ
	void RockOnDirUpdate(float _DeltaTime);


	void KeyDirUpdate(float _DeltaTime);
	// update�� �Լ��� ���� �� �̵��� ������
	void MoveUpdate(float _DeltaTime);
	void CurDirUpdate(float _DeltaTime);
	void RockOnUpdate(float _DeltaTime);

	void StaminaRecoverUpdate(float _DeltaTime);

#pragma endregion

#pragma region MemberFunc
private:
	const float4 CalculateTargetDir(float4 TargetPos)
	{
		float4 Dir = TargetPos - GetTransform()->GetWorldPosition();
		Dir.Normalize3D();

		return Dir;
	}

	//�ӽ� ���� ����, �κ��丮���� �������� ��������, �����ۿ��� �̸��� �������ͽ��� �������Բ�
	//bool EquipItem(std::string _BuffName, PlayerStatus* _PlayerStatus);

#pragma endregion

private:

#pragma region Player �ܺ��Լ�
public:

	void SetParentMouse(GameMouse* _targetMouse)
	{
		targetMouse_ = _targetMouse;
	}

	void SetTargetPos(const float4 _targetPos)
	{
		targetPos_ = _targetPos;
		arrivalPos_ = { targetPos_.x + GetTransform()->GetWorldPosition().x, 0.0f,  targetPos_.y + GetTransform()->GetWorldPosition().y };
		IsMove_ = true;
	}
#pragma endregion
};

