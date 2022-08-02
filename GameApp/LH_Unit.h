#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

#include "Enums.h"
#include "LH_Status.h"

class GameEngineCollision;
class GameEngineFBXRenderer;
class Unit : public GameEngineActor
{
public:
	Unit();
	~Unit();

protected:		// delete constructer
	Unit(const Unit& _other) = delete;
	Unit(Unit&& _other) = delete;
	Unit& operator=(const Unit& _other) = delete;
	Unit& operator=(const Unit&& _other) = delete;

protected: // �⺻����
	CharacterType CharacterType_;		// (0: AI, 1: MAIN) : default(AI)

	GameEngineFSM State_;

	GameEngineFSM CameraState_;

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

	//Ű ���� 07.26, �������� ���� ���������� ����, 
	float4 KeyDir_;

	//���µ� ����� ����, ������ ���⺸�� �ٶ������ ���⿡ �����
	float4 TargetDir_;

	//�ٶ���� �� ����
	float4 FowordDir_;

	//���� �ٶ󺸴� ����
	float4 CurFowordDir_;

	//Y�� ȸ�� �ӵ�
	float YRotateSpeed_;

	bool IsMove_;

	//���� ��
	float AttackTurm_;
	//���� ���ϸ��̼� �ܰ�
	int AttackLevel_;
	//���� �ð�, �� �ð� �̳��� ����Ű�� ������ ���� �ܰ� ������ ��
	float AttackTime_;
	//Attack Hit Box Ȱ��ȭ �ð�
	float AttackHitTime_;

	GameEngineActor* Target_;

	//�ٴ� �ݸ���
	GameEngineCollision* UnitGroundCollision_;
	//�þ� �浹ü
	GameEngineCollision* UnitSightCollision_;
	//�ǰ� ��Ʈ�ڽ�
	GameEngineCollision* UnitHitBoxCollision_;


private: // Ŀ�ǵ� ����
// 1. ����ĳ���� �þ� �ۿ����� ����(�̵�-�Ĺ� ����)
// 2. ����ĳ���� �þ� ������������ ����(�̵�-���� or ����-ȸ��)

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
	// ���ݿ� ��ȭ�� ���涧���� ������
	void SyncStatus();
	void AddBaseStat(Status _Status);
	void SetBaseStat(Status _Status);
	void AddBuff(std::string _Name, Status _Status, float _Time);
	void RemoveBuff(std::string _Name);

	void UpdateBuff(float _DeltaTime);

public:
	void Unit_ChangeState(std::string _State)
	{
		State_.ChangeState(_State);
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
};
