#include "PreCompile.h"
#include "Monsters.h"

void Monsters::StartHitState()
{
	// ���� ���� ����
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
	// ���� ���� ����
	CurStateType_ = MonsterStateType::DEATH;

	// 
}

void Monsters::UpdateDeathState(float _DeltaTime)
{
	// ���� �ִϸ��̼��� ����Ǹ� �������(���)���·� ��ȯ



}

void Monsters::EndDeathState()
{
}

void Monsters::StartDeadState()
{
	// ���� ���� ����
	CurStateType_ = MonsterStateType::DEAD;

	// 

}

void Monsters::UpdateDeadState(float _DeltaTime)
{
	// �ش� ���Ͱ� ����Ͽ����Ƿ� �������ð�üũ
	float RegenTime = StateInfo_.RegenTime_ -= _DeltaTime;
	if (0.0f >= RegenTime)
	{
		// �������·� ��ȯ
		NormalState_ << "REGEN";
		MainState_ << "NORMAL";
		
		// �����ð� �ʱ�ȭ
		StateInfo_.RegenTime_ = StateInfo_.RegenTimeMax_;
	}
}

void Monsters::EndDeadState()
{
}
