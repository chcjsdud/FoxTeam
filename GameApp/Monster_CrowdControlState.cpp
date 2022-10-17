#include "PreCompile.h"
#include "Monsters.h"

#include <GameEngine/GameEngineCollision.h>

#include "GameServer.h"
#include "GameClient.h"
#include "MonsterStateChangePacket.h"

void Monsters::StartHitState()
{
	// 현재 상태 지정
	CurStateType_ = MonsterStateType::HIT;
}

void Monsters::UpdateHitState(float _DeltaTime)
{
	// 모션 종료시 대기상태 돌입
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
	// 현재 상태 지정
	CurStateType_ = MonsterStateType::DEATH;
}

void Monsters::UpdateDeathState(float _DeltaTime)
{
	// 현재 애니메이션이 종료되면 리젠대기(사망)상태로 전환
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
	// 현재 상태 지정
	CurStateType_ = MonsterStateType::DEAD;

	// 메인렌더러 렌더링 Off, 충돌체 Off
	MainRenderer_->Off();
	BodyCollider_->Off();
}

void Monsters::UpdateDeadState(float _DeltaTime)
{
	// 해당 몬스터가 사망하였으므로 리젠대기시간체크
	float RegenTime = StateInfo_.RegenTime_ -= _DeltaTime;
	if (0.0f >= RegenTime)
	{
		// 리젠상태로 전환
		ChangeAnimationAndState(MonsterStateType::REGEN);

		// 상태전환 패킷전송
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
