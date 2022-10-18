#include "PreCompile.h"
#include "Monsters.h"

#include <GameEngine/GameEngineCollision.h>

#include "Enums.h"

void Monsters::StartAppearState()
{
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::APPEAR;
}

void Monsters::UpdateAppearState(float _DeltaTime)
{
	// ù����ִϸ��̼� �������� �����·� ����
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
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::REGEN;

	// ���� �ʱ������� �ʱ�ȭ
	StateInfo_.HP_ = StateInfo_.HPMax_;
	StateInfo_.SP_ = StateInfo_.SPMax_;
	StateInfo_.HomingInstinctValue_ = StateInfo_.HomingInstinctValueMax_;
	StateInfo_.RegenTime_ = StateInfo_.RegenTimeMax_;

	// Ÿ������ �ʱ�ȭ
	CurTarget_ = nullptr;
	CurTargetIndex_ = -1;

	// �ʱ⽺����ġ�� ���� �� ������� ���� �غ�
	GetTransform()->SetLocalPosition(StateInfo_.NestPosition_);
	MainRenderer_->On();
}

void Monsters::UpdateRegenState(float _DeltaTime)
{
	// �����Ϸ�� ������ ����
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
	// ���� ���� ����
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
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::CHASE;

	// ����ó��
	// -> Ÿ�پ����� ������ �絹��
	if (nullptr == CurTarget_)
	{
		ChangeAnimationAndState(MonsterStateType::IDLE);
	}
}

void Monsters::UpdateChaseState(float _DeltaTime)
{
	// Ÿ���� �����ϸ�
	if (nullptr != CurTarget_)
	{
		// Ÿ�� ��������



		// Ÿ�� ������ ���ݹ����� Ÿ���� ���Խ� ���ݻ��� ��ȯ



		// Ÿ�� ������ �ͼҺ��ɼ�ġ ����Ͽ� ����
	}
}

void Monsters::EndChaseState()
{
}

void Monsters::StartHomingInstinctState()
{
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::HOMINGINSTINCT;

	// ����ó��
	if (nullptr != CurTarget_)
	{
		CurTarget_ = nullptr;
		CurTargetIndex_ = -1;
	}

	// ��ȯ�� �ǰݹ���
	BodyCollider_->Off();
}

void Monsters::UpdateHomingInstinctState(float _DeltaTime)
{
	// ������ġ�� �̵�
	// -> �ǹ�����???? �÷��̾��� �þ߹������̸� ����???



	// �����̵� ��ġ ���޽� ������ ����

}

void Monsters::EndHomingInstinctState()
{
	// ��ȯ�Ϸ�� �ǰݻ��� üũ�� ���� �浹ü �ٽ� On
	BodyCollider_->On();
}
