#include "PreCompile.h"
#include "Monsters.h"

#include <GameEngine/GameEngineCollision.h>

#include "GameServer.h"
#include "GameClient.h"
#include "MonsterStateChangePacket.h"

void Monsters::StartHitState()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::HIT;
}

void Monsters::UpdateHitState(float _DeltaTime)
{
	// 모션 종료시 대기상태 돌입
	if ("HIT" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->CheckCurrentAnimationEnd())
	{
		ChangeAnimationAndState(MonsterStateType::IDLE);
	}
}

void Monsters::EndHitState()
{
}

void Monsters::StartDeathState()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::DEATH;

	// 피격판정무시 Flag On
	GetHitOffFlag_ = true;
}

void Monsters::UpdateDeathState(float _DeltaTime)
{
	// 현재 애니메이션이 종료되면 리젠대기(사망)상태로 전환
	if ("DEATH" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->CheckCurrentAnimationEnd())
	{
		ChangeAnimationAndState(MonsterStateType::DEAD);
	}
}

void Monsters::EndDeathState()
{
}

void Monsters::StartDeadState()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::DEAD;
}

void Monsters::UpdateDeadState(float _DeltaTime)
{
	// 해당 몬스터가 사망하였으므로 리젠대기시간체크
	StateInfo_.RegenTime_ -= _DeltaTime;
	if (0.0f >= StateInfo_.RegenTime_)
	{
		// 리젠상태로 전환
		ChangeAnimationAndState(MonsterStateType::REGEN);

		// 리젠시간 초기화
		StateInfo_.RegenTime_ = StateInfo_.RegenTimeMax_;
	}
}

void Monsters::EndDeadState()
{
	// MainRenderer Off
	// -> 잠시 Off
	MainRenderer_->Off();
}
