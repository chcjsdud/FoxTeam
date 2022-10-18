#include "PreCompile.h"
#include "Monsters.h"

#include <GameEngine/GameEngineCollision.h>

#include "GameServer.h"
#include "GameClient.h"
#include "MonsterStateChangePacket.h"

void Monsters::StartHitState()
{
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::HIT;
}

void Monsters::UpdateHitState(float _DeltaTime)
{
	// ��� ����� ������ ����
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
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::DEATH;

	// �ǰ��������� Flag On
	GetHitOffFlag_ = true;
}

void Monsters::UpdateDeathState(float _DeltaTime)
{
	// ���� �ִϸ��̼��� ����Ǹ� �������(���)���·� ��ȯ
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
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::DEAD;
}

void Monsters::UpdateDeadState(float _DeltaTime)
{
	// �ش� ���Ͱ� ����Ͽ����Ƿ� �������ð�üũ
	StateInfo_.RegenTime_ -= _DeltaTime;
	if (0.0f >= StateInfo_.RegenTime_)
	{
		// �������·� ��ȯ
		ChangeAnimationAndState(MonsterStateType::REGEN);

		// �����ð� �ʱ�ȭ
		StateInfo_.RegenTime_ = StateInfo_.RegenTimeMax_;
	}
}

void Monsters::EndDeadState()
{
	// MainRenderer Off
	// -> ��� Off
	MainRenderer_->Off();
}
