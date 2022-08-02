#include "PreCompile.h"
#include "LH_Unit.h"

Unit::Unit() 
	: CharacterType_(CharacterType::AI)
	, SkillInfo_{}
	, CurFowordDir_{ 0.f,0.f,1.f,0.f }
	, KeyDir_{ 0.f,0.f,1.f,0.f }
	, FowordDir_{ 0.f,0.f,1.f,0.f }
	, YRotateSpeed_(900.f)
	, IsMove_(false)
	, AttackTurm_(0.f)
	, AttackTime_(0.f)
	, AttackHitTime_(0.f)
	, AttackLevel_(0)
{
	{
		State_.CreateState<Unit>("Idle", this, &Unit::Idle_Start, &Unit::Idle_Update, &Unit::Idle_End);;
		State_.CreateState<Unit>("Walk", this, &Unit::Walk_Start, &Unit::Walk_Update, &Unit::Walk_End);;
		State_.CreateState<Unit>("Run", this, &Unit::Run_Start, &Unit::Run_Update, &Unit::Run_End);;
		State_.CreateState<Unit>("Attack", this, &Unit::Attack_Start, &Unit::Attack_Update, &Unit::Attack_End);;

		State_.ChangeState("Idle");
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

void Unit::StateInit()
{

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
		if (true == iter0->second->Stat_IsMult_)
		{
			Status_Mult_ += *(iter0->second);
		}
		else
		{
			Status_Add_ += *(iter0->second);
		}
		iter0++;
	}

	// ���������� ���Ǵ� ������ PlayerStatusFinal_��
	// PlayerStatusFinal_�� PlayerStatusBase_�� ���ؾ��� ������ ���� ���ϰ� �������� ���ؾ��� ������ �����־� �����

	Status_Final_ = Status_Base_ + Status_Add_;
	Status_Final_ *= Status_Mult_;
}
