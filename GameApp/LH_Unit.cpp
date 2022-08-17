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
1. 초기화
2. 업데이트
3. 스탯
4. 무브 함수
5. 명령 State
6. 동작 State
*/

#pragma region 초기화
void Unit::Start()
{

}

#pragma endregion

#pragma region 업데이트
void Unit::Update(float _DeltaTime)
{
	// AI냐 플레이어냐 에따라 Order를 얻어오는 방법이 달라질것
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
			if ((*iter0)->Target_Unit_ != this) //나를 타겟한 유닛이 아직도 나를 타겟으로 삼고 있나 검사
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
	if (IsSturn_ == false) // 명령 업데이트가 가능한 상태라면
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
			GameEngineDebug::MsgBoxError("올바르지 않은 Order");
			break;
		}

		OrderState_.Update(_DeltaTime);
		//ProcessState_.Update(_DeltaTime);
		ActionState_.Update(_DeltaTime);
	}
}

#pragma endregion

#pragma region 스탯
void Unit::SetSyncStatus()
{
	// 스텟 초기화
	Status_Mult_.Reset_mult();
	Status_Add_.Reset();
	Status_Final_.Reset();

	//플레이어 버프 리스트
	auto iter0 = BufferList_.begin();
	auto iter1 = BufferList_.end();

	for (; iter0 != iter1;)
	{
		// 더해야 할 스텟은 PlayerStatusAdd_, 
		// 곱해야 할 스텟은 PlayerStatusMult_에 더해줌
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

	// 최종적으로 사용되는 스탯은 PlayerStatusFinal_임
	// PlayerStatusFinal_은 PlayerStatusBase_에 더해야할 스텟을 먼저 더하고 마지막에 곱해야할 스텟을 곱해주어 계산함

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
		//같은 버프가 들어왔을 경우, 지속시간만 갱신함
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

#pragma region 무브 함수
void Unit::MoveUpdate(float _DeltaTime)
{
	NaviUpdate(_DeltaTime); // 네비게이션 업데이트
	CurDirUpdate(_DeltaTime); // 네비게이션 업데이트를 통해 FBX가 회전해야 할 방향을 업데이트함
	MoveRotateUpdate(_DeltaTime); // 업데이트된 회전 방향으로 회전시켜줌

	GetTransform()->SetWorldMove(Move_ForwardDir_ * Status_Final_.Stat_MoveSpeed_ * _DeltaTime); //이동을 수행함
}

void Unit::NaviUpdate(float _DeltaTime)
{
	// 일단 네비가 충돌을 감지하지 않았다고 가정함
	bool NaniCol = false;

	// 네비가 Navi_Final_Move_Pos_로 이동하게끔 Navi_Temp_Move_Pos_를 계속 바꾸어 주어야함


	//if (NaniCol == true)
	//{

	//}
	//else
	//{
	//	// 마우스로 대상을 지정하지 않았을때
	//	if (Target_Unit_ == nullptr)
	//	{
	//		// 네비가 충돌을 감지하지 않았다면 마우스가 지정한 좌표를 앞으로 움직일 좌표로 설정함
	//		Move_Navi_Temp_Pos_ = Target_Pos_;
	//	}
	//	else //마우스로 지정한 타겟이 있을때
	//	{
	//		// 네비가 충돌을 감지하지 않았다면 마우스로 지정한 타겟의 좌표를 앞으로 움직일 좌표로 설정함
	//		Move_Navi_Temp_Pos_ = Target_Unit_->GetTransform()->GetWorldPosition();
	//	}
	//}
}

void Unit::CurDirUpdate(float _DeltaTime)	// 마우스 커서 우클릭한 위치로의 방향 벡터를 계산합니다.
{
	// 캐릭터 FBX가 회전해야할 방향을 Navi_Temp_Move_Pos_로 지정해줌

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

	// _ChaseDist == 추적할 사거리, 이 거리 안까지 추적하면 추적을 멈춤
	// return true == 추적 완료
	// return false == 추적 중

	float4 Dist = GetTransform()->GetWorldPosition() - _Target_Unit->GetTransform()->GetWorldPosition();

	if (_ChaseDist < Dist.Len3D())
	{
		//Move_ForwardDir_ = _Target_Unit->GetTransform()->GetWorldPosition() - GetTransform()->GetWorldPosition();
		//Move_ForwardDir_.Normalize3D();

		//공격 사거리 안에 적이 없으면 움직임
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
		// 같은 위치에 도달했을 경우, 추적을 끝냄
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
				ActionState_.ChangeState("Walk"); //사거리 밖이면 거기까지 걸어가라
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

#pragma region 명령 State

void Unit::Order_Attack_Target_Start()
{
}

void Unit::Order_Attack_Target_Update(float _DeltaTime)
{
	// 공격 사거리 안에 적이 나타날때까지 Target_Unit_ 방향이나 Target_Pos_로 움직임
	if (ChaseTargetUpdate(Target_Unit_, Status_Final_.Stat_AttackDist_) == true);
	{
		// Target_Unit_ 에게 공격을 가함
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
		// 명령 종료
		SetOrderEnd();
	}
	else
	{
		SetChangeActionState("Walk");

		if (ChasePosUpdate(Target_Pos_, Status_Final_.Stat_AttackDist_));
		{
			// Target_Unit_ 에게 공격을 가함
			SetChangeActionState("Attack");
		}
	}

	if (Target_Unit_ == nullptr)
	{
		if (ActionState_.GetCurrentState()->Name_ != "Walk")
		{
			// 마우스, 혹은 시야 AI로 부터 TargetPos_ 위치를 지정해서 거까지 움직이게 해준다
			ActionState_.ChangeState("Walk"); //사거리 밖이면 거기까지 걸어가라
		}

		// 어택땅 상태에서 시야에 적이 들어오게 된다면 TargetActor_를 세팅해 주고, 위 if문으로 들어가게 된다.
		// 이 상태에서 적을 물리쳤을 경우, 다시 가던길을 마저 가게 해 주어야 한다.

		std::list<GameEngineCollision*> collist = UnitSightCollision_->CollisionPtrGroup(static_cast<int>(CollisionGroup::Unit));
		//std::list<GameEngineCollision*> CollisionIDlist;

		auto iter0 = collist.begin();
		auto iter1 = collist.end();

		for (; iter0 != iter1;)
		{
			Unit* colunit = dynamic_cast<Unit*>((*iter0)->GetActor());
			//unsigned int CollisionTargetID =0; // =(*iter0)->GetID();

			if (colunit->Unit_GetTeam() != Team_) // 자신과 다른팀일경우
			{
				//SetTargetID(CollisionTargetID);
				SetUnitTarget(colunit);
			}
		}
	}
	//A키를 이용한 공격 명령이 내려지면 이곳에서 적을 추적할지, 추적을 종료할지, 시야에 들어온 적을 공격할지 판별한다.

	//if (Target_Unit_ != nullptr)
	//{
	//	float4 Dist = GetTransform()->GetWorldPosition() - Target_Unit_->GetTransform()->GetWorldPosition();

	//	if (AttackDist_ < Dist.Len3D())
	//	{
	//		if (ActionState_.GetCurrentState()->Name_ != "Walk")
	//		{
	//			ActionState_.ChangeState("Walk"); //사거리 밖이면 거기까지 걸어가라
	//		}
	//	}
	//	else
	//	{
	//		if (ActionState_.GetCurrentState()->Name_ != "Attack")
	//		{
	//			ActionState_.ChangeState("Attack"); // 사거리 안이면 공격해라
	//		}
	//	}
	//}


	if (Controller_Order_ == Order::Attack_Pos) // 지정한 타겟이 없는 어택땅 상태면
	{
		if (Target_Unit_ == nullptr)
		{
			if (ActionState_.GetCurrentState()->Name_ != "Walk")
			{
				// 마우스, 혹은 시야 AI로 부터 TargetPos_ 위치를 지정해서 거까지 움직이게 해준다
				ActionState_.ChangeState("Walk"); //사거리 밖이면 거기까지 걸어가라
			}

			// 어택땅 상태에서 시야에 적이 들어오게 된다면 TargetActor_를 세팅해 주고, 위 if문으로 들어가게 된다.
			// 이 상태에서 적을 물리쳤을 경우, 다시 가던길을 마저 가게 해 주어야 한다.

			std::list<GameEngineCollision*> collist = UnitSightCollision_->CollisionPtrGroup(static_cast<int>(CollisionGroup::Unit));

			auto iter0 = collist.begin();
			auto iter1 = collist.end();

			for (; iter0 != iter1;)
			{
				Unit* colunit = dynamic_cast<Unit*>((*iter0)->GetActor());
				//unsigned int CollisionTargetID=0; // =(*iter0)->GetID();

				if (colunit->Unit_GetTeam() != Team_) // 자신과 다른팀일경우
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

#pragma region 동작 State
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
