#include "PreCompile.h"
#include "Monsters.h"

#include <GameEngine/GameEngineCollision.h>

#include "GameServer.h"
#include "GameClient.h"
#include "MonsterStateChangePacket.h"

void Monsters::StartHitState()
{
	// ���� ���� ����
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
	CurStateType_ = MonsterStateType::DEATH;
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
	CurStateType_ = MonsterStateType::DEAD;

	// ���η����� ������ Off, �浹ü Off
	MainRenderer_->Off();
	BodyCollider_->Off();
}

void Monsters::UpdateDeadState(float _DeltaTime)
{
	// �ش� ���Ͱ� ����Ͽ����Ƿ� �������ð�üũ
	float RegenTime = StateInfo_.RegenTime_ -= _DeltaTime;
	if (0.0f >= RegenTime)
	{
		// �������·� ��ȯ
		ChangeAnimationAndState(MonsterStateType::REGEN);

		// ������ȯ ��Ŷ����
		MonsterStateChangePacket Packet;
		Packet.SetIndex(Index_);
		Packet.SetMonsterType(Type_);
		Packet.SetMonsterStateType(MonsterStateType::REGEN);
		Packet.SetMonsterStatInfo(StateInfo_);
		Packet.SetTargetIndex(-1);
		if (true == GameServer::GetInstance()->IsOpened())
		{
			GameServer::GetInstance()->Send(&Packet);
		}
		else if (true == GameClient::GetInstance()->IsConnected())
		{
			GameClient::GetInstance()->Send(&Packet);
		}
	}
}

void Monsters::EndDeadState()
{
}
