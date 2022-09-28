#include "PreCompile.h"
#include "Monsters.h"

void Monsters::StartHitState()
{
	// 현재 상태 지정
	CurStateType_ = MonsterStateType::HIT;

	// 
}

void Monsters::UpdateHitState(float _DeltaTime)
{

}

void Monsters::EndHitState()
{
}

void Monsters::StartDeathState()
{
	// 현재 상태 지정
	CurStateType_ = MonsterStateType::DEATH;

	// 
}

void Monsters::UpdateDeathState(float _DeltaTime)
{
	// 현재 애니메이션이 종료되면 리젠대기(사망)상태로 전환



}

void Monsters::EndDeathState()
{
}

void Monsters::StartDeadState()
{
	// 현재 상태 지정
	CurStateType_ = MonsterStateType::DEAD;

	// 

}

void Monsters::UpdateDeadState(float _DeltaTime)
{
	// 해당 몬스터가 사망하였으므로 리젠대기시간체크
	float RegenTime = StateInfo_.RegenTime_ -= _DeltaTime;
	if (0.0f >= RegenTime)
	{
		// 리젠상태로 전환
		NormalState_ << "REGEN";
		MainState_ << "NORMAL";
		
		// 리젠시간 초기화
		StateInfo_.RegenTime_ = StateInfo_.RegenTimeMax_;
	}
}

void Monsters::EndDeadState()
{
}
