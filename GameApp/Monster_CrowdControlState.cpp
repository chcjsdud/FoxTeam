#include "PreCompile.h"
#include "Monsters.h"

#include <GameEngine/GameEngineCollision.h>

#include "GameServer.h"
#include "GameClient.h"
#include "MonsterStateChangePacket.h"

void Monsters::StartHitState()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::HIT;
}

void Monsters::UpdateHitState(float _DeltaTime)
{
	// 모션 종료시(피격모션이 따로 없으므로 바로 대기상태로 전환
	ChangeAnimationAndState(MonsterStateType::IDLE);
}

void Monsters::EndHitState()
{
}

void Monsters::StartDeathState()
{
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::DEATH;

	// 사망시작시 타겟제거
	CurTarget_ = nullptr;
	CurTargetIndex_ = -1;

	// 사망 Flag On
	IsDeath_ = true;
}

void Monsters::UpdateDeathState(float _DeltaTime)
{
	// 현재 애니메이션이 종료되면 리젠대기(사망)상태로 전환
	if ("DEATH" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->CheckCurrentAnimationEnd())
	{
		// 서버라면 상태 전환 후 패킷전송
		// 클라라면 패킷전송 후 패킷수신시 상태전환
		if (true == GameServer::GetInstance()->IsOpened())
		{
			ChangeAnimationAndState(MonsterStateType::DEAD);
		}
		
		// 완전사망상태전환 패킷 전송
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
	// 현재 상태 지정
	PrevStateType_ = CurStateType_;
	CurStateType_ = MonsterStateType::DEAD;
}

void Monsters::UpdateDeadState(float _DeltaTime)
{
	// 해당 몬스터가 사망하였으므로 리젠대기시간체크
	StateInfo_.RegenTime_ -= _DeltaTime;
	if (0.0f >= StateInfo_.RegenTime_)
	{
		// 리젠시간 초기화
		StateInfo_.RegenTime_ = StateInfo_.RegenTimeMax_;

		// 서버라면 바로 리젠상태로 전환 후 패킷 전송
		// 클라라면 패킷전송 후 패킷수신시 상태전환
		if (true == GameServer::GetInstance()->IsOpened())
		{
			ChangeAnimationAndState(MonsterStateType::REGEN);
		}

		// 리젠상태전환 패킷 전송
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
	// -> 잠시 Off
	MainRenderer_->Off();
}
