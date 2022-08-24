#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include "Controller.h"

#include "Enums.h"
#include "LH_Status.h"

//TODO: üũ ����Ʈ (����)
// 
// 1. ��Ŷ ������
// 2. ����, �÷��̾� �и��۾�
// 3. ������ �κ��丮����
// 4. �׺� �ϼ���, �̵��Լ�
// 5. �̵� �Լ� �ϼ���, �����Լ��� ����
// 
// 6. �׺�� �����ϰ� �ϼ��� �ٷ� ���� ������ ������ �̵�, �����Լ� �ϼ�
// 7. ��ų�� ���� �˾Ƽ� ����, ��ų ��� ��ɸ� �����ϴ� ��[��� ���� �ٵ� �̰� �нú����� ��Ƽ������ ������ ����
// 8. ��ų ����ü�� ����� ��Ƽ�� �нú�, ���, Ÿ��, ����, ����, ����, �����, ��Ÿ� ��� �� �� �־�ѱ�?
// 9. ����, Ȧ��, �޽� ����
// 10. 
//
//
//
// 

// �÷��̾� ��Ʈ�ѷ��� ���� Ű ���ۿ� ���� ���� ������ ���޹޾� �״�� �ൿ��
// �÷��̾�� �Űܾ� �� �κ��� ������ �ϴ� �ϼ��� �Ұ�

// �Լ� ���
//
// 1. ���� == Set
// 2. �߰� == Add
//
// 3. (Ŭ����)_(Set||Add||Get)(���)(�ൿ)
//
// 4. (�̸�)(Update)
//


//TODO : ���� ������ :	1.FSM���� ���� ������Ʈ�� ���� ������Ʈ�� �����Ϸ� �ϸ� �����ϴ� ����߰�, ü���� ���ϸ��̼�ó�� bool Force �߰�
//						2. fbx �������� ���� ���ϸ��̼� �̸��� �˰Բ���

#pragma region ��Ŷ 

class UnitPaket
{
protected:
	UnitPaket() 
		: UnitID_(0)
	{
	}
	~UnitPaket()
	{
	}

	unsigned int UnitID_;

public:
	unsigned int UnitPaket_GetUnitID()
	{
		return UnitID_;
	}
};


class UnitUpdatePaket : public UnitPaket// �ӽ� ���� ����
{
	friend class Unit;
protected:
	UnitUpdatePaket()
		: AniFrame_(0)
		, AniFrameTime_(0.f)
		, AniCurFrameTime(0.f)
	{
	}
	~UnitUpdatePaket()
	{
	}

public:
	//���� ����ϰ� ���������� �ݿ��Ǿ�� �� ������� ��Ŷ���� ������
	float4 vWorldPosition_;
	float4 vWorldRotation_;
	float4 vWorldScaling_;

	float4 vLocalPosition_;
	float4 vLocalRotation_;
	float4 vLocalScaling_;

	//Status_Buff Status_Buff_[static_cast<int>(BuffType::Max)]; // ������ ������ ���߿�
	//Status Status_Final_; // Ŭ���̾�Ʈ�� ������� �ʰ� ���� ���� ���ȸ� �޴´�.

	std::string AniName_;
	//AnimationType AnimationType_;

	//TODO: �������� �������� �����ϱ�� �ߴٸ� Ŭ���̾�Ʈ���� ������ �������� �귯���� �ʰ� �ؾ���

	int AniFrame_;
	float AniFrameTime_;
	float AniCurFrameTime;
};

class UnitAttackPaket : public UnitPaket
{
	friend class Unit;
protected:
	UnitAttackPaket()
	{

	}
	UnitAttackPaket(unsigned int _TargerUnitID, int _Damage)
	{
		TargerUnitID_ = _TargerUnitID;
		Damage_ = _Damage;
	}
	~UnitAttackPaket()
	{

	}

public:
	unsigned int TargerUnitID_;
	int Damage_;
};

#pragma endregion

enum class Unit_Team
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
	void Unit_AddBuff(BuffType _BuffType, Status _Status, float _Time = -1.f, bool _IsSturn = false, std::function<void()> _BuffFunc = nullptr); //_Time(-1.f) == ��������
	void Unit_RemoveBuff(BuffType _BuffType);
	void Unit_RemoveAllBuff();

	//void Unit_AddBuff(std::string _Name, Status _Status, float _Time = -1.f, bool _IsSturn = false, std::function<void()> _BuffFunc = nullptr); //_Time(-1.f) == ��������
	//void Unit_RemoveBuff(std::string _Name);
	//void Unit_RemoveAllBuff();

public:
	const unsigned int Unit_GetUnitID()
	{
		return UnitID_;
	}

	UnitUpdatePaket* Unit_Get_UnitUpdatePaket()
	{
		return UnitUpdatePaket_;
	}

	const Unit_Team Unit_GetTeam()
	{
		return Unit_Team_;
	}

	void Unit_SetTeam(Unit_Team _Unit_Team)
	{
		Unit_Team_ = _Unit_Team;
	}

	std::map <BuffType, Status_Buff*> Unit_GetPlayerBuffList()
	{
		return Unit_BufferList_;
	}

	const Status Unit_GetBaseStat()
	{
		return Unit_Status_Base_;
	}

	const Status Unit_GetAddStat()
	{
		return Unit_Status_Add_;
	}

	const Status Unit_GetMultStat()
	{
		return Unit_Status_Mult_;
	}

	const Status Unit_GetFinalStat()
	{
		return Unit_Status_Final_;
	}

	const int Unit_GetCurHP()
	{
		return Unit_Status_Final_.Stat_Health_;
	}

	const float Unit_GetCurStamina()
	{
		return Unit_Status_Final_.Stat_Stamina_;
	}

	Unit* Unit_GetTarget()
	{
		return Target_Unit_;
	}

	const int Unit_GetAttackPower()
	{
		return Unit_Status_Final_.Stat_AttackPower_;
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
	virtual void Unit_Set_State_Init();
	void Unit_Set_Collision_Init();

#pragma endregion
private:
	static unsigned int* UnitStaticIDNumbers_;
public:
	unsigned int UnitID_; // ���� �ʱ�ȭ ���� ���� �Լ� ����
	//�������� Actor�� ������ ��쿡�� ��ȿ��,

protected: // �⺻����
	UnitUpdatePaket* UnitUpdatePaket_;
	Unit_Team Unit_Team_; //�ø������� ���� Ÿ���� �˾Ƴ� ��, ������ �Ʊ����� �Ǻ���

	GameEngineFSM OrderState_;
	GameEngineFSM ActionState_;

	GameEngineFBXRenderer* FBXRenderer_;
	//�ٴ� �ݸ���
	GameEngineCollision* UnitGroundCollision_;
	//�þ� �浹ü
	GameEngineCollision* UnitSightCollision_;
	//�ǰ� ��Ʈ�ڽ�
	GameEngineCollision* UnitHitBoxCollision_;

	Controller* Controller_; //TODO: �÷��̾� ����� �̵��Ұ�, ���Ϳ��� ����� �ϴ��ϴ� ���簡 ���ܳ��� �ٷ� �Űܵ���
#pragma region ��Ʈ�ѷ� ���� ������ ������

	float4 Target_Pos_; // ���콺�� ������ ���������� �̵��ؾ� �� ��ġ
	Unit* Target_Unit_; // ���콺�� Ÿ������ ����
	Controller_Order Controller_Order_; // ��Ʈ�ѷ����� ���޵� ���
	//unsigned int Target_ID_; //Ÿ���� �� ������ ID Ȯ���� ���� ������ ����� �ʿ���,
#pragma endregion

#pragma region �̵� ����

	float4 Move_Navi_Temp_Pos_;	// �׺� �����ִ� �ӽ� �̵� ��ġ
	float4 Move_ForwardDir_;	// ������ �̵� ����, ������ �ٶ���� �� ����	
	float4 Move_Rotate_CurForwardDir_;	// ������ ���� �ٶ󺸴� ���� (�̵��� ������)	
	float Move_YRotateSpeed_;	//Y�� ȸ�� �ӵ�
#pragma endregion

#pragma region ���� ����

	//std::map <std::string, Status_Buff*> Unit_BufferList_;	//���� ����Ʈ, 

	std::map <BuffType, Status_Buff*> Unit_BufferList_;	//�ӽ� ��������, 

	Status Unit_Status_Base_;		// ĳ���� �⺻ ����
	Status Unit_Status_Add_;			// ĳ���� �߰� ���� / ��
	Status Unit_Status_Mult_;		// ĳ���� �߰� ���� / ��	

	Status Unit_Status_Final_;		// ĳ���� ���� ����

	float Unit_AttackTurm_;	//���� ��, ���� �ӵ�
	float Unit_AttackBuffer_;	// ��Ʈ�ڽ� Ȱ��ȭ ���� ����̴� �ð� (���ϸ��̼�) 	
	float Unit_AttackHitTime_;	//Attack Hit Box Ȱ��ȭ �ð�
#pragma endregion

#pragma region ���� ��Ʈ�� ����

	std::list<Unit*> TargetingUnits_; // ���� Ÿ���� ���ֵ�, ���� Ÿ���� ������ ������ �� Ÿ���� �ߴ��� �Ź� üũ�ؾ��Ѵ�.
									// �Ҹ��� ȣ���, �� ����Ʈ �ο� ������ Target_Unit_�� null�� ����
	bool EnemyInSight_; //�þ߿� ���� ����

	bool IsSturn_;	// ���� ����
	bool OrderUpdate_;	// ��� ���ɻ���
#pragma endregion

protected:
#pragma region ������Ʈ
	void Unit_Controller_Update();
	void Unit_TargetUpdate(float _DeltaTime);
	virtual void Unit_StateUpdate(float _DeltaTime);
	void Unit_UpdateBuff(float _DeltaTime);

	void Unit_Send_Server_PaketUpdate();
	void Unit_Receive_Server_PaketUpdate();
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

	void Unit_SetOrderEnd();
	void Unit_SetSyncStatus();

	void Unit_SetChangeActionState(std::string _State)
	{
		if (ActionState_.GetCurrentState()->Name_ != _State)
		{
			ActionState_.ChangeState(_State);
		}
	}
	
	void Unit_SetChangeOrderState(std::string _State)
	{
		if (OrderState_.GetCurrentState()->Name_ != _State)
		{
			OrderState_.ChangeState(_State);
		}
	}

	const float4 GetTargetDir(float4 TargetPos)
	{
		float4 Dir = TargetPos - GetTransform()->GetWorldPosition();
		Dir.Normalize3D();

		return Dir;
	}

	void Unit_SetTargetErase()
	{
		Target_Unit_ = nullptr;
	}

	void Unit_SetUnitTarget(Unit* _Unit)
	{
		Target_Unit_ = _Unit;

		if (_Unit != nullptr)
		{
			Target_Unit_->TargetingUnits_.push_back(this);
		}
	}

protected:
#pragma region ��� State
	virtual void Order_Idle_Start();
	virtual void Order_Idle_Update(float _DeltaTime);
	virtual void Order_Idle_End();

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

	virtual void Order_Rest_Start();
	virtual void Order_Rest_Update(float _DeltaTime);
	virtual void Order_Rest_End();

	virtual void Order_Reload_Start();
	virtual void Order_Reload_Update(float _DeltaTime);
	virtual void Order_Reload_End();

	//TODO: �� �غκ��� ���߿� �÷��̾�.h �� �̵��ϰ� �� ���̴�.

	virtual void Order_Q_Start() = 0;
	virtual void Order_Q_Update(float _DeltaTime) = 0;
	virtual void Order_Q_End() = 0;

	virtual void Order_W_Start() = 0;
	virtual void Order_W_Update(float _DeltaTime) = 0;
	virtual void Order_W_End() = 0;

	virtual void Order_E_Start() = 0;
	virtual void Order_E_Update(float _DeltaTime) = 0;
	virtual void Order_E_End()=0;

	virtual void Order_R_Start() = 0;
	virtual void Order_R_Update(float _DeltaTime) = 0;
	virtual void Order_R_End() = 0;

	virtual void Order_D_Start() = 0;
	virtual void Order_D_Update(float _DeltaTime) = 0;
	virtual void Order_D_End() = 0;

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

	virtual void Action_Stop_Start() ;
	virtual void Action_Stop_Update(float _DeltaTime) ;
	virtual void Action_Stop_End() ;

	virtual void Action_Hold_Start();
	virtual void Action_Hold_Update(float _DeltaTime);
	virtual void Action_Hold_End();

	virtual void Action_Rest_Start();
	virtual void Action_Rest_Update(float _DeltaTime);
	virtual void Action_Rest_End();

	//TODO: �� �غκ��� ���߿� �÷��̾�.h �� �̵��ϰ� �� ���̴�.

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

protected:		// delete constructer
	Unit(const Unit& _other) = delete;
	Unit(Unit&& _other) = delete;
	Unit& operator=(const Unit& _other) = delete;
	Unit& operator=(const Unit&& _other) = delete;
};
