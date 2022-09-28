#include "PreCompile.h"
#include "Monsters.h"

void Monsters::StartAttack01State()
{
	// 현재 상태 지정
	CurStateType_ = MonsterStateType::ATK01;

	// 
}

void Monsters::UpdateAttack01State(float _DeltaTime)
{

}

void Monsters::EndAttack01State()
{

}

void Monsters::StartAttack02State()
{
	// 현재 상태 지정
	CurStateType_ = MonsterStateType::ATK02;

	// 
}

void Monsters::UpdateAttack02State(float _DeltaTime)
{

}

void Monsters::EndAttack02State()
{

}

void Monsters::StartSkillAttackState()
{
	// 현재 상태 지정
	CurStateType_ = MonsterStateType::SKILLATTACK;

	// 
}

void Monsters::UpdateSkillAttackState(float _DeltaTime)
{

}

void Monsters::EndSkillAttackState()
{

}
