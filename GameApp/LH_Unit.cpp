#include "PreCompile.h"
#include "UserGame.h"

#include "LH_Unit.h"
#include "GameEngine/GameEngineCollision.h"

Unit::Unit()
	: UnitGroundCollision_(nullptr)
	, FBXRenderer_(nullptr)
	, UnitSightCollision_(nullptr)
	, UnitHitBoxCollision_(nullptr)
	, Controller_(nullptr)
	, Target_Unit_(nullptr)
	, Unit_Team_(Unit_Team::None)
	, Controller_Order_(Controller_Order::None_Idle)
	, Move_YRotateSpeed_(0.f)
	, Unit_AttackTurm_(0.f)
	, Unit_AttackBuffer_(0.f)
	, Unit_AttackHitTime_(0.f)
	, EnemyInSight_(false)
	, IsSturn_(false)
	, OrderUpdate_(false)
{	
	ActionState_.CreateState<Unit>
		("Action_Idle", this, &Unit::Action_Idle_Start, &Unit::Action_Idle_Update, &Unit::Action_Idle_End);

	ActionState_.CreateState<Unit>
		("Action_Walk", this, &Unit::Action_Walk_Start, &Unit::Action_Walk_Update, &Unit::Action_Walk_End);

	ActionState_.CreateState<Unit>
		("Action_Attack", this, &Unit::Action_Attack_Start, &Unit::Action_Attack_Update, &Unit::Action_Attack_End);

	ActionState_.CreateState<Unit>
		("Action_Stop", this, &Unit::Action_Stop_Start, &Unit::Action_Stop_Update, &Unit::Action_Stop_End);

	ActionState_.CreateState<Unit>
		("Action_Hold", this, &Unit::Action_Hold_Start, &Unit::Action_Hold_Update, &Unit::Action_Hold_End);

	ActionState_.CreateState<Unit>
		("Action_Rest", this, &Unit::Action_Rest_Start, &Unit::Action_Rest_Update, &Unit::Action_Rest_End);

	ActionState_.ChangeState("Action_Idle");

	OrderState_.CreateState<Unit>
		("Order_Idle", this, &Unit::Order_Idle_Start, &Unit::Order_Idle_Update, &Unit::Order_Idle_End);

	OrderState_.CreateState<Unit>
		("Order_Attack_Target", this, &Unit::Order_Attack_Target_Start, &Unit::Order_Attack_Target_Update, &Unit::Order_Attack_Target_End);

	OrderState_.CreateState<Unit>
		("Order_Attack_Pos", this, &Unit::Order_Attack_Pos_Start, &Unit::Order_Attack_Pos_Update, &Unit::Order_Attack_Pos_End);

	OrderState_.CreateState<Unit>
		("Order_Move", this, &Unit::Order_Move_Start, &Unit::Order_Move_Update, &Unit::Order_Move_End);

	OrderState_.CreateState<Unit>
		("Order_Stop", this, &Unit::Order_Stop_Start, &Unit::Order_Stop_Update, &Unit::Order_Stop_End);

	OrderState_.CreateState<Unit>
		("Order_Hold", this, &Unit::Order_Hold_Start, &Unit::Order_Hold_Update, &Unit::Order_Hold_End);


	OrderState_.ChangeState("Order_Idle");
}

Unit::~Unit()
{
	{
		auto iter0 = Unit_BufferList_.begin();
		auto iter1 = Unit_BufferList_.end();

		for (; iter0 != iter1;)
		{
			delete iter0->second;
			iter0++;
		}

		Unit_BufferList_.clear();
	}

	{
		auto iter0 = TargetingUnits_.begin();
		auto iter1 = TargetingUnits_.end();

		for (; iter0 != iter1;)
		{
			(*iter0)->Unit_SetTargetErase();
			++iter0;
		}

		TargetingUnits_.clear();
	}
}

/*
pragma region
1. �ʱ�ȭ
2. ������Ʈ
3. ����
4. ���� �Լ�
5. ��� State
6. ���� State
*/

#pragma region �ʱ�ȭ
void Unit::Start()
{
	Unit_Set_State_Init(); // ������ ��ӹ޾� ����ؼ� �ǹ� ���� ����, �׳� �ڽ� Ŭ������ �̷��� ���Ŷ�� �ǹ�
}

void Unit::Unit_Set_State_Init()
{
	{
		Unit_Status_Base_.Stat_AttackPower_ = 0;
		Unit_Status_Base_.Stat_Health_ = 0;
		Unit_Status_Base_.Stat_AttackDist_ = 0.f;
		Unit_Status_Base_.Stat_HealthRecovery_ = 0.f;
		Unit_Status_Base_.Stat_Stamina_ = 0;
		Unit_Status_Base_.Stat_StaminaRecovery_ = 0.f;
		Unit_Status_Base_.Stat_Defense_ = 0.f;
		Unit_Status_Base_.Stat_AttackSpeed_ = 0.f;
		Unit_Status_Base_.Stat_CriticalHit_ = 0.f;
		Unit_Status_Base_.Stat_MoveSpeed_ = 0.f;
		Unit_Status_Base_.Stat_Eyesight_ = 0.f;
	}
}
void Unit::Unit_Set_Collision_Init()
{
}
#pragma endregion

#pragma region ������Ʈ
void Unit::Update(float _DeltaTime)
{
	if (UserGame::IsServer_ == false)
	{
		Unit_Set_Receive_PaketUpdate();
	}
	else
	{
		Unit_Set_Send_PaketUpdate();
	}

	// AI�� �÷��̾�� ������ Order�� ������ ����� �޶�����
	Unit_TargetUpdate(_DeltaTime);
	Unit_UpdateBuff(_DeltaTime);

	Unit_StateUpdate(_DeltaTime);
}

void Unit::Unit_TargetUpdate(float _DeltaTime)
{
	{
		auto iter0 = TargetingUnits_.begin();
		auto iter1 = TargetingUnits_.end();

		for (; iter0 != iter1;)
		{
			if ((*iter0)->Target_Unit_ != this) //���� Ÿ���� ������ ������ ���� Ÿ������ ��� �ֳ� �˻�
			{
				(*iter0)->Unit_SetTargetErase();

				TargetingUnits_.erase(iter0);
			}

			++iter0;
		}
	}
}

void Unit::Unit_UpdateBuff(float _DeltaTime)
{
	auto iter0 = Unit_BufferList_.begin();
	auto iter1 = Unit_BufferList_.end();

	for (; iter0 != iter1;)
	{
		if (iter0->second->Time_ == -1.f)
		{
			continue;
		}
		else
		{
			iter0->second->Time_ -= _DeltaTime;

			if (iter0->second->Time_ < 0.f)
			{
				delete iter0->second;
				Unit_BufferList_.erase(iter0);
				Unit_SetSyncStatus();
			}
		}

		iter0++;
	}
}

//void Unit::Unit_Controller_Update()
//{
//	// �̰� ���ֹ����� �ʿ��Ҷ��� ������ ����
//
//	Target_Pos_ = Controller_->Controller_GetTarget_Pos();
//
//	Unit_SetUnitTarget(Controller_->Controller_GetTargetUnit());
//	//Target_ID_ = PlayController_->PlayerController_GetTarget_ID();
//	Controller_Order_ = Controller_->Controller_GetOrder();
//}

void Unit::Unit_StateUpdate(float _DeltaTime) // �����ε� �����ϰ� ������
{
	
	if (IsSturn_ == false && OrderUpdate_==true) // ��� ������Ʈ�� ������ ���¶��
	{
		switch (Controller_Order_)
		{
		case Controller_Order::None_Idle:
			Unit_SetChangeOrderState("Order_Idle");
			break;
		case Controller_Order::A_RB_Attack_Target:
			Unit_SetChangeOrderState("Order_Attack_Target");
			break;
		case Controller_Order::A_Attack_Pos:
			Unit_SetChangeOrderState("Order_Attack_Pos");
			break;
		case Controller_Order::RB_Move:
			Unit_SetChangeOrderState("Order_Move");
			break;
		case Controller_Order::S_Stop:
			Unit_SetChangeOrderState("Order_Stop");
			break;
		case Controller_Order::H_Hold:
			Unit_SetChangeOrderState("Order_Hold");
			break;
		case Controller_Order::X_Rest:
			Unit_SetChangeOrderState("Rest");
			break;
		case Controller_Order::F_ReLoad:
			Unit_SetChangeOrderState("ReLoad");
			break;
		case Controller_Order::Q_Skill:
			Unit_SetChangeOrderState("Order_Q");
			break;
		case Controller_Order::W_Skill:
			Unit_SetChangeOrderState("Order_W");
			break;
		case Controller_Order::E_Skill:
			Unit_SetChangeOrderState("Order_E");
			break;
		case Controller_Order::R_Skill:
			Unit_SetChangeOrderState("Order_R");
			break;
		case Controller_Order::D_Weapon_Skill:
			Unit_SetChangeOrderState("Order_D");
			break;
		default:
			GameEngineDebug::MsgBoxError("�ùٸ��� ���� Order");
			break;
		}

		OrderState_.Update(_DeltaTime);
		//ProcessState_.Update(_DeltaTime);
		ActionState_.Update(_DeltaTime);
	}
}

void Unit::Unit_Set_Receive_PaketUpdate()
{
	if (Unit_Packet_.Unit_ID_ != UnitID_)
	{
		GameEngineDebug::MsgBoxError("����ID�� ��ġ ���� �ʴ� ��Ŷ�� �޾ƿԽ��ϴ�.");
	}

	Target_Pos_ = Unit_Packet_.Target_Pos_;
	Target_Unit_ = dynamic_cast<PlayLevel*>(GetLevel())->GetUnitPtr(Unit_Packet_.Target_Unit_ID_);
	Controller_Order_ = static_cast<Controller_Order>(Unit_Packet_.ControlOrder_);
}

void Unit::Unit_Set_Send_PaketUpdate()
{
	Unit_Packet_.Unit_ID_ = UnitID_; //�� ID
	Unit_Packet_.Target_Pos_ = Target_Pos_; //���콺�� ���� ��ǥ
	Unit_Packet_.Target_Unit_ID_ = Target_Unit_->Unit_GetUnitID(); // Ÿ���� ID
	Unit_Packet_.ControlOrder_ = static_cast<int>(Controller_Order_); // ��� ����
}

#pragma endregion

#pragma region ����
void Unit::Unit_SetSyncStatus()
{
	// ���� �ʱ�ȭ
	Unit_Status_Mult_.Reset_mult();
	Unit_Status_Add_.Reset();
	Unit_Status_Final_.Reset();

	//�÷��̾� ���� ����Ʈ
	auto iter0 = Unit_BufferList_.begin();
	auto iter1 = Unit_BufferList_.end();

	for (; iter0 != iter1;)
	{
		// ���ؾ� �� ������ PlayerStatusAdd_, 
		// ���ؾ� �� ������ PlayerStatusMult_�� ������
		if (true == iter0->second->Status_.Stat_IsMult_)
		{
			Unit_Status_Mult_ += iter0->second->Status_;
		}
		else
		{
			Unit_Status_Add_ += iter0->second->Status_;
		}
		iter0++;
	}

	// ���������� ���Ǵ� ������ PlayerStatusFinal_��
	// PlayerStatusFinal_�� PlayerStatusBase_�� ���ؾ��� ������ ���� ���ϰ� �������� ���ؾ��� ������ �����־� �����

	Unit_Status_Final_ = Unit_Status_Base_ + Unit_Status_Add_;
	Unit_Status_Final_ *= Unit_Status_Mult_;
}

void Unit::Unit_AddBaseStat(Status _Status)
{
	Unit_Status_Base_ += _Status;
}

void Unit::Unit_SetBaseStat(Status _Status)
{
	Unit_Status_Base_ = _Status;
}

void Unit::Unit_AddBuff(BuffType _BuffType, Status _Status, float _Time, bool _IsSturn, std::function<void()> _BuffFunc)
{
	auto iter = Unit_BufferList_.find(_BuffType);

	if (iter->first == _BuffType)
	{
		iter->second->Time_ = _Time;
		//���� ������ ������ ���, ���ӽð��� ������
		return; 
	}

	Status_Buff* _Buff = new Status_Buff;

	*_Buff = { _BuffType, _Time , _Status, _IsSturn , _BuffFunc };

	Unit_BufferList_.insert(std::make_pair(_BuffType, _Buff));
}

void Unit::Unit_RemoveBuff(BuffType _BuffType)
{
	Unit_BufferList_.erase(Unit_BufferList_.find(_BuffType));
}

void Unit::Unit_RemoveAllBuff()
{
	auto iter0 = Unit_BufferList_.begin();
	auto iter1 = Unit_BufferList_.end();

	for (; iter0 != iter1;)
	{
		delete iter0->second;
		iter0++;
	}

	Unit_BufferList_.clear();
}

#pragma endregion

#pragma region ���� �Լ�
void Unit::MoveUpdate(float _DeltaTime)
{
	NaviUpdate(_DeltaTime); // �׺���̼� ������Ʈ
	CurDirUpdate(_DeltaTime); // �׺���̼� ������Ʈ�� ���� FBX�� ȸ���ؾ� �� ������ ������Ʈ��
	MoveRotateUpdate(_DeltaTime); // ������Ʈ�� ȸ�� �������� ȸ��������

	GetTransform()->SetWorldMove(Move_ForwardDir_ * Unit_Status_Final_.Stat_MoveSpeed_ * _DeltaTime); //�̵��� ������
}

void Unit::NaviUpdate(float _DeltaTime)
{
	// �ϴ� �׺� �浹�� �������� �ʾҴٰ� ������
	bool NaniCol = false;

	// �׺� Navi_Final_Move_Pos_�� �̵��ϰԲ� Navi_Temp_Move_Pos_�� ��� �ٲپ� �־����


	//if (NaniCol == true)
	//{

	//}
	//else
	//{
	//	// ���콺�� ����� �������� �ʾ�����
	//	if (Target_Unit_ == nullptr)
	//	{
	//		// �׺� �浹�� �������� �ʾҴٸ� ���콺�� ������ ��ǥ�� ������ ������ ��ǥ�� ������
	//		Move_Navi_Temp_Pos_ = Target_Pos_;
	//	}
	//	else //���콺�� ������ Ÿ���� ������
	//	{
	//		// �׺� �浹�� �������� �ʾҴٸ� ���콺�� ������ Ÿ���� ��ǥ�� ������ ������ ��ǥ�� ������
	//		Move_Navi_Temp_Pos_ = Target_Unit_->GetTransform()->GetWorldPosition();
	//	}
	//}
}

void Unit::CurDirUpdate(float _DeltaTime)	// ���콺 Ŀ�� ��Ŭ���� ��ġ���� ���� ���͸� ����մϴ�.
{
	// ĳ���� FBX�� ȸ���ؾ��� ������ Navi_Temp_Move_Pos_�� ��������

	Move_ForwardDir_ = GetTransform()->GetWorldPosition() - Move_Navi_Temp_Pos_;

	Move_ForwardDir_.Normalize3D();
}

void Unit::MoveRotateUpdate(float _DeltaTime)
{
	if (Move_Rotate_CurForwardDir_ == Move_ForwardDir_)
	{
		return;
	}

	float4 dir = float4::Cross3D(Move_Rotate_CurForwardDir_, Move_ForwardDir_);

	float goaldegree = GameEngineMath::UnitVectorToDegree(Move_ForwardDir_.z, Move_ForwardDir_.x);

	if (dir.y >= 0.f)
	{

		GetTransform()->AddLocalRotationDegreeY(Move_YRotateSpeed_ * _DeltaTime);
		Move_Rotate_CurForwardDir_.RotateYDegree(Move_YRotateSpeed_ * _DeltaTime);

		dir = float4::Cross3D(Move_Rotate_CurForwardDir_, Move_ForwardDir_);
		if (dir.y < 0.f)
		{
			GetTransform()->SetLocalRotationDegree({ 0.f,goaldegree,0.f });
			Move_Rotate_CurForwardDir_ = Move_ForwardDir_;
		}
	}
	
	else if (dir.y < 0.f)
	{
		GetTransform()->AddLocalRotationDegreeY(-Move_YRotateSpeed_ * _DeltaTime);
		Move_Rotate_CurForwardDir_.RotateYDegree(-Move_YRotateSpeed_ * _DeltaTime);

		dir = float4::Cross3D(Move_Rotate_CurForwardDir_, Move_ForwardDir_);
		if (dir.y > 0.f)
		{
			GetTransform()->SetLocalRotationDegree({ 0.f,goaldegree,0.f });
			Move_Rotate_CurForwardDir_ = Move_ForwardDir_;
		}
	}
}

bool Unit::ChaseTargetUpdate(Unit* _Target_Unit, float _ChaseDist)
{
	//TODO: ���� �߰��ϰ� �������� ���� ���������� �Ѱ��ְԲ� �ϸ� ���ڴ�.

	// _ChaseDist == ������ ��Ÿ�, �� �Ÿ� �ȱ��� �����ϸ� ������ ����
	// return true == ���� �Ϸ�
	// return false == ���� ��

	float4 Dist = GetTransform()->GetWorldPosition() - _Target_Unit->GetTransform()->GetWorldPosition();

	if (_ChaseDist < Dist.Len3D())
	{
		//Move_ForwardDir_ = _Target_Unit->GetTransform()->GetWorldPosition() - GetTransform()->GetWorldPosition();
		//Move_ForwardDir_.Normalize3D();

		//���� ��Ÿ� �ȿ� ���� ������ ������
		return false;
	}
	else
	{
		return true;
	}
}

bool Unit::ChasePosUpdate(float4 _Target_Pos, float _ChaseDist)
{
	//TODO: ���ְ� �������� ��ü�ص� �ɰŰ���.

	//if (Target_ID_ == 0)
	if (Target_Unit_ == nullptr)
	{
		Move_ForwardDir_ = _Target_Pos - GetTransform()->GetWorldPosition();
		Move_ForwardDir_.Normalize3D();

		return false;
	}

	if (_Target_Pos == GetTransform()->GetWorldPosition())
	{
		// ���� ��ġ�� �������� ���, ������ ����
		return true;
	}

	//if (Target_ID_ != 0)
	if (Target_Unit_ != nullptr)
	{
		float4 Dist = GetTransform()->GetWorldPosition() - Target_Unit_->GetTransform()->GetWorldPosition();

		if (_ChaseDist < Dist.Len3D())
		{
			Move_ForwardDir_ = Target_Unit_->GetTransform()->GetWorldPosition() - GetTransform()->GetWorldPosition();
			Move_ForwardDir_.Normalize3D();

			if (ActionState_.GetCurrentState()->Name_ != "Walk")
			{
				ActionState_.ChangeState("Walk"); //��Ÿ� ���̸� �ű���� �ɾ��
			}
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		Move_ForwardDir_ = Target_Unit_->GetTransform()->GetWorldPosition() - GetTransform()->GetWorldPosition();
		Move_ForwardDir_.Normalize3D();

		Target_Pos_;
	}

	return false;
}
#pragma endregion


void Unit::Unit_SetOrderEnd()
{
	Controller_->Controller_SetController_Order(Controller_Order::None_Idle);
	OrderState_.ChangeState("Idle");
	ActionState_.ChangeState("Idle");

	OrderUpdate_ = true;
}

#pragma region ��� State

void Unit::Order_Idle_Start()
{
	OrderUpdate_ = true;
	//FBXRenderer_->ChangeFBXAnimation("Unit_Ani_Idle");
}

void Unit::Order_Idle_Update(float _DeltaTime)
{
}

void Unit::Order_Idle_End()
{
}

void Unit::Order_Attack_Target_Start()
{
}

void Unit::Order_Attack_Target_Update(float _DeltaTime)
{
	if (Target_Unit_ == nullptr)
	{
		Unit_SetOrderEnd();
	}

	// ���� ��Ÿ� �ȿ� ���� ��Ÿ�������� Target_Unit_ �����̳� Target_Pos_�� ������
	if (ChaseTargetUpdate(Target_Unit_, Unit_Status_Final_.Stat_AttackDist_) == true)
	{
		// Target_Unit_ ���� ������ ����
		Unit_SetChangeActionState("Action_Attack");
	}
	else
	{
		// �� �������� �����̰� �������ֱ�
		Unit_SetChangeActionState("Action_Walk");
	}
}

void Unit::Order_Attack_Target_End()
{
}

void Unit::Order_Attack_Pos_Start()
{
}

void Unit::Order_Attack_Pos_Update(float _DeltaTime)
{
	if (GetTransform()->GetWorldPosition() == Target_Pos_)
	{
		// ��� ����
		Unit_SetOrderEnd();
	}
	else
	{
		//Ÿ�� ��ǥ �������� �̵��ϰ� �������ֱ�
		Unit_SetChangeActionState("Walk");

		// �����̴��� ���� �þ߿� ������ �׳��� �����ϰ�, �þ߿��� ����ų� �׾������ ���� ������ ���� �ϱ�
		std::list<GameEngineCollision*> collist = UnitSightCollision_->CollisionPtrGroup(static_cast<int>(CollisionGroup::Unit));

		auto iter0 = collist.begin();
		auto iter1 = collist.end();

		for (; iter0 != iter1;)
		{		
			Unit* colunit = dynamic_cast<Unit*>((*iter0)->GetActor());
			
			if (colunit->Unit_GetTeam() != Unit_Team_) // �ڽŰ� �ٸ����ϰ��
			{
				// Ÿ������ ������
				Unit_SetUnitTarget(colunit);
			}
		}

		if (Target_Unit_ == nullptr)
		{
			Unit_SetOrderEnd();
		}

		// ���� ��Ÿ� �ȿ� ���� ��Ÿ�������� Target_Unit_ �����̳� Target_Pos_�� ������
		if (ChaseTargetUpdate(Target_Unit_, Unit_Status_Final_.Stat_AttackDist_) == true)
		{
			// Target_Unit_ ���� ������ ����
			Unit_SetChangeActionState("Action_Attack");
		}
		else
		{
			// �� �������� �����̰� �������ֱ�
			Unit_SetChangeActionState("Action_Walk");
		}




		if (ChasePosUpdate(Target_Pos_, Unit_Status_Final_.Stat_AttackDist_));
		{
			Unit_SetChangeActionState("Attack");
		}
	}

	if (Target_Unit_ == nullptr)
	{
		if (ActionState_.GetCurrentState()->Name_ != "Walk")
		{
			// ���콺, Ȥ�� �þ� AI�� ���� TargetPos_ ��ġ�� �����ؼ� �ű��� �����̰� ���ش�
			ActionState_.ChangeState("Walk"); //��Ÿ� ���̸� �ű���� �ɾ��
		}

		// ���ö� ���¿��� �þ߿� ���� ������ �ȴٸ� TargetActor_�� ������ �ְ�, �� if������ ���� �ȴ�.
		// �� ���¿��� ���� �������� ���, �ٽ� �������� ���� ���� �� �־�� �Ѵ�.

		//std::list<GameEngineCollision*> collist = UnitSightCollision_->CollisionPtrGroup(static_cast<int>(CollisionGroup::Unit));
		////std::list<GameEngineCollision*> CollisionIDlist;

		//auto iter0 = collist.begin();
		//auto iter1 = collist.end();

		//for (; iter0 != iter1;)
		//{
		//	Unit* colunit = dynamic_cast<Unit*>((*iter0)->GetActor());
		//	//unsigned int CollisionTargetID =0; // =(*iter0)->GetID();

		//	if (colunit->Unit_GetTeam() != Unit_Team_) // �ڽŰ� �ٸ����ϰ��
		//	{
		//		//SetTargetID(CollisionTargetID);
		//		Unit_SetUnitTarget(colunit);
		//	}
		//}
	}
	//AŰ�� �̿��� ���� ����� �������� �̰����� ���� ��������, ������ ��������, �þ߿� ���� ���� �������� �Ǻ��Ѵ�.

	//if (Target_Unit_ != nullptr)
	//{
	//	float4 Dist = GetTransform()->GetWorldPosition() - Target_Unit_->GetTransform()->GetWorldPosition();

	//	if (AttackDist_ < Dist.Len3D())
	//	{
	//		if (ActionState_.GetCurrentState()->Name_ != "Walk")
	//		{
	//			ActionState_.ChangeState("Walk"); //��Ÿ� ���̸� �ű���� �ɾ��
	//		}
	//	}
	//	else
	//	{
	//		if (ActionState_.GetCurrentState()->Name_ != "Attack")
	//		{
	//			ActionState_.ChangeState("Attack"); // ��Ÿ� ���̸� �����ض�
	//		}
	//	}
	//}


	if (Controller_Order_ == Controller_Order::A_Attack_Pos) // ������ Ÿ���� ���� ���ö� ���¸�
	{
		if (Target_Unit_ == nullptr)
		{
			if (ActionState_.GetCurrentState()->Name_ != "Walk")
			{
				// ���콺, Ȥ�� �þ� AI�� ���� TargetPos_ ��ġ�� �����ؼ� �ű��� �����̰� ���ش�
				ActionState_.ChangeState("Walk"); //��Ÿ� ���̸� �ű���� �ɾ��
			}

			// ���ö� ���¿��� �þ߿� ���� ������ �ȴٸ� TargetActor_�� ������ �ְ�, �� if������ ���� �ȴ�.
			// �� ���¿��� ���� �������� ���, �ٽ� �������� ���� ���� �� �־�� �Ѵ�.

			std::list<GameEngineCollision*> collist = UnitSightCollision_->CollisionPtrGroup(static_cast<int>(CollisionGroup::Unit));

			auto iter0 = collist.begin();
			auto iter1 = collist.end();

			for (; iter0 != iter1;)
			{
				Unit* colunit = dynamic_cast<Unit*>((*iter0)->GetActor());
				//unsigned int CollisionTargetID=0; // =(*iter0)->GetID();

				if (colunit->Unit_GetTeam() != Unit_Team_) // �ڽŰ� �ٸ����ϰ��
				{
					//SetTargetID(CollisionTargetID);
					Unit_SetUnitTarget(colunit);
				}
			}
		}
	}
}

void Unit::Order_Attack_Pos_End()
{
}

void Unit::Order_Move_Start()
{
	//if()
}

void Unit::Order_Move_Update(float _DeltaTime)
{
}

void Unit::Order_Move_End()
{
}

void Unit::Order_Stop_Start()
{
}

void Unit::Order_Stop_Update(float _DeltaTime)
{
}

void Unit::Order_Stop_End()
{
}

void Unit::Order_Hold_Start()
{
}

void Unit::Order_Hold_Update(float _DeltaTime)
{
}

void Unit::Order_Hold_End()
{
}

void Unit::Order_Rest_Start()
{
}

void Unit::Order_Rest_Update(float _DeltaTime)
{
}

void Unit::Order_Rest_End()
{
}

void Unit::Order_Reload_Start()
{
}

void Unit::Order_Reload_Update(float _DeltaTime)
{
}

void Unit::Order_Reload_End()
{
}

#pragma endregion

#pragma region ���� State
void Unit::Action_Idle_Start()
{
}

void Unit::Action_Idle_Update(float _DeltaTime)
{
}

void Unit::Action_Idle_End()
{
}

void Unit::Action_Walk_Start()
{
}

void Unit::Action_Walk_Update(float _DeltaTime)
{
}

void Unit::Action_Walk_End()
{
}

void Unit::Action_Attack_Start()
{
	OrderUpdate_ = false;
}

void Unit::Action_Attack_Update(float _DeltaTime)
{
}

void Unit::Action_Attack_End()
{
	OrderUpdate_ = true;
}

void Unit::Action_Stop_Start()
{
}

void Unit::Action_Stop_Update(float _DeltaTime)
{
}

void Unit::Action_Stop_End()
{
}

void Unit::Action_Hold_Start()
{
}

void Unit::Action_Hold_Update(float _DeltaTime)
{
}

void Unit::Action_Hold_End()
{
}

void Unit::Action_Rest_Start()
{
}

void Unit::Action_Rest_Update(float _DeltaTime)
{
}

void Unit::Action_Rest_End()
{
}
#pragma endregion
