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
	// ����(ȣ��Ʈ)�� ���ؼ� �۽ŵǹǷ� Ŭ��(�Խ�Ʈ)������ ó��
	if (false == _bServer)
	{
		// Get Current Level(LumiaLevel Downcasting)
		GameEngineLevel* CurrentLevel = GameEngineCore::CurrentLevel();
		LumiaLevel* PlayLevel = dynamic_cast<LumiaLevel*>(CurrentLevel);
		if (nullptr == PlayLevel)
		{
			GameEngineDebug::MsgBoxError("��������� ���緹���� ã���������ϴ�!!!!!");
			return;
		}

		// ��̾Ʒ����� ��ġ�� ���͵��� �ϰ������� ������ȯ
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
