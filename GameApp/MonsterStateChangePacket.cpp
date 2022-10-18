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
		GameEngineDebug::MsgBoxError("현재게임의 현재레벨을 찾을수없습니다!!!!!");
		return;
	}

	// Index_ == -1 이라면 현재 맵에 배치된 해당 타입의 몬스터에게 일괄처리
	// 단, 서버에서만 일괄처리명령을 내리므로 클라일때만 처리
	if (-1 == Index_ && false == _bServer)
	{
		// 루미아레벨에 배치된 몬스터들을 일괄적으로 상태전환
		switch (StateType_)
		{
			case MonsterStateType::APPEAR: // 첫등장상태전환
			{
				PlayLevel->GuestMonsterFirstAppearBatchProcessing(Type_);
				break;
			}
		}

		return;
	}
	// 아니라면 특정인덱스를 가지는 몬스터의 처리
	else if(-1 != Index_)
	{
		Monsters* CurMonster = PlayLevel->GetSpecificMonster(Index_);
		if (nullptr == CurMonster)
		{
			GameEngineDebug::MsgBoxError("여기들어오면 이상한거임!!!!");
			return;
		}

		int Index = CurMonster->GetIndex();									// 몬스터인덱스(확인용)

		switch (StateType_)
		{
			case MonsterStateType::ATK01: // 공격01상태전환 ---
			{
				CurMonster->rcvAttack01(StatInfo_);
				break;
			}
			case MonsterStateType::ATK02: // 공격02상태전환 ---
			{
				CurMonster->rcvAttack02(StatInfo_);
				break;
			}
			case MonsterStateType::HIT:	// 피격상태전환
			{
				CurMonster->rcvDamage(StatInfo_, TargetIndex_);
				break;
			}
			case MonsterStateType::DEATH: // 사망중상태전환
			{
				CurMonster->rcvDeath(StatInfo_);
				break;
			}
			case MonsterStateType::REGEN: // 리젠상태전환
			{
				CurMonster->rcvRegen(StatInfo_);
				break;
			}
			case MonsterStateType::HOMINGINSTINCT: // 귀환상태전환 ---
			{
				CurMonster->rcvHomingInstinct(StatInfo_);
				break;
			}
		}
	}

	// 동기화처리를 위해 서버라면 다른 클라이언트에게 전달
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
