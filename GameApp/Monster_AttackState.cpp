#include "PreCompile.h"
#include "Monsters.h"

#include <GameEngine/GameEngineCollision.h>

void Monsters::StartAttack01State()
{
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::ATK01;

	// ������ Flag On
	IsAttack_ = true;
	AtkCollider_->On();
}

void Monsters::UpdateAttack01State(float _DeltaTime)
{
	// ���ݸ�� ����� �����·� ��ȯ
	if ("ATK01" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->IsCurrentAnimationEnd())
	{
		ChangeAnimationAndState(MonsterStateType::IDLE);
	}
}

void Monsters::EndAttack01State()
{
}

void Monsters::StartAttack02State()
{
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::ATK02;

	// ������ Flag On
	IsAttack_ = true;
	AtkCollider_->On();
}

void Monsters::UpdateAttack02State(float _DeltaTime)
{
	// ���ݸ�� ����� �����·� ��ȯ
	if ("ATK02" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->IsCurrentAnimationEnd())
	{
		ChangeAnimationAndState(MonsterStateType::IDLE);
	}
}

void Monsters::EndAttack02State()
{
}

void Monsters::StartSkillAttackState()
{
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::SKILLATTACK;
}

void Monsters::UpdateSkillAttackState(float _DeltaTime)
{
	// ���� Ÿ�Ժ� ��ų������ �ٸ��Ƿ� ���������Լ��� ������ �Լ� ����
	SkillAttackProcessing(_DeltaTime);
}

void Monsters::EndSkillAttackState()
{
}
