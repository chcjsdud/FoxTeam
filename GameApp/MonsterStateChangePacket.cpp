#include "Precompile.h"
#include "MonsterStateChangePacket.h"

#include <GameEngine/GameEngineCore.h>

#include "ePacketID.h"
#include "Enums.h"

#include "LumiaLevel.h"
#include "Character.h"
#include "Monsters.h"

void MonsterStateChangePacket::userSerialize()
{
	// Monster Distinction Related
	serializer_ << Index_;
	serializer_ << static_cast<int>(Type_);
	serializer_ << static_cast<int>(StateType_);

	// Monster Stat Update Related
	serializer_ << StatInfo_.Level_;
	serializer_ << StatInfo_.HPMax_;
	serializer_ << StatInfo_.HP_;
	serializer_ << StatInfo_.HPRegenPercent_;
	serializer_ << StatInfo_.SPMax_;
	serializer_ << StatInfo_.SP_;
	serializer_ << StatInfo_.SPRegenPercent_;
	serializer_ << StatInfo_.Defence_;
	serializer_ << StatInfo_.NestPosition_;
	serializer_ << StatInfo_.HomingInstinctValueMax_;
	serializer_ << StatInfo_.HomingInstinctValue_;
	serializer_ << StatInfo_.AttackRange_;
	serializer_ << StatInfo_.DetectRange_;
	serializer_ << StatInfo_.AttackSpeed_;
	serializer_ << StatInfo_.MoveSpeed_;
	serializer_ << StatInfo_.SkillCoolDown_;
	serializer_ << StatInfo_.RegenTimeMax_;
	serializer_ << StatInfo_.RegenTime_;

	// Target(Character) Related
	serializer_ << TargetIndex_;
}

void MonsterStateChangePacket::userDeserialize()
{
	// Monster Related
	serializer_ >> Index_;

	int rcvType = 0;
	serializer_ >> rcvType;
	Type_ = static_cast<MonsterType>(rcvType);

	int rcvStateType = 0;
	serializer_ >> rcvStateType;
	StateType_ = static_cast<MonsterStateType>(rcvStateType);

	// Monster Stat Update Related
	serializer_ >> StatInfo_.Level_;
	serializer_ >> StatInfo_.HPMax_;
	serializer_ >> StatInfo_.HP_;
	serializer_ >> StatInfo_.HPRegenPercent_;
	serializer_ >> StatInfo_.SPMax_;
	serializer_ >> StatInfo_.SP_;
	serializer_ >> StatInfo_.SPRegenPercent_;
	serializer_ >> StatInfo_.Defence_;
	serializer_ >> StatInfo_.NestPosition_;
	serializer_ >> StatInfo_.HomingInstinctValueMax_;
	serializer_ >> StatInfo_.HomingInstinctValue_;
	serializer_ >> StatInfo_.AttackRange_;
	serializer_ >> StatInfo_.DetectRange_;
	serializer_ >> StatInfo_.AttackSpeed_;
	serializer_ >> StatInfo_.MoveSpeed_;
	serializer_ >> StatInfo_.SkillCoolDown_;
	serializer_ >> StatInfo_.RegenTimeMax_;
	serializer_ >> StatInfo_.RegenTime_;

	// Target(Character) Related
	serializer_ >> TargetIndex_;
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
	// Get Current Level(LumiaLevel Downcasting)
	GameEngineLevel* CurrentLevel = GameEngineCore::CurrentLevel();
	LumiaLevel* PlayLevel = dynamic_cast<LumiaLevel*>(CurrentLevel);
	if (nullptr == PlayLevel)
	{
		GameEngineDebug::MsgBoxError("��������� ���緹���� ã���������ϴ�!!!!!");
		return;
	}

	// Index_ == -1 �̶�� ���� �ʿ� ��ġ�� �ش� Ÿ���� ���Ϳ��� �ϰ�ó��
	// ��, ���������� �ϰ�ó������� �����Ƿ� Ŭ���϶��� ó��
	if (-1 == Index_ && false == _bServer)
	{
		// ��̾Ʒ����� ��ġ�� ���͵��� �ϰ������� ������ȯ
		switch (StateType_)
		{
			case MonsterStateType::APPEAR: // ù���������ȯ
			{
				PlayLevel->GuestMonsterFirstAppearBatchProcessing(Type_);
				break;
			}
		}

		return;
	}
	// �ƴ϶�� Ư���ε����� ������ ������ ó��
	else if(-1 != Index_)
	{
		Monsters* CurMonster = PlayLevel->GetSpecificMonster(Index_);
		if (nullptr == CurMonster)
		{
			GameEngineDebug::MsgBoxError("��������� �̻��Ѱ���!!!!");
			return;
		}

		int Index = CurMonster->GetIndex();									// �����ε���(Ȯ�ο�)

		switch (StateType_)
		{
			case MonsterStateType::ATK01: // ����01������ȯ ---
			{
				CurMonster->rcvAttack01(StatInfo_);
				break;
			}
			case MonsterStateType::ATK02: // ����02������ȯ ---
			{
				CurMonster->rcvAttack02(StatInfo_);
				break;
			}
			case MonsterStateType::HIT:	// �ǰݻ�����ȯ
			{
				CurMonster->rcvDamage(StatInfo_, TargetIndex_);
				break;
			}
			case MonsterStateType::DEATH: // ����߻�����ȯ
			{
				CurMonster->rcvDeath(StatInfo_);
				break;
			}
			case MonsterStateType::REGEN: // ����������ȯ
			{
				CurMonster->rcvRegen(StatInfo_);
				break;
			}
			case MonsterStateType::HOMINGINSTINCT: // ��ȯ������ȯ ---
			{
				CurMonster->rcvHomingInstinct(StatInfo_);
				break;
			}
		}
	}

	// ����ȭó���� ���� ������� �ٸ� Ŭ���̾�Ʈ���� ����
	if (true == _bServer)
	{
		_network->Send(this);
	}
}

MonsterStateChangePacket::MonsterStateChangePacket()
	: Index_(-1)
	, Type_(MonsterType::NONE)
	, StateType_(MonsterStateType::NONE)
	, StatInfo_{}
	, TargetIndex_(-1)
{
}

MonsterStateChangePacket::~MonsterStateChangePacket()
{
}
