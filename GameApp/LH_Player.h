#pragma once

#include "LH_Characters.h"
/// <summary>
/// ���� ���ΰ� ����
/// </summary>


// �÷��̾�� ĳ���� �θ� Ŭ����

class GameMouse;
class GameEngineLevel;
class GameEngineImageRenderer;
class Player : public Characters
{
public:
	Player();
	~Player();

	Player(const Player& _Other) = delete;
	Player(Player&& _Other) noexcept = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) noexcept = delete;

private:
#pragma region Player component
	//�ٴ� �ݸ���
	GameEngineCollision* PlayerGroundCollision_;
	// RockOn �浹ü
	GameEngineCollision* PlayerLockOnCollision_;
	//�ǰ� ��Ʈ�ڽ�
	GameEngineCollision* PlayerHitBoxCollision_;
	//���� ��Ʈ�ڽ�
	GameEngineCollision* PlayerAttackHitBoxCollision_;
	//FBX Renderer
	GameEngineFBXRenderer* FBXRenderer_;
#pragma endregion

	//state
	GameEngineFSM CameraState_;

	// �ν��ؾ� �ϴ� ���콺 ���� ������
	GameMouse* targetMouse_;

	bool IsRockon_;


	// ���ͳ� ���� �߰� ����
	float4 targetPos_;
	// �ϴ޹��� ���콺 Ŀ�� ��Ŭ�� ��ǥ
	float4 arrivalPos_;
	// ���� ���� ��ǥ

public:
	void SetTargetPos(const float4 _targetPos)
	{
		targetPos_ = _targetPos;
		arrivalPos_ = { targetPos_.x + GetTransform()->GetWorldPosition().x, 0.0f,  targetPos_.y + GetTransform()->GetWorldPosition().y };
		IsMove_ = true;
	}

private:
	//UI
	class TopUI* TopUI_;
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

#pragma region Init
protected:
	//Init

	// ������, �ݸ���, ��� Componenet �ʱ�ȭ
	virtual void ComponenetInit();
	//State �ʱ�ȭ
	virtual void StateInit();
	virtual void KeyInit();
	virtual void UIInit();
#pragma endregion

#pragma region State
protected:
	// ĳ���� State, �����Լ��� ���� ĳ���� Ŭ������ �⺻������ ������ �־�� �� state�� ����
	//virtual void Idle_Start() = 0;
	//virtual void Idle_Update(float _DeltaTime) = 0;
	//virtual void Idle_End() = 0;

	//virtual void Walk_Start() = 0;
	//virtual void Walk_Update(float _DeltaTime) = 0;
	//virtual void Walk_End() = 0;

	//virtual void Run_Start() = 0;
	//virtual void Run_Update(float _DeltaTime) = 0;
	//virtual void Run_End() = 0;

	//virtual void Attack_Start() = 0;
	//virtual void Attack_Update(float _DeltaTime) = 0;
	//virtual void Attack_End() = 0;

	// �ӽ� �ڵ�, ���߿� �ּ�ó����
	virtual void Idle_Start();
	virtual void Idle_Update(float _DeltaTime);
	virtual void Idle_End();

	virtual void Walk_Start();
	virtual void Walk_Update(float _DeltaTime);
	virtual void Walk_End();

	virtual void Run_Start();
	virtual void Run_Update(float _DeltaTime);
	virtual void Run_End();

	virtual void Attack_Start();
	virtual void Attack_Update(float _DeltaTime);
	virtual void Attack_End();

	void Stand_Start();
	void Stand_Update(float _DeltaTime);
	void Stand_End();

	void Move_Start();
	void Move_Update(float _DeltaTime);
	void Move_End();
#pragma endregion

#pragma region MemberUpdateFunc
private:
	//Member Func
	void DEBUGUpdate(float _DeltaTime);

	// update�� �Լ��� ���� �� ī�޶� ������Ʈ, �ٷ� �ڿ��� �ٶ󺸴� �Լ�
	void CameraUpdate_BackPosition(float _DeltaTime);
	void CameraUpdate_UpPosition(float _DeltaTime);

	// ���ͳ� ���Ͽ� �߰� : ���ͺ� ����
	void CameraUpdate_EternalReturn(float _DeltaTime);

	void KeyDirUpdate(float _DeltaTime);
	// update�� �Լ��� ���� �� �̵��� ������
	void MoveUpdate(float _DeltaTime);

	//ĳ���Ͱ� �ٶ󺸴� ������ �ٷ� �ٷ� �Ⱥ��ϰ� õõ�� ����(�̵����� �����ϰ�), 
	//07.18 �ٶ󺸴� ������ �־������
	void MoveRotateUpdate(float _DeltaTime);

	//RockOn ���϶� ���� ������Ʈ
	void RockOnDirUpdate(float _DeltaTime);

	void CurDirUpdate(float _DeltaTime);

	void RockOnUpdate(float _DeltaTime);

	void StaminaRecoverUpdate(float _DeltaTime);

	//void Run_Stamina_Decrease(float _DeltaTime)
	//{
	//	Status_Final_.Stat_Stamina_ -= _DeltaTime;

	//	if (Status_Final_.Stat_Stamina_ < 0.f)
	//	{
	//		Status_Final_.Stat_Stamina_ = 0.f;
	//	}
	//}


	//void StaminaDecrease(float _DeltaTime);
	//void HpRecover(float _DeltaTime);
	// 
	//void ChangeCamFunc(void (Player::* _Camfunc)(float))
	//{
	//	CamFunc_ = std::bind(_Camfunc, this, std::placeholders::_1);
	//}
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
	//void PlayerAddStat(PlayerStatus& _PlayerStatus)
	//{
	//	PlayerStatusAdd_ += _PlayerStatus;
	//}

	//void PlayerSetAddStat(PlayerStatus& _PlayerStatus)
	//{
	//	PlayerStatusAdd_ = _PlayerStatus;
	//}



	//Inventory* PlayerGetInventory()
	//{
	//	return Inventory_;
	//}

#pragma endregion
};

