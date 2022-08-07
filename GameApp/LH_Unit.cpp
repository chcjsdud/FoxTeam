#include "PreCompile.h"
#include "LH_Unit.h"

Unit::Unit()
	: CharacterType_(CharacterType::AI)
	, CurFowordDir_{ 0.f,0.f,1.f,0.f }
	, KeyDir_{ 0.f,0.f,1.f,0.f }
	, FowordDir_{ 0.f,0.f,1.f,0.f }
	, YRotateSpeed_(900.f)
	//, IsMove_(false)
	, AttackTurm_(0.f)
	//, AttackTime_(0.f)
	, AttackHitTime_(0.f)
	//, AttackLevel_(0)
	, UnitGroundCollision_(nullptr)
	, UnitSightCollision_(nullptr)
	, UnitHitBoxCollision_(nullptr)
	, Target_(nullptr)
	, AttackDist_(0.f)
	, AttackBuffer_(0.f)
	, IsAttack_(false)
	, EnemyInSight_(false)
	, Iscontrolled_(false)

{
	{
		State_.CreateState<Unit>("Idle", this, &Unit::Idle_Start, &Unit::Idle_Update, &Unit::Idle_End);;
		State_.CreateState<Unit>("Walk", this, &Unit::Walk_Start, &Unit::Walk_Update, &Unit::Walk_End);;
		State_.CreateState<Unit>("Attack", this, &Unit::Attack_Start, &Unit::Attack_Update, &Unit::Attack_End);;

		State_.ChangeState("Idle");

		//AIController_.SetMainActor(this);

		//UnitGroundCollision_ = CreateTransformComponent<GameEngineCollision>();
		//UnitSightCollision_ = CreateTransformComponent<GameEngineCollision>();
		//UnitHitBoxCollision_ = CreateTransformComponent<GameEngineCollision>();
		//UnitAttackHitBoxCollision_ = CreateTransformComponent<GameEngineCollision>();
	}
}

Unit::~Unit()
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

void Unit::Start()
{
}

void Unit::Update(float _DeltaTime)
{

}

void Unit::SyncStatus()
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

void Unit::AddBaseStat(Status _Status)
{
	Status_Base_ += _Status;
}

void Unit::SetBaseStat(Status _Status)
{
	Status_Base_ = _Status;
}

void Unit::AddBuff(std::string _Name, Status _Status, float _Time)
{
	Buff* _Buff = new Buff;

	*_Buff = { _Name, _Time , _Status , nullptr };

	BufferList_.insert(std::make_pair(_Name, _Buff));
}

void Unit::RemoveBuff(std::string _Name)
{
	BufferList_.erase(BufferList_.find(_Name));
}

void Unit::RemoveAllBuff(std::string _Name)
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
				SyncStatus();
			}
		}

		iter0++;
	}
}

void Unit::CurDirUpdate(float _DeltaTime)	// 마우스 커서 우클릭한 위치로의 방향 벡터를 계산합니다.
{
	if (Target_ == nullptr)
	{
		FowordDir_ = TargetDir_;
	}


	// RockOnUpdate(_DeltaTime);

	//KeyDirUpdate(_DeltaTime);


}

void Unit::TargetDirUpdate(float _DeltaTime)
{
	if (Target_ != nullptr)
	{
		TargetDir_ = GetTransform()->GetWorldPosition() - Target_->GetTransform()->GetWorldPosition();
		TargetDir_.Normalize3D();
	}
}

//void Unit::RockOnDirUpdate(float _DeltaTime)
//{
//	if (nullptr != Target_)
//	{
//		float4 MoveDir = CalculateTargetDir(Target_->GetTransform()->GetWorldPosition());
//
//		MoveDir.Normalize3D();
//		TargetDir_ = MoveDir;
//		FowordDir_ = MoveDir;
//	}
//}
//
//void Unit::KeyDirUpdate(float _DeltaTime)
//{
//	float4 MoveDir = float4::ZERO;
//
//	bool Key = false;
//
//	//if (true == GameEngineInput::GetInst().Press("D"))
//	//{
//	//	MoveDir.x += 1.f;
//	//	//IsMove_ = true;
//	//	Key = true;
//	//}
//	//if (true == GameEngineInput::GetInst().Press("A"))
//	//{
//	//	MoveDir.x -= 1.f;
//	//	//IsMove_ = true;
//	//	Key = true;
//	//}
//	//if (true == GameEngineInput::GetInst().Press("W"))
//	//{
//	//	MoveDir.z += 1.f;
//	//	//IsMove_ = true;
//	//	Key = true;
//	//}
//	//if (true == GameEngineInput::GetInst().Press("S"))
//	//{
//	//	MoveDir.z -= 1.f;
//	//	//IsMove_ = true;
//	//	Key = true;
//	//}
//	//if (true == IsMove_)
//
//	if (true == Key)
//	{
//		MoveDir.Normalize3D();
//		KeyDir_ = MoveDir;
//
//		if (Target_ == nullptr)
//		{
//			FowordDir_ = KeyDir_;
//		}
//		//IsMove_ = false;
//	}
//}

void Unit::MoveUpdate(float _DeltaTime)
{
	GetTransform()->SetWorldMove(FowordDir_ * Status_Final_.Stat_MoveSpeed_ * _DeltaTime);
}


void Unit::MoveRotateUpdate(float _DeltaTime)
{
	//if (Target_ == nullptr && IsMove_ == false)
	//{
	//	return;
	//}

	if (CurFowordDir_ == FowordDir_)
	{
		return;
	}

	float4 dir = float4::Cross3D(CurFowordDir_, FowordDir_);

	float goaldegree = GameEngineMath::UnitVectorToDegree(FowordDir_.z, FowordDir_.x);

	if (dir.y >= 0.f)
	{

		GetTransform()->AddLocalRotationDegreeY(YRotateSpeed_ * _DeltaTime);
		CurFowordDir_.RotateYDegree(YRotateSpeed_ * _DeltaTime);

		dir = float4::Cross3D(CurFowordDir_, FowordDir_);
		if (dir.y < 0.f)
		{
			GetTransform()->SetLocalRotationDegree({ 0.f,goaldegree,0.f });
			CurFowordDir_ = FowordDir_;
		}
	}

	else if (dir.y < 0.f)
	{
		GetTransform()->AddLocalRotationDegreeY(-YRotateSpeed_ * _DeltaTime);
		CurFowordDir_.RotateYDegree(-YRotateSpeed_ * _DeltaTime);

		dir = float4::Cross3D(CurFowordDir_, FowordDir_);
		if (dir.y > 0.f)
		{
			GetTransform()->SetLocalRotationDegree({ 0.f,goaldegree,0.f });
			CurFowordDir_ = FowordDir_;
		}
	}
}
