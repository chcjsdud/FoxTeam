#include "Precompile.h"
#include "MonsterStateChangePacket.h"

#include <GameEngine/GameEngineCore.h>

#include "ePacketID.h"
#include "Enums.h"

#include "LumiaLevel.h"

void MonsterStateChangePacket::userSerialize()
{
	serializer_ << static_cast<int>(Type_);
	serializer_ << static_cast<int>(StateType_);
}

void MonsterStateChangePacket::userDeserialize()
{
	int rcvType = 0;
	serializer_ >> rcvType;
	Type_ = static_cast<MonsterType>(rcvType);

	int rcvStateType = 0;
	serializer_ >> rcvStateType;
	StateType_ = static_cast<MonsterStateType>(rcvStateType);
}

void MonsterStateChangePacket::initPacketID()
{
	SetPacketID(ePacketID::MonsterStatePacket);
}

GameEnginePacketBase* MonsterStateChangePacket::getUserObject()
{
	return new MonsterStateChangePacket;
}

void MonsterStateChangePacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
	// 서버(호스트)에 의해서 송신되므로 클라(게스트)에서만 처리
	if (false == _bServer)
	{
		// Get Current Level(LumiaLevel Downcasting)
		GameEngineLevel* CurrentLevel = GameEngineCore::CurrentLevel();
		LumiaLevel* PlayLevel = dynamic_cast<LumiaLevel*>(CurrentLevel);
		if (nullptr == PlayLevel)
		{
			GameEngineDebug::MsgBoxError("현재게임의 현재레벨을 찾을수없습니다!!!!!");
			return;
		}

		// 루미아레벨에 배치된 몬스터들을 일괄적으로 상태전환
		switch (StateType_)
		{
			case MonsterStateType::APPEAR:
			{
				PlayLevel->GuestMonsterFirstAppear(Type_);
				break;
			}
			case MonsterStateType::REGEN:
			{
				break;
			}
			case MonsterStateType::IDLE:
			{
				break;
			}
			case MonsterStateType::RUN:
			{
				break;
			}
			case MonsterStateType::HOMINGINSTINCT:
			{
				break;
			}
			case MonsterStateType::CHASE:
			{
				break;
			}
			case MonsterStateType::HIT:
			{
				break;
			}
			case MonsterStateType::DEATH:
			{
				break;
			}
			case MonsterStateType::DEAD:
			{
				break;
			}
			case MonsterStateType::ATK01:
			{
				break;
			}
			case MonsterStateType::ATK02:
			{
				break;
			}
			case MonsterStateType::SKILLATTACK:
			{
				break;
			}
		}
	}
}

MonsterStateChangePacket::MonsterStateChangePacket()
	: Type_(MonsterType::NONE)
	, StateType_(MonsterStateType::NONE)
{
}

MonsterStateChangePacket::~MonsterStateChangePacket()
{
}
