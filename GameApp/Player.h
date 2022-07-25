#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

#include "Player_Status.h"
/// <summary>
/// ���� ���ΰ� ����
/// </summary>


// �÷��̾ static���� ����� ���� ��𼭵� �ҷ��� �� ������ ���ڴ�.(���ϰڴ�)

class GameMouse;
class GameEngineLevel;
class GameEngineImageRenderer;
class Player : public GameEngineActor
{
public:
	Player();
	~Player();
	Player(const Player& _Other) = delete;
	Player(Player&& _Other) noexcept = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) noexcept = delete;

private:
	//state
	GameEngineFSM PlayerState_;
	GameEngineFSM CameraState_;

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

	// �ν��ؾ� �ϴ� ���콺 ���� ������
	GameMouse* targetMouse_;

	GameEngineActor* Target_;


	//Ű ����
	float4 KeyDir_;
	//���µ� ����� ����
	float4 TargetDir_;
	//�ٶ���� �� ����
	float4 FowordDir_;
	//���� �ٶ󺸴� ����
	float4 CurFowordDir_;
	//Y�� ȸ�� �ӵ�
	float YRotateSpeed_;
	bool IsMove_;
	bool IsRockon_;

	//UI
	class TopUI* TopUI_;
	class LockOnUI* LockOnUI_;
	class Inventory* Inventory_;

#pragma region Player Status
	//���� ��
	float AttackTurm_;
	//���� ���ϸ��̼� �ܰ�
	int AttackLevel_;
	//���� �ð�, �� �ð� �̳��� ����Ű�� ������ ���� �ܰ� ������ ��
	float AttackTime_;
	//Attack Hit Box Ȱ��ȭ �ð�
	float AttackHitTime_;
	//������ ��Ÿ��
	float RollCoolTime_;
	//������ �ð�(����Ÿ��)
	float RollSecond_;

	//���� ����Ʈ, 
	std::map <std::string, PlayerStatus*> Player_BufferList_;

	// �÷��̾� �⺻ ����
	PlayerStatus PlayerStatusBase_;
	// �÷��̾� �߰� ���� / ��
	PlayerStatus PlayerStatusAdd_;
	// �÷��̾� �߰� ���� / ��
	PlayerStatus PlayerStatusMult_;
	// �÷��̾� ���� ����
	PlayerStatus PlayerStatusFinal_;

#pragma endregion

private:
	virtual void Start();
	virtual void Update(float _DeltaTime);

#pragma region Init
private:
	//Init

	// ������, �ݸ���, ��� Componenet �ʱ�ȭ
	void ComponenetInit();
	//State �ʱ�ȭ
	void StateInit();
	void KeyInit();
	void UIInit();
#pragma endregion

#pragma region State
private:
	void Idle_Start();
	void Idle_Update(float _DeltaTime);
	void Idle_End();

	void Walk_Start();
	void Walk_Update(float _DeltaTime);
	void Walk_End();

	void Run_Start();
	void Run_Update(float _DeltaTime);
	void Run_End();

	void Attack_Start();
	void Attack_Update(float _DeltaTime);
	void Attack_End();


	void AttackStateInit()
	{
		// �� �ð� �ȿ� �߰� ���� �ϸ� ���ӱⰡ ����, ���ݽ� ����
		AttackTime_ = 0.5f;

		//���ӱ� �ܰ�, 3�ܰ� ������ �ٽ� 0����
		AttackLevel_ = 0;

		//0.5�� �������� �����Ѵ�. ���ݽ� ����
		AttackTurm_ = 0.25f;

		//0.1�� ���ȸ� Ÿ�� ������ �ִ�. ���ݽ� ����
		AttackHitTime_ = 0.1f;	
	}

#pragma endregion

#pragma region MemberUpdateFunc
private:
	//Member Func
	void DEBUGUpdate(float _DeltaTime);

	// update�� �Լ��� ���� �� ī�޶� ������Ʈ, �ٷ� �ڿ��� �ٶ󺸴� �Լ�
	void CameraUpdate_BackPosition(float _DeltaTime);
	void CameraUpdate_UpPosition(float _DeltaTime);

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

	void Run_Stamina_Decrease(float _DeltaTime)
	{
		PlayerStatusFinal_.Stat_Stamina_ -= _DeltaTime;

		if (PlayerStatusFinal_.Stat_Stamina_ < 0.f)
		{
			PlayerStatusFinal_.Stat_Stamina_ = 0.f;
		}
	}


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

	void SetParentMouse(GameMouse* _targetMouse)
	{
		targetMouse_ = _targetMouse;
	}

	void SetTarget(GameEngineActor* _Target)
	{
		Target_ = _Target;
	}

	const float4 CalculateTargetDir(float4 TargetPos)
	{
		float4 Dir = TargetPos - GetTransform()->GetWorldPosition();
		Dir.Normalize3D();

		return Dir;
	}

	bool SyncPlayerStat();

	//�ӽ� ���� ����, �κ��丮���� �������� ��������, �����ۿ��� �̸��� �������ͽ��� �������Բ�
	bool EquipItem(std::string _BuffName, PlayerStatus* _PlayerStatus);

#pragma endregion

private:

#pragma region Player �ܺ��Լ�
public:

	//void PlayerAddStat(PlayerStatus& _PlayerStatus)
	//{
	//	PlayerStatusAdd_ += _PlayerStatus;
	//}

	//void PlayerSetAddStat(PlayerStatus& _PlayerStatus)
	//{
	//	PlayerStatusAdd_ = _PlayerStatus;
	//}

	std::map < std::string, PlayerStatus*> PlayerGetBuffList()
	{
		return Player_BufferList_;
	}

	const PlayerStatus PlayerGetBaseStat()
	{
		return PlayerStatusBase_;
	}

	const PlayerStatus PlayerGetAddStat()
	{
		return PlayerStatusAdd_;
	}

	const PlayerStatus PlayerGetMultStat()
	{
		return PlayerStatusMult_;
	}

	const PlayerStatus PlayerGetFinalStat()
	{
		return PlayerStatusFinal_;
	}

	const int PlayerGetCurHP()
	{
		return PlayerStatusFinal_.Stat_Hp_;
	}

	//void PlayerSetHP(int _Hp)
	//{
	//	PlayerStatusFinal_.Stat_Hp_ = _Hp;
	//}

	const float PlayerGetCurStamina()
	{
		return PlayerStatusFinal_.Stat_Stamina_;
	}

	//void PlayerSetStamina(float _Stamina)
	//{
	//	PlayerStatusFinal_.Stat_Hp_ = _Stamina;
	//}

	GameEngineActor* PlayerGetTarget()
	{
		return Target_;
	}

	const int PlayerGetAttackPower()
	{
		return PlayerStatusFinal_.Stat_AttackPower_;
	}

	Inventory* PlayerGetInventory()
	{
		return Inventory_;
	}

#pragma endregion
};

