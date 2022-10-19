#include "PreCompile.h"
#include "Monsters.h"

#include <GameEngine/GameEngineCollision.h>

#include "Enums.h"
#include "Character.h"

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

	// 피격판정무시 Flag Off
	GetHitOffFlag_ = false;
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
	// 타겟지정 유무 판단
	if (nullptr != CurTarget_)
	{
		// 타겟이 존재할때 타겟과 나의 거리 측정
		float4 TargetPosition = CurTarget_->GetTransform()->GetWorldPosition();
		float4 MyPosition = transform_.GetWorldPosition();
		float Dist = (TargetPosition - MyPosition).Len3D();

		// 거리가 공격범위내에 일때 공격상태로 변경
		if (Dist <= StateInfo_.AttackRange_)
		{
			ChangeAnimationAndState(MonsterStateType::ATK01);
			//ChangeAnimationAndState(MonsterStateType::ATK02);
			//ChangeAnimationAndState(MonsterStateType::SKILLATTACK);
		}
		// 거리가 공격범위를 벗어나있다면 추적상태로 변경
		else
		{
			ChangeAnimationAndState(MonsterStateType::CHASE);
		}
	}
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

	// 이동시작
	StartMove(CurTarget_->GetTransform()->GetWorldPosition());
}

void Monsters::UpdateChaseState(float _DeltaTime)
{
	// 이동갱신
	UpdateMove(_DeltaTime);
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
	GetHitOffFlag_ = true;

	// 이동시작
	StartMove(StateInfo_.NestPosition_);
}

void Monsters::UpdateHomingInstinctState(float _DeltaTime)
{
	UpdateMove(_DeltaTime);
}

void Monsters::EndHomingInstinctState()
{
	// 귀환완료시 피격상태 체크를 위해 충돌체 다시 On
	GetHitOffFlag_ = false;
}
