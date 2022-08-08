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

	bool Iscontrolled_; //����(�÷��̾�)�� �÷��̾� ��Ʈ�ѷ��� ���� ��Ʈ�� �Ǵ��� �Ǵ��Ͽ� AI�� ����
	/*
	������ ��ɵ��� Unit class �ȿ��� ���� �⺻���� Ʋ�� �̸� ���ǵǾ� �ְ�, ���� ������ �ְ� ������ override �ؼ� ���(��ų�� ������ ���� �����Լ���)


	1. ���� ��� : PlayerControl, AIControll ���� UI, ����Ű, AI�˰����� ���� ������ ���, ������ ��ħ

				1. Idle(None)
				2. ��Ŭ��
				3. ��Ŭ��
				4. A(Attack)
				5. S(Stop(����!, ��� ���))
				6. H(Hold)
				7. P(Patrrol)

				8~ . Skill 1~
					��ų�� ��ų ��Ÿ�, Ÿ��, ��Ÿ�� ���θ� ������

	2. �ܰ� ��� : ���� ����� �޾� �װ��� �����ϱ� ���� �ܰ�, ���� ����� ������ �ؼ��ϰ� ������ ���ϱ� ���� ���(����)�� ��
					�ܰ� ��ɿ��� ���� ��ɿ� �ɸ´� ���� ����� ����
					���� ����� ���� ������� �����ϴ� �ؼ� �ܰ�

					���� �÷��̾� ĳ���͸��� ������ ��ų �ܰ� ����� ������ �� �ְ� �ؾ���
					������ �÷��̾� ĳ���͸��� ������ ���� �˰����̳� �׷��� ���� ���� �ִ°� �����ؾ���
					�����׿��� ���� ����� ������ �����Ұ�����, �ܰ� ����� �� ĳ���͸��� �������̵� �ؼ� ����� �� �ְ���(�������̵� ���ϸ� �׳� ������ ���)

					0. �ܰ� ����� ������Ʈ �ϴ���, CC�⸦ ������ ���� ����� ���(Stop)�� 

					1. Idle(None)
					2. Detect (���� �ִ°� Ž����)
					3. Chase (���� ���� ����� ���� ������, ���� Ž���ϸ� ������ (��Ÿ� �ȱ��� Walk ���� ��� ����))
								(��ų ���� ����� ����������, Ÿ�� �������� �� Ÿ������ ����)
								(Ÿ�� ������ ��� ���� Ž���ϸ� ������ (��Ÿ� �ȱ��� Walk ���� ��� ����))
								(��Ÿ���̸� �ٷ� ��ų �ܰ�� �Ѿ)

					4-1. Attack (������ �Ϸ�Ǿ����� ������(Attack ���� ��� ����)
								(Attack ���� ��� ���� ��, ���� ����� Attack ���¸� Chase ���·� ���ư�(���� �������� �ʾ����� �ٽ� Attack���� ���ƿð�))
								(���� �Ϸ���, Attack �� ������ ������ ���� ����� �ְų� �Ÿ��� ����� Chase or Idle �� ���ư�)
								(Stop�̳� �ٸ� ���� ����� �������� �׿� �ɸ´� �ܰ� ��� ����)

					4-2. Skill (������ �Ϸ�Ǿ�����(�� Ÿ���̰ų�) �׿� �ɸ´� ��ų ���� ����� ����, ��ų�� �������� �Ǵ��Ͽ� �ɸ´� ��� ����) 

					5. Sturn (CC�⿡ ���� Sturn�� �ɸ� ����, Sturn ���� ��� ������, �ð��� ������ None ���·� �ǵ���)

					��) ���� ��ɿ��� � Actor�� ��Ŭ�� �ϸ� �װ� ������ �Ʊ����� ���� �Ǵ��ؼ� �װͿ� �ɸ´� ���� ����� ����

					��) ���� ���� ����߿� ����� ������ų� ����� �׾������, ���� ����� Idle�� ��ȯ�ϰ�, ���� ����� ���(Stop)��

					��) ĳ���͸��� �ٸ� ��ų �˰����� ���Ұ��, �ܰ��ɿ��� ���� ����� �����ϴ� �Լ��� �������̵� �ؼ� ���

	3. ���� ��� : Idle, Walk, Attack, Sturn ���� ���� ������ �����ϴ� �ܰ�,
					������ ���۵��� ���� �������� 
					���� ĳ���͸��� ������ �Լ��� ���Ұ��, �������̵� �ؼ� ���� ���� ����





		��� ������ ���� ��� �ܰ踦 ������
		AI�� AI ��Ʈ�ѷ���, �÷��̾�� �÷��̾� ��Ʈ�ѷ��� ����� ����

		���� �÷��̾�� ��ų�� ������ �˰����� �ٸ��� ������, �� ��� ���� ���� �߰��Ͽ� ����� �� �ְ� �ؾ���

		AI ��Ʈ�ѷ��� �θ� Ŭ������ �����ؼ� ���� ��� �޾Ƽ� �ڱ⸸�� ��Ʈ�ѷ��� ���ϵ��� �ؾ���

		�÷��̾� ��Ʈ�ѷ��� �׳� ����Ű �����Ϳ� ���� ��ɸ� ������ ���̴� �߿�ġ ����


		���¿� ����� �̿��ϰ� ���ϵ� �������̽��� ������� �ϴ� ������ �ټ��� ������ �÷��̾� Ŭ������ �����Ҷ� �˰����� �����Ͽ� ���� �浹, ���׸� �����ϱ� ����




	���� �ǵ� : ������ �ൿ, ���۵鿡 �������� �ο��Ͽ� �ٸ������� ������ ���� �����ϰ� ����
				

	����� �ܰ迡�� ���� ������ ���, ����� �����ϱ� ���� ���ϴ� �ܰ�, ���� ���� ����� ���������� ������
	
	
	
	*/


	//AIController AIController_;
	GameEngineFSM Primal_Order_; // �÷��̾�, AI���� ������ ���� ������ ��ɴܰ�, UI, ����Ű ���� ���� Attack, Skill, Stop, Hold, Move, Patrrol ���� ���
	GameEngineFSM Process_Order_; // ���� ����� �����ϱ� ���� �ܰ��� ���, ������ ���� ����, Ž��, 
	GameEngineFSM Action_Order_; // ���� ����� �����ϱ� ���� ���� �ܰ�, Attack ����, Move ����, Idle ���� 

	//GameEngineFSM AIController_;

	GameEngineFSM State_;


	CharacterType CharacterType_;		// (0: AI, 1: MAIN) : default(AI)

#pragma region ���� ����
	//Ű ���� 07.26, �������� ���� ���������� ����, 
	float4 KeyDir_;

	//���µ� ����� ����, ������ ���⺸�� �ٶ������ ���⿡ �����
	float4 TargetDir_;
	float4 TargetPos_;
	bool IsAttack_;

	bool EnemyInSight_; //�þ߿� ���� ����

#pragma endregion

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

	bool IsSturn_;
	float SturnTime_;

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

	Unit* Target_;

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

	Unit* Unit_GetTarget()
	{
		return Target_;
	}

	const int Unit_GetAttackPower()
	{
		return Status_Final_.Stat_AttackPower_;
	}

	void Unit_SetTarget(Unit* _Target)
	{
		Target_ = _Target;
	}

protected:		// delete constructer
	Unit(const Unit& _other) = delete;
	Unit(Unit&& _other) = delete;
	Unit& operator=(const Unit& _other) = delete;
	Unit& operator=(const Unit&& _other) = delete;
};
