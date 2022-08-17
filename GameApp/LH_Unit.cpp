#include "PreCompile.h"
#include "LH_Unit.h"
#include "GameEngine/GameEngineCollision.h"

Unit::Unit()
	: UnitGroundCollision_(nullptr)
	, UnitSightCollision_(nullptr)
	, UnitHitBoxCollision_(nullptr)
	, PlayController_(nullptr)
	, Target_Unit_(nullptr)
	, Team_(Team::None)
	, Controller_Order_(Order::None)
	, Move_YRotateSpeed_(0.f)
	, AttackTurm_(0.f)
	, AttackBuffer_(0.f)
	, AttackHitTime_(0.f)
	, EnemyInSight_(false)
	, IsSturn_(false)
{	
	{
		ActionState_.CreateState<Unit>
			("Idle", this, &Unit::Action_Idle_Start, &Unit::Action_Idle_Update, &Unit::Action_Idle_End);

		ActionState_.CreateState<Unit>
			("Walk", this, &Unit::Action_Walk_Start, &Unit::Action_Walk_Update, &Unit::Action_Walk_End);

		ActionState_.CreateState<Unit>
			("Attack", this, &Unit::Action_Attack_Start, &Unit::Action_Attack_Update, &Unit::Action_Attack_End);

		ActionState_.CreateState<Unit>
			("Q", this, &Unit::Action_Q_Start, &Unit::Action_Q_Update, &Unit::Action_Q_End);
		ActionState_.CreateState<Unit>
			("W", this, &Unit::Action_W_Start, &Unit::Action_W_Update, &Unit::Action_W_End);
		ActionState_.CreateState<Unit>
			("E", this, &Unit::Action_E_Start, &Unit::Action_E_Update, &Unit::Action_E_End);
		ActionState_.CreateState<Unit>
			("R", this, &Unit::Action_R_Start, &Unit::Action_R_Update, &Unit::Action_R_End);

		ActionState_.ChangeState("Idle");
	}

	{
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

		OrderState_.ChangeState("Order_Stop");
		//OrderState_.CreateState<Unit>
		//	("Order_Q", this, &Unit::Order_Q_Start, &Unit::Order_Q_Update, &Unit::Order_Q_End);

		//OrderState_.CreateState<Unit>
		//	("Order_W", this, &Unit::Order_W_Start, &Unit::Order_W_Update, &Unit::Order_W_End);

		//OrderState_.CreateState<Unit>
		//	("Order_E", this, &Unit::Order_E_Start, &Unit::Order_E_Update, &Unit::Order_E_End);

		//OrderState_.CreateState<Unit>
		//	("Order_R", this, &Unit::Order_R_Start, &Unit::Order_R_Update, &Unit::Order_R_Start);
	}

	{
		Status_Base_.Stat_AttackPower_ = 0;
		Status_Base_.Stat_Health_ = 0;
		Status_Base_.Stat_AttackDist_ = 0.f;
		Status_Base_.Stat_HealthRecovery_ = 0.f;
		Status_Base_.Stat_Stamina_ = 0;
		Status_Base_.Stat_StaminaRecovery_ = 0.f;
		Status_Base_.Stat_Defense_ = 0.f;
		Status_Base_.Stat_AttackSpeed_ = 0.f;
		Status_Base_.Stat_CriticalHit_ = 0.f;
		Status_Base_.Stat_MoveSpeed_ = 0.f;
		Status_Base_.Stat_Eyesight_ = 0.f;
	}
}

Unit::~Unit()
{
	{
		auto iter0 = BufferList_.begin();
		auto iter1 = BufferList_.end();

		for (; iter0 != iter1;)
		{
			delete iter0->second;
			iter0++;
		}

		BufferList_.clear();
	}

	{
		auto iter0 = TargetingUnits_.begin();
		auto iter1 = TargetingUnits_.end();

		for (; iter0 != iter1;)
		{
			(*iter0)->SetTargetErase();
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

}

#pragma endregion

#pragma region ������Ʈ
void Unit::Update(float _DeltaTime)
{
	// AI�� �÷��̾�� ������ Order�� ������ ����� �޶�����
	UnitTargetUpdate(_DeltaTime);
	UpdateBuff(_DeltaTime);
	Controller_Update();

	StateUpdate(_DeltaTime);
}

void Unit::UnitTargetUpdate(float _DeltaTime)
{
	{
		auto iter0 = TargetingUnits_.begin();
		auto iter1 = TargetingUnits_.end();

		for (; iter0 != iter1;)
		{
			if ((*iter0)->Target_Unit_ != this) //���� Ÿ���� ������ ������ ���� Ÿ������ ��� �ֳ� �˻�
			{
				(*iter0)->SetTargetErase();

				TargetingUnits_.erase(iter0);
			}

			++iter0;
		}
	}
}

void Unit::UpdateBuff(float _DeltaTime)
{
	auto iter0 = BufferList_.begin();
	auto iter1 = BufferList_.end();

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
				BufferList_.erase(iter0);
				SetSyncStatus();
			}
		}

		iter0++;
	}
}

void Unit::Controller_Update()
{
	Target_Pos_ = PlayController_->PlayerController_GetTarget_Pos();

	SetUnitTarget(PlayController_->PlayerController_GetTargetUnit());
	//Target_ID_ = PlayController_->PlayerController_GetTarget_ID();
	Controller_Order_ = PlayController_->PlayerController_GetOrder();
}

void Unit::StateUpdate(float _DeltaTime)
{
	if (IsSturn_ == false) // ��� ������Ʈ�� ������ ���¶��
	{
		switch (Controller_Order_)
		{
		case Order::None:
			//SetChangeOrderState("Order_Stop");
			break;
		case Order::Attack_Target:
			SetChangeOrderState("Order_Attack_Target");
			break;
		case Order::Attack_Pos:
			SetChangeOrderState("Order_Attack_Pos");
			break;
		case Order::Move:
			SetChangeOrderState("Order_Move");
			break;
		case Order::Stop:
			SetChangeOrderState("Order_Stop");
			break;
		case Order::Hold:
			SetChangeOrderState("Order_Hold");
			break;
		case Order::Pattrol:
			//SetChangeOrderState("Order_Pattrol");
			break;
		case Order::Q:
			//SetChangeOrderState("Order_Q");
			break;
		case Order::W:
			//SetChangeOrderState("Order_W");
			break;
		case Order::E:
			//SetChangeOrderState("Order_E");
			break;
		case Order::R:
			//SetChangeOrderState("Order_R");
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

#pragma endregion

#pragma region ����
void Unit::SetSyncStatus()
{
	// ���� �ʱ�ȭ
	Status_Mult_.Reset_mult();
	Status_Add_.Reset();
	Status_Final_.Reset();

	//�÷��̾� ���� ����Ʈ
	auto iter0 = BufferList_.begin();
	auto iter1 = BufferList_.end();

	for (; iter0 != iter1;)
	{
		// ���ؾ� �� ������ PlayerStatusAdd_, 
		// ���ؾ� �� ������ PlayerStatusMult_�� ������
		if (true == iter0->second->Status_.Stat_IsMult_)
		{
			Status_Mult_ += iter0->second->Status_;
		}
		else
		{
			Status_Add_ += iter0->second->Status_;
		}
		iter0++;
	}

	// ���������� ���Ǵ� ������ PlayerStatusFinal_��
	// PlayerStatusFinal_�� PlayerStatusBase_�� ���ؾ��� ������ ���� ���ϰ� �������� ���ؾ��� ������ �����־� �����

	Status_Final_ = Status_Base_ + Status_Add_;
	Status_Final_ *= Status_Mult_;
}

void Unit::Unit_AddBaseStat(Status _Status)
{
	Status_Base_ += _Status;
}

void Unit::Unit_SetBaseStat(Status _Status)
{
	Status_Base_ = _Status;
}

void Unit::Unit_AddBuff(std::string _Name, Status _Status, float _Time, bool _IsSturn, std::function<void()> _BuffFunc)
{
	auto iter = BufferList_.find("_Name");

	if (iter->first == _Name)
	{
		iter->second->Time_ = _Time;
		//���� ������ ������ ���, ���ӽð��� ������
		return; 
	}

	Buff* _Buff = new Buff;

	*_Buff = { _Name, _Time , _Status, _IsSturn , _BuffFunc };

	BufferList_.insert(std::make_pair(_Name, _Buff));
}

void Unit::Unit_RemoveBuff(std::string _Name)
{
	BufferList_.erase(BufferList_.find(_Name));
}

void Unit::Unit_RemoveAllBuff(std::string _Name)
{
	auto iter0 = BufferList_.begin();
	auto iter1 = BufferList_.end();

	for (; iter0 != iter1;)
	{
		delete iter0->second;
		iter0++;
	}

	BufferList_.clear();
}

#pragma endregion

#pragma region ���� �Լ�
void Unit::MoveUpdate(float _DeltaTime)
{
	NaviUpdate(_DeltaTime); // �׺���̼� ������Ʈ
	CurDirUpdate(_DeltaTime); // �׺���̼� ������Ʈ�� ���� FBX�� ȸ���ؾ� �� ������ ������Ʈ��
	MoveRotateUpdate(_DeltaTime); // ������Ʈ�� ȸ�� �������� ȸ��������

	GetTransform()->SetWorldMove(Move_ForwardDir_ * Status_Final_.Stat_MoveSpeed_ * _DeltaTime); //�̵��� ������
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
	if (_Target_Unit == nullptr)
	{
		return true;
	}

	// _ChaseDist == ������ ��Ÿ�, �� �Ÿ� �ȱ��� �����ϸ� ������ ����
	// return true == ���� �Ϸ�
	// return false == ���� ��

	float4 Dist = GetTransform()->GetWorldPosition() - _Target_Unit->GetTransform()->GetWorldPosition();

	if (_ChaseDist < Dist.Len3D())
	{
		//Move_ForwardDir_ = _Target_Unit->GetTransform()->GetWorldPosition() - GetTransform()->GetWorldPosition();
		//Move_ForwardDir_.Normalize3D();

		//���� ��Ÿ� �ȿ� ���� ������ ������
		SetChangeActionState("Walk");
		return false;
	}
	else
	{
		return true;
	}
}

bool Unit::ChasePosUpdate(float4 _Target_Pos, float _ChaseDist)
{
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

void Unit::SetOrderEnd()
{
	PlayController_->SetController_Order(Order::Stop);
}

#pragma region ��� State

void Unit::Order_Attack_Target_Start()
{
}

void Unit::Order_Attack_Target_Update(float _DeltaTime)
{
	// ���� ��Ÿ� �ȿ� ���� ��Ÿ�������� Target_Unit_ �����̳� Target_Pos_�� ������
	if (ChaseTargetUpdate(Target_Unit_, Status_Final_.Stat_AttackDist_) == true);
	{
		// Target_Unit_ ���� ������ ����
		SetChangeActionState("Attack");
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
		SetOrderEnd();
	}
	else
	{
		SetChangeActionState("Walk");

		if (ChasePosUpdate(Target_Pos_, Status_Final_.Stat_AttackDist_));
		{
			// Target_Unit_ ���� ������ ����
			SetChangeActionState("Attack");
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

		std::list<GameEngineCollision*> collist = UnitSightCollision_->CollisionPtrGroup(static_cast<int>(CollisionGroup::Unit));
		//std::list<GameEngineCollision*> CollisionIDlist;

		auto iter0 = collist.begin();
		auto iter1 = collist.end();

		for (; iter0 != iter1;)
		{
			Unit* colunit = dynamic_cast<Unit*>((*iter0)->GetActor());
			//unsigned int CollisionTargetID =0; // =(*iter0)->GetID();

			if (colunit->Unit_GetTeam() != Team_) // �ڽŰ� �ٸ����ϰ��
			{
				//SetTargetID(CollisionTargetID);
				SetUnitTarget(colunit);
			}
		}
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


	if (Controller_Order_ == Order::Attack_Pos) // ������ Ÿ���� ���� ���ö� ���¸�
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

				if (colunit->Unit_GetTeam() != Team_) // �ڽŰ� �ٸ����ϰ��
				{
					//SetTargetID(CollisionTargetID);
					SetUnitTarget(colunit);
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
}

void Unit::Action_Attack_Update(float _DeltaTime)
{
}

void Unit::Action_Attack_End()
{
}

void Unit::Action_Q_Start()
{
}

void Unit::Action_Q_Update(float _DeltaTime)
{
}

void Unit::Action_Q_End()
{
}

void Unit::Action_W_Start()
{
}

void Unit::Action_W_Update(float _DeltaTime)
{
}

void Unit::Action_W_End()
{
}

void Unit::Action_E_Start()
{
}

void Unit::Action_E_Update(float _DeltaTime)
{
}

void Unit::Action_E_End()
{
}

void Unit::Action_R_Start()
{
}

void Unit::Action_R_Update(float _DeltaTime)
{
}

void Unit::Action_R_End()
{
}
#pragma endregion
