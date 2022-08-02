#include "PreCompile.h"
#include "LH_Unit.h"

Unit::Unit() 
	: CharacterType_(CharacterType::AI)
	, CurFowordDir_{ 0.f,0.f,1.f,0.f }
	, KeyDir_{ 0.f,0.f,1.f,0.f }
	, FowordDir_{ 0.f,0.f,1.f,0.f }
	, YRotateSpeed_(900.f)
	, IsMove_(false)
	, AttackTurm_(0.f)
	, AttackTime_(0.f)
	, AttackHitTime_(0.f)
	, AttackLevel_(0)
	, UnitGroundCollision_(nullptr)
	, UnitSightCollision_(nullptr)
	, UnitHitBoxCollision_(nullptr)
	, UnitAttackHitBoxCollision_(nullptr)
	//, FBXRenderer_(nullptr)

{
	{
		State_.CreateState<Unit>("Idle", this, &Unit::Idle_Start, &Unit::Idle_Update, &Unit::Idle_End);;
		State_.CreateState<Unit>("Walk", this, &Unit::Walk_Start, &Unit::Walk_Update, &Unit::Walk_End);;
		State_.CreateState<Unit>("Run", this, &Unit::Run_Start, &Unit::Run_Update, &Unit::Run_End);;
		State_.CreateState<Unit>("Attack", this, &Unit::Attack_Start, &Unit::Attack_Update, &Unit::Attack_End);;

		State_.ChangeState("Idle");

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
	UpdateBuff(_DeltaTime);
}

void Unit::SyncStatus()
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
	
	*_Buff = { _Time , _Status };

	BufferList_.insert(std::make_pair( _Name, _Buff));
}

void Unit::RemoveBuff(std::string _Name)
{
	BufferList_.erase(BufferList_.find(_Name));
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
