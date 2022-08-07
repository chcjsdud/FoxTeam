#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

#include "Enums.h"
#include "LH_Status.h"

//#include "AIController.h"

class GameEngineCollision;
class GameEngineFBXRenderer;
class Unit : public GameEngineActor
{
public:
	Unit();
	~Unit();

protected: // �⺻����

	bool Iscontrolled_;

	//AIController AIController_;
	GameEngineFSM AIController_;

	CharacterType CharacterType_;		// (0: AI, 1: MAIN) : default(AI)

	GameEngineFSM State_;


	//���� ����Ʈ, 
	std::map <std::string, Buff*> BufferList_;
	// ĳ���� �⺻ ����
	Status Status_Base_;
	// ĳ���� �߰� ���� / ��
	Status Status_Add_;
	// ĳ���� �߰� ���� / ��
	Status Status_Mult_;
	// ĳ���� ���� ����
	Status Status_Final_;



#pragma region ���� ����
	//Ű ���� 07.26, �������� ���� ���������� ����, 
	float4 KeyDir_;

	//���µ� ����� ����, ������ ���⺸�� �ٶ������ ���⿡ �����
	float4 TargetDir_;
	float4 TargetPos_;
	bool IsAttack_;

	bool EnemyInSight_; //�þ߿� ���� ����

#pragma endregion

	// ������ �̵� ����, ������ �ٶ���� �� ����
	float4 FowordDir_;
	// ������ ���� �ٶ󺸴� ���� (�̵��� ������)
	float4 CurFowordDir_;
	//Y�� ȸ�� �ӵ�
	float YRotateSpeed_;

	// ��Ÿ�
	float AttackDist_;
	//���� ��, ���� �ӵ�
	float AttackTurm_;
	// ��Ʈ�ڽ� Ȱ��ȭ ���� ����̴� �ð� (���ϸ��̼�)
	float AttackBuffer_;
	//Attack Hit Box Ȱ��ȭ �ð�
	float AttackHitTime_;

	GameEngineActor* Target_;

	//�ٴ� �ݸ���
	GameEngineCollision* UnitGroundCollision_;
	//�þ� �浹ü
	GameEngineCollision* UnitSightCollision_;
	//�ǰ� ��Ʈ�ڽ�
	GameEngineCollision* UnitHitBoxCollision_;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

protected:
	void SyncStatus();

	const float4 CalculateTargetDir(float4 TargetPos)
	{
		float4 Dir = TargetPos - GetTransform()->GetWorldPosition();
		Dir.Normalize3D();

		return Dir;
	}

protected:
	void UpdateBuff(float _DeltaTime);

	void CurDirUpdate(float _DeltaTime);
	void TargetDirUpdate(float _DeltaTime);
	//RockOn ���϶� ���� ������Ʈ
	//void RockOnDirUpdate(float _DeltaTime);
	//void KeyDirUpdate(float _DeltaTime);
	void MoveUpdate(float _DeltaTime);

	//ĳ���Ͱ� �ٶ󺸴� ������ �ٷ� �ٷ� �Ⱥ��ϰ� õõ�� ����(�̵����� �����ϰ�), 
//07.18 �ٶ󺸴� ������ �־������
	void MoveRotateUpdate(float _DeltaTime);

protected:
	// ĳ���� State, �����Լ��� ���� ĳ���� Ŭ������ �⺻������ ������ �־�� �� state�� ����
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

	// ���ݰ� ���� ���� ���ϸ��̼� ��� ����ϴ� ����, ������ �Ϸ�� �����̸�, �� �ܰ迡�� Attack�� ������ �Լ����� �Ǻ��Ѵ�.
	virtual void Attack_Ready_Start() = 0;
	virtual void Attack_Ready_Update(float _DeltaTime) = 0;
	virtual void Attack_Ready_End() = 0;

public:
	void AddBaseStat(Status _Status);
	void SetBaseStat(Status _Status);
	void AddBuff(std::string _Name, Status _Status, float _Time);
	void RemoveBuff(std::string _Name);
	void RemoveAllBuff(std::string _Name);


public:

	void Unit_AIController_Off()
	{
		Iscontrolled_ = false;
		//AIController_->AIController_Off();
	}
	void Unit_AIController_On()
	{
		Iscontrolled_ = true;
		//AIController_->AIController_On();
	}

	void Unit_SetTargetDir(float4 _Dir)
	{
		TargetDir_ = _Dir;
	}

	void Unit_SetTargetPos(float4 _Pos)
	{
		TargetPos_ = _Pos;
	}

	void Unit_ChangeState(std::string _StateName)
	{
		State_.ChangeState(_StateName);
	}

	//void Unit_SetMove(bool _IsMove)
	//{
	//	IsMove_ = _IsMove;
	//}

	std::string Unit_GetStateName()
	{
		return State_.GetCurrentState()->Name_;
	}

	inline void Unit_SetCharacterType(CharacterType _Type)
	{
		// �߸��� Ÿ�� ���Ž� ����
		if (_Type == CharacterType::NONE || _Type == CharacterType::MAX)
		{
			return;
		}

		CharacterType_ = _Type;
	}

	std::map < std::string, Buff*> PlayerGetBuffList()
	{
		return BufferList_;
	}

	const Status Unit_GetBaseStat()
	{
		return Status_Base_;
	}

	const Status Unit_GetAddStat()
	{
		return Status_Add_;
	}

	const Status Unit_GetMultStat()
	{
		return Status_Mult_;
	}

	const Status Unit_GetFinalStat()
	{
		return Status_Final_;
	}

	const int Unit_GetCurHP()
	{
		return Status_Final_.Stat_Health_;
	}

	const float Unit_GetCurStamina()
	{
		return Status_Final_.Stat_Stamina_;
	}

	GameEngineActor* Unit_GetTarget()
	{
		return Target_;
	}

	const int Unit_GetAttackPower()
	{
		return Status_Final_.Stat_AttackPower_;
	}

	void Unit_SetTarget(GameEngineActor* _Target)
	{
		Target_ = _Target;
	}

protected:		// delete constructer
	Unit(const Unit& _other) = delete;
	Unit(Unit&& _other) = delete;
	Unit& operator=(const Unit& _other) = delete;
	Unit& operator=(const Unit&& _other) = delete;
};
