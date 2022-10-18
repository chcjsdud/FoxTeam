#include "PreCompile.h"
#include "Monsters.h"

#include <GameEngine/GameEngineCollision.h>

#include "Enums.h"

void Monsters::StartAppearState()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::APPEAR;
}

void Monsters::UpdateAppearState(float _DeltaTime)
{
	// 첫등장애니메이션 모션종료시 대기상태로 돌입
	if ("APPEAR" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->CheckCurrentAnimationEnd())
	{
		ChangeAnimationAndState(MonsterStateType::IDLE);
	}
}

void Monsters::EndAppearState()
{
}

void Monsters::StartRegenState()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::REGEN;

	// 스텟 초기정보로 초기화
	StateInfo_.HP_ = StateInfo_.HPMax_;
	StateInfo_.SP_ = StateInfo_.SPMax_;
	StateInfo_.HomingInstinctValue_ = StateInfo_.HomingInstinctValueMax_;
	StateInfo_.RegenTime_ = StateInfo_.RegenTimeMax_;

	// 타겟정보 초기화
	CurTarget_ = nullptr;
	CurTargetIndex_ = -1;

	// 초기스폰위치로 셋팅 후 재등장을 위한 준비
	GetTransform()->SetLocalPosition(StateInfo_.NestPosition_);
	MainRenderer_->On();
}

void Monsters::UpdateRegenState(float _DeltaTime)
{
	// 리젠완료시 대기상태 돌입
	if ("REGEN" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->CheckCurrentAnimationEnd())
	{
		ChangeAnimationAndState(MonsterStateType::IDLE);
	}
}

void Monsters::EndRegenState()
{
}

void Monsters::StartIdleState()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::IDLE;
}

void Monsters::UpdateIdleState(float _DeltaTime)
{
	// ...

}

void Monsters::EndIdleState()
{
}

void Monsters::StartChaseState()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::CHASE;

	// 예외처리
	// -> 타겟없으면 대기상태 재돌입
	if (nullptr == CurTarget_)
	{
		ChangeAnimationAndState(MonsterStateType::IDLE);
	}
}

void Monsters::UpdateChaseState(float _DeltaTime)
{
	// 타겟이 존재하면
	if (nullptr != CurTarget_)
	{
		// 타겟 추적시작



		// 타겟 추적중 공격범위내 타겟이 진입시 공격상태 전환



		// 타겟 추적중 귀소본능수치 계속하여 감소
	}
}

void Monsters::EndChaseState()
{
}

void Monsters::StartHomingInstinctState()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::HOMINGINSTINCT;

	// 예외처리
	if (nullptr != CurTarget_)
	{
		CurTarget_ = nullptr;
		CurTargetIndex_ = -1;
	}

	// 귀환중 피격무시
	BodyCollider_->Off();
}

void Monsters::UpdateHomingInstinctState(float _DeltaTime)
{
	// 둥지위치로 이동
	// -> 건물무시???? 플레이어의 시야범위밖이면 워프???



	// 둥지이동 위치 도달시 대기상태 돌입

}

void Monsters::EndHomingInstinctState()
{
	// 귀환완료시 피격상태 체크를 위해 충돌체 다시 On
	BodyCollider_->On();
}
