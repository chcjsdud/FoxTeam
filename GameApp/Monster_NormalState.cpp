#include "PreCompile.h"
#include "Monsters.h"

#include <GameEngine/GameEngineCollision.h>

#include "Enums.h"
#include "Character.h"

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

	// �ǰ��������� Flag Off
	GetHitOffFlag_ = false;
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
	// Ÿ������ ���� �Ǵ�
	if (nullptr != CurTarget_)
	{
		// Ÿ���� �����Ҷ� Ÿ�ٰ� ���� �Ÿ� ����
		float4 TargetPosition = CurTarget_->GetTransform()->GetWorldPosition();
		float4 MyPosition = transform_.GetWorldPosition();
		float Dist = (TargetPosition - MyPosition).Len3D();

		// �Ÿ��� ���ݹ������� �϶� ���ݻ��·� ����
		if (Dist <= StateInfo_.AttackRange_)
		{
			ChangeAnimationAndState(MonsterStateType::ATK01);
			//ChangeAnimationAndState(MonsterStateType::ATK02);
			//ChangeAnimationAndState(MonsterStateType::SKILLATTACK);
		}
		// �Ÿ��� ���ݹ����� ����ִٸ� �������·� ����
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
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::CHASE;

	// ����ó��
	// -> Ÿ�پ����� ������ �絹��
	if (nullptr == CurTarget_)
	{
		ChangeAnimationAndState(MonsterStateType::IDLE);
	}

	// �̵�����
	StartMove(CurTarget_->GetTransform()->GetWorldPosition());
}

void Monsters::UpdateChaseState(float _DeltaTime)
{
	// �̵�����
	UpdateMove(_DeltaTime);
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
	GetHitOffFlag_ = true;

	// �̵�����
	StartMove(StateInfo_.NestPosition_);
}

void Monsters::UpdateHomingInstinctState(float _DeltaTime)
{
	UpdateMove(_DeltaTime);
}

void Monsters::EndHomingInstinctState()
{
	// ��ȯ�Ϸ�� �ǰݻ��� üũ�� ���� �浹ü �ٽ� On
	GetHitOffFlag_ = false;
}
