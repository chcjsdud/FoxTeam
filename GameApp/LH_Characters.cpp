#include "PreCompile.h"
#include "LH_Characters.h"

Characters* Characters::MainPlayer = nullptr;

Characters* Characters::GetMainPlayer()
{
	return MainPlayer;
}

void Characters::SetMainPlayer(Characters* _Player)
{
	if (nullptr != _Player)
	{
		// 메인플레이어 변경
		MainPlayer = _Player;

		// 메인플레이어로 변경되었으므로 타입변경
		MainPlayer->CharacterType_ = CharacterType::MAIN;
	}
}

Characters::Characters() 
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
		State_.CreateState<Characters>("Idle", this, &Characters::Idle_Start, &Characters::Idle_Update, &Characters::Idle_End);;
		State_.CreateState<Characters>("Walk", this, &Characters::Walk_Start, &Characters::Walk_Update, &Characters::Walk_End);;
		State_.CreateState<Characters>("Run", this, &Characters::Run_Start, &Characters::Run_Update, &Characters::Run_End);;
		State_.CreateState<Characters>("Attack", this, &Characters::Attack_Start, &Characters::Attack_Update, &Characters::Attack_End);;

		State_.ChangeState("Idle");
	}
}

Characters::~Characters()
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

void Characters::Start()
{
}

void Characters::Update(float _DeltaTime)
{
}

void Characters::StateInit()
{

}

void Characters::SyncStatus()
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

	// 최종적으로 사용되는 스탯은 PlayerStatusFinal_임
	// PlayerStatusFinal_은 PlayerStatusBase_에 더해야할 스텟을 먼저 더하고 마지막에 곱해야할 스텟을 곱해주어 계산함

	Status_Final_ = Status_Base_ + Status_Add_;
	Status_Final_ *= Status_Mult_;
}
