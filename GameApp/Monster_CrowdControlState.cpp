#include "PreCompile.h"
#include "Monsters.h"

#include <GameEngine/GameEngineCollision.h>

#include "GameServer.h"
#include "GameClient.h"
#include "MonsterStateChangePacket.h"

#include "LumiaLevel.h"
#include "LumiaMap.h"
#include "NavMesh.h"

void Monsters::StartHitState()
{
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::HIT;
}

void Monsters::UpdateHitState(float _DeltaTime)
{
	// ��� �����(�ǰݸ���� ���� �����Ƿ� �ٷ� �����·� ��ȯ
	ChangeAnimationAndState(MonsterStateType::IDLE);
}

void Monsters::EndHitState()
{
}

void Monsters::StartDeathState()
{
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::DEATH;

	// ������۽� Ÿ������
	CurTarget_ = nullptr;
	CurTargetIndex_ = -1;

	// ��� Flag On
	IsDeath_ = true;
}

void Monsters::UpdateDeathState(float _DeltaTime)
{
	// ���� �ִϸ��̼��� ����Ǹ� �������(���)���·� ��ȯ
	if ("DEATH" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->IsCurrentAnimationEnd())
	{
		// ������� ���� ��ȯ �� ��Ŷ����
		// Ŭ���� ��Ŷ���� �� ��Ŷ���Ž� ������ȯ
		if (true == GameServer::GetInstance()->IsOpened())
		{
			ChangeAnimationAndState(MonsterStateType::DEAD);
		}
		
		// �������������ȯ ��Ŷ ����
		MonsterStateChangePacket ChangePacket;
		ChangePacket.SetIndex(Index_);
		ChangePacket.SetMonsterType(Type_);
		ChangePacket.SetMonsterStateType(MonsterStateType::DEAD);
		ChangePacket.SetMonsterStatInfo(StateInfo_);
		ChangePacket.SetTargetIndex(-1);
		if (true == GameServer::GetInstance()->IsOpened())
		{
			GameServer::GetInstance()->Send(&ChangePacket);
		}
		else if (true == GameClient::GetInstance()->IsConnected())
		{
			GameClient::GetInstance()->Send(&ChangePacket);
		}
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
		// �����ð� �ʱ�ȭ
		StateInfo_.RegenTime_ = StateInfo_.RegenTimeMax_;

		// ������� �ٷ� �������·� ��ȯ �� ��Ŷ ����
		// Ŭ���� ��Ŷ���� �� ��Ŷ���Ž� ������ȯ
		if (true == GameServer::GetInstance()->IsOpened())
		{
			ChangeAnimationAndState(MonsterStateType::REGEN);
		}

		// ����������ȯ ��Ŷ ����
		MonsterStateChangePacket ChangePacket;
		ChangePacket.SetIndex(Index_);
		ChangePacket.SetMonsterType(Type_);
		ChangePacket.SetMonsterStateType(MonsterStateType::REGEN);
		ChangePacket.SetMonsterStatInfo(StateInfo_);
		ChangePacket.SetTargetIndex(-1);
		if (true == GameServer::GetInstance()->IsOpened())
		{
			GameServer::GetInstance()->Send(&ChangePacket);
		}
		else if (true == GameClient::GetInstance()->IsConnected())
		{
			GameClient::GetInstance()->Send(&ChangePacket);
		}
	}
}

void Monsters::EndDeadState()
{
	// MainRenderer Off
	// -> ��� Off
	MainRenderer_->Off();
}

void Monsters::StartStunState()
{
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::STUN;
}

void Monsters::UpdateStunState(float _DeltaTime)
{
	StunTime_ -= _DeltaTime;
	if (0.0f >= StunTime_)
	{
		// ������ȯ
		ChangeAnimationAndState(MonsterStateType::IDLE);

		// ���Ͻð� �ʱ�ȭ
		StunTime_ = 0.0f;
	}
}

void Monsters::EndStunState()
{
}

void Monsters::StartKnockbackState()
{
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::KNOCKBACK;
}

void Monsters::UpdateKnockbackState(float _DeltaTime)
{
	KnockbackTime_ -= _DeltaTime;
	if (0.0f >= KnockbackTime_)
	{
		// ������ȯ
		ChangeAnimationAndState(MonsterStateType::IDLE);
		
		// �˹����� �ʱ�ȭ
		KnockbackTime_ = 0.0f;
		KnockbackSpeed_ = float4::ZERO;

		return;
	}

	transform_.AddWorldPosition(KnockbackSpeed_ * _DeltaTime);
}

void Monsters::EndKnockbackState()
{
}

void Monsters::StartWallSlamState()
{
	// ���� ���� ����
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::WALLSLAM;
}

void Monsters::UpdateWallSlamState(float _DeltaTime)
{
	KnockbackTime_ -= _DeltaTime;
	if (0.0f >= KnockbackTime_)
	{
		ChangeAnimationAndState(MonsterStateType::IDLE);

		// �����ϰ��� ���� �ʱ�ȭ
		KnockbackTime_ = 0.0f;
		KnockbackSpeed_ = float4::ZERO;

		return;
	}

	float4 moveSpeed = KnockbackSpeed_ * _DeltaTime;
	float4 nextMovePosition = transform_.GetWorldPosition() + moveSpeed;

	float temp = 0.0f;
	if (true == CurrentMap_->GetNavMesh()->CheckIntersects(nextMovePosition + float4{ 0.0f, FT::Map::MAX_HEIGHT, 0.0f }, float4::DOWN, temp))
	{
		GetTransform()->SetWorldPosition(nextMovePosition);
	}
	else
	{
		// ���ϻ��� ��ȯ
		Stun(StunTime_);
	}
}

void Monsters::EndWallSlamState()
{
}
