#include "PreCompile.h"
#include "Monsters.h"

void Monsters::StartAttack01State()
{
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
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
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
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
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::SKILLATTACK;

	// 
}

void Monsters::UpdateSkillAttackState(float _DeltaTime)
{

}

void Monsters::EndSkillAttackState()
{

}
