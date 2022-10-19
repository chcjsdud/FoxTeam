#include "PreCompile.h"
#include "Monsters.h"

void Monsters::StartAttack01State()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::ATK01;
}

void Monsters::UpdateAttack01State(float _DeltaTime)
{
	// 공격모션 종료시 대기상태로 전환
	if ("ATK01" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->CheckCurrentAnimationEnd())
	{
		ChangeAnimationAndState(MonsterStateType::IDLE);
	}
}

void Monsters::EndAttack01State()
{
}

void Monsters::StartAttack02State()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::ATK02;
}

void Monsters::UpdateAttack02State(float _DeltaTime)
{
	// 공격모션 종료시 대기상태로 전환
	if ("ATK02" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->CheckCurrentAnimationEnd())
	{
		ChangeAnimationAndState(MonsterStateType::IDLE);
	}
}

void Monsters::EndAttack02State()
{
}

void Monsters::StartSkillAttackState()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::SKILLATTACK;
}

void Monsters::UpdateSkillAttackState(float _DeltaTime)
{
	// 스킬공격모션 종료시 대기상태로 전환
	if ("SKILLATTACK" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->CheckCurrentAnimationEnd())
	{
		ChangeAnimationAndState(MonsterStateType::IDLE);
	}
}

void Monsters::EndSkillAttackState()
{
}
