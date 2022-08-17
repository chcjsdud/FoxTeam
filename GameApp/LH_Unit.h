#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include "PlayerController.h"

#include "Enums.h"
#include "LH_Status.h"

// �÷��̾� ��Ʈ�ѷ��� ���� Ű ���ۿ� ���� ���� ������ ���޹޾� �״�� �ൿ��
// �÷��̾�� �Űܾ� �� �κ��� ������ �ϴ� �ϼ��� �Ұ�

// �Լ� ���
//
// 1. ���� == Set
// 2. �߰� == Add
//
// 3. (Set||Add||Get)(���)(�ൿ)
//
// 4. (�̸�)(Update)
//


class GameActorUpdatePaket
{
	//���� ����ϰ� ���������� �ݿ��Ǿ�� �� ������� ��Ŷ���� ������

	unsigned int ObjectID_;
	//std::string Name_;
	//float4 Pos_;
	//TransformData TransformData_;

	float4 vWorldPosition_;
	float4 vWorldRotation_;
	float4 vWorldScaling_;

	float4 vLocalPosition_;
	float4 vLocalRotation_;
	float4 vLocalScaling_;


	int Size_;

	const int GetPaketSize()
	{
		Size_ = sizeof(GameActorUpdatePaket);
	}


};



	// * ��ȹ
	// 1. �÷��̾� ��Ʈ�ѷ��� ������ �����ϰ� ���� ��ǻ�Ͱ� ���� (���� ������, �Ƿ� Ű ��ǲ ������ ����)
	// 2. ������ �÷��̾� ĳ���͸� ������, Ŭ���̾�Ʈ�� ���� ��Ŷ�� �޾� Ŭ���̾�Ʈ�� �÷��̾� ��Ʈ�ѷ��� ĳ���� ���
	// 3. Ŭ���̾�Ʈ�� ������ ����ȭ ������ ��Ŷ�� ����
	// 4. ������ ��Ŷ�� �ް� �Ի���, �ٽ� Ŭ���̾�Ʈ�� ����
	// 5. �ݺ�
	// 
	// ������ ��Ŷ�� ������ �ϴ� ����
	// 1. ����ȭ ��Ű�� ����,
	// 2. ��� ��ǻ�͵��� ��Ÿ Ÿ���� �ٸ��� ������ ��� ������ �ʿ䰡 ����
	// 3. 
	// 
	// ���� ��Ŷ���� ������
	//
	//	0. ��ŸŸ���� ������ ���� ����
	//	1. ���� ������ ���̵�
	//	2. �������� �� �̸�ŭ �̵�����! ��� ������ (������ _deltaTime �����, �̵��� �� �ٽ� �����ٰ�)
	//	2. Ʈ������ ������
	//	3. IsDeath
	//	4. ���ϸ��̼� String
	//	5. ĳ���� ���� �����̵�
	//
	// �������� �޾ƾ� �Ұ�
	//	
	//	1. ���� ���̵�(��Ī���Ѿ� �ϴϱ�) 
	//	2. ������ ����� �̵� ������
	//	3. ������ ����� Ʈ������ ������
	//	4. 
	//
	//	������ ����� �Ұ�
	//	1. ������ ��ŸŸ�ӿ� �°� ����ؼ� �ٽ� ���ֱ�
	//
	// ������ ������ �� ���
	//
	//	1. ��� ���͵��� ������ ID�� ������ �Ұ�
	//	2. �Լ��� ���� ��� ���͵��� ���� ID�� �� �� �ְ� �Ұ�
	// 
	//	3. ID�� ���� ���� �����͵��� ������ ����� �� �ְ� �Ұ�
	// 
	//	4. static unsigned int IDs_ ������ �����, ���͵��� ���� �ɶ����� ++ �ϸ鼭 ���� ����ID�� �Ű��ٰ� (�ߺ� ������ ����)
	//	5. �浹 �Լ��� �浹�� ����� ID ��������
	//	6. ID�� 1���� �����ϰԲ�, (0�� null ���)
	//

enum class Team
{
	None,
	Team1,
	Team2,
	Team3,
	Team4,
	Team5,
	Team6,
	Monster
};

class GameEngineCollision;
class GameEngineFBXRenderer;
class Unit : public GameEngineActor
{
	friend class PlayerController;

public:
	Unit();
	~Unit();


#pragma region Public
public:
	void Unit_AddBaseStat(Status _Status);
	void Unit_SetBaseStat(Status _Status);
	void Unit_AddBuff(std::string _Name, Status _Status, float _Time = -1.f, bool _IsSturn = false, std::function<void()> _BuffFunc = nullptr); //_Time(-1.f) == ��������
	void Unit_RemoveBuff(std::string _Name);
	void Unit_RemoveAllBuff(std::string _Name);

public:
	const Team Unit_GetTeam()
	{
		return Team_;
	}

	void Unit_SetTeam(Team _Team)
	{
		Team_ = _Team;
	}

	std::map < std::string, Buff*> Unit_GetPlayerBuffList()
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
		return Target_Unit_;
	}

	const int Unit_GetAttackPower()
	{
		return Status_Final_.Stat_AttackPower_;
	}

	void Unit_SetTarget(Unit* _Target)
	{
		Target_Unit_ = _Target;
	}

	//const unsigned int GetUnitID()
	//{
	//	return UnitID_;
	//}
	//void Unit_ChangeState(std::string _StateName)
	//{
	//	State_.ChangeState(_StateName);
	//}

	//std::string Unit_GetStateName()
	//{
	//	return State_.GetCurrentState()->Name_;
	//}

	//inline void Unit_SetCharacterType(CharacterType _Type)
	//{
	//	// �߸��� Ÿ�� ���Ž� ����
	//	if (_Type == CharacterType::NONE || _Type == CharacterType::MAX)
	//	{
	//		return;
	//	}

	//	CharacterType_ = _Type;
	//}
#pragma endregion

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

protected:
#pragma region �ʱ�ȭ

#pragma endregion

protected: // �⺻����

	class PlayerController* PlayController_; // ==nullptr ==����
	Team Team_; //������ �Ʊ����� �Ǻ���

	GameEngineFSM OrderState_;
	GameEngineFSM ActionState_;

	//�ٴ� �ݸ���
	GameEngineCollision* UnitGroundCollision_;
	//�þ� �浹ü
	GameEngineCollision* UnitSightCollision_;
	//�ǰ� ��Ʈ�ڽ�
	GameEngineCollision* UnitHitBoxCollision_;

#pragma region ��Ʈ�ѷ� ���� ������ ������

	float4 Target_Pos_; // ���콺�� ������ ���������� �̵��ؾ� �� ��ġ
	Unit* Target_Unit_; // ���콺�� Ÿ������ ����
	Order Controller_Order_; // ��Ʈ�ѷ����� ���޵� ���
	//unsigned int Target_ID_; //Ÿ���� �� ������ ID Ȯ���� ���� ������ ����� �ʿ���,
#pragma endregion

#pragma region �̵� ����

	float4 Move_Navi_Temp_Pos_;	// �׺� �����ִ� �ӽ� �̵� ��ġ
	float4 Move_ForwardDir_;	// ������ �̵� ����, ������ �ٶ���� �� ����	
	float4 Move_Rotate_CurForwardDir_;	// ������ ���� �ٶ󺸴� ���� (�̵��� ������)	
	float Move_YRotateSpeed_;	//Y�� ȸ�� �ӵ�
#pragma endregion

#pragma region ���� ����

	std::map <std::string, Buff*> BufferList_;	//���� ����Ʈ, 	
	Status Status_Base_;		// ĳ���� �⺻ ����
	Status Status_Add_;			// ĳ���� �߰� ���� / ��
	Status Status_Mult_;		// ĳ���� �߰� ���� / ��	
	Status Status_Final_;		// ĳ���� ���� ����

	float AttackTurm_;	//���� ��, ���� �ӵ�
	float AttackBuffer_;	// ��Ʈ�ڽ� Ȱ��ȭ ���� ����̴� �ð� (���ϸ��̼�) 	
	float AttackHitTime_;	//Attack Hit Box Ȱ��ȭ �ð�
#pragma endregion

#pragma region ���� ��Ʈ�� ����

	std::list<Unit*> TargetingUnits_; // ���� Ÿ���� ���ֵ�, ���� Ÿ���� ������ ������ �� Ÿ���� �ߴ��� �Ź� üũ�ؾ��Ѵ�.
									// �Ҹ��� ȣ���, �� ����Ʈ �ο� ������ Target_Unit_�� null�� ����
	bool EnemyInSight_; //�þ߿� ���� ����

	bool IsSturn_;	// ���� ����
#pragma endregion

protected:
#pragma region ������Ʈ
	void Controller_Update();
	void UnitTargetUpdate(float _DeltaTime);
	void StateUpdate(float _DeltaTime);
	void UpdateBuff(float _DeltaTime);
#pragma endregion

#pragma region ���� �Լ�
	void MoveUpdate(float _DeltaTime);

	void NaviUpdate(float _DeltaTime);
	void CurDirUpdate(float _DeltaTime);
	void MoveRotateUpdate(float _DeltaTime);

	bool ChaseTargetUpdate(Unit* _Target_Unit, float _ChaseDist);
	//���ϰ��� true�� ������ �����Ѵ�.
	bool ChasePosUpdate(float4 _Target_Pos, float _ChaseDist);
#pragma endregion


	void SetOrderEnd();

	//���ϰ��� true�� ������ �����Ѵ�.

protected:
	void SetChangeActionState(std::string _State)
	{
		if (ActionState_.GetCurrentState()->Name_ != _State)
		{
			ActionState_.ChangeState(_State);
		}
	}
	
	void SetChangeOrderState(std::string _State)
	{
		if (OrderState_.GetCurrentState()->Name_ != _State)
		{
			OrderState_.ChangeState(_State);
		}
	}

	void SetSyncStatus();

	const float4 GetTargetDir(float4 TargetPos)
	{
		float4 Dir = TargetPos - GetTransform()->GetWorldPosition();
		Dir.Normalize3D();

		return Dir;
	}

	void SetTargetErase()
	{
		Target_Unit_ = nullptr;
	}

	void SetUnitTarget(Unit* _Unit)
	{
		Target_Unit_ = _Unit;
		Target_Unit_->TargetingUnits_.push_back(this);
	}

	//void SetTargetID(unsigned int _Target_ID)
	//{
	//	Target_ID_ = _Target_ID;
	//}

protected:
#pragma region ��� State
	virtual void Order_Attack_Target_Start();
	virtual void Order_Attack_Target_Update(float _DeltaTime);
	virtual void Order_Attack_Target_End();

	virtual void Order_Attack_Pos_Start();
	virtual void Order_Attack_Pos_Update(float _DeltaTime);
	virtual void Order_Attack_Pos_End();

	virtual void Order_Move_Start();
	virtual void Order_Move_Update(float _DeltaTime);
	virtual void Order_Move_End();

	virtual void Order_Stop_Start();
	virtual void Order_Stop_Update(float _DeltaTime);
	virtual void Order_Stop_End();

	virtual void Order_Hold_Start();
	virtual void Order_Hold_Update(float _DeltaTime);
	virtual void Order_Hold_End();

	//virtual void Order_Q_Start();
	//virtual void Order_Q_Update(float _DeltaTime);
	//virtual void Order_Q_End();

	//virtual void Order_W_Start();
	//virtual void Order_W_Update(float _DeltaTime);
	//virtual void Order_W_End();

	//virtual void Order_E_Start();
	//virtual void Order_E_Update(float _DeltaTime);
	//virtual void Order_E_End();

	//virtual void Order_R_Start();
	//virtual void Order_R_Update(float _DeltaTime);
	//virtual void Order_R_End();

	//virtual void Order_D_Start();
	//virtual void Order_D_Update(float _DeltaTime);
	//virtual void Order_D_End();

#pragma endregion

#pragma region ���� State
	virtual void Action_Idle_Start();
	virtual void Action_Idle_Update(float _DeltaTime);
	virtual void Action_Idle_End();

	virtual void Action_Walk_Start();
	virtual void Action_Walk_Update(float _DeltaTime);
	virtual void Action_Walk_End();

	virtual void Action_Attack_Start();
	virtual void Action_Attack_Update(float _DeltaTime);
	virtual void Action_Attack_End();

	virtual void Action_Q_Start()=0;
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

protected:		// delete constructer
	Unit(const Unit& _other) = delete;
	Unit(Unit&& _other) = delete;
	Unit& operator=(const Unit& _other) = delete;
	Unit& operator=(const Unit&& _other) = delete;
};
