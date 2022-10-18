#include "Precompile.h"
#include "Monsters.h"

#include <GameEngineBase/GameEngineRandom.h>
#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "MonsterInfoManager.h"
#include "GameServer.h"
#include "GameClient.h"
#include "MonsterStateChangePacket.h"

#include "LumiaLevel.h"
#include "LumiaMap.h"
#include "NavMesh.h"

#include "Character.h"

int Monsters::GetIndex()
{
	return Index_;
}

void Monsters::Damage(float _Amount)
{
	// 몬스터는 얘로 데미지 호출불가!!!
	GameEngineDebug::MsgBox("현재 몬스터가 타겟을 모릅니다!!! 타겟을 알려주세요!!!!");
	return;
}

void Monsters::Damage(float _Amount, GameEngineActor* _Target)
{
	// 예외처리
	if (0.0f >= _Amount)
	{
		GameEngineDebug::OutPutDebugString("해당 몬스터를 공격하는 데미지가 잘못된 값으로 설정되었습니다!!!!\n");
		GameEngineDebug::OutPutDebugString("수신 데미지: " + std::to_string(_Amount) + "\n");
		return;
	}

	// 예외처리
	if (nullptr == _Target)
	{
		GameEngineDebug::OutPutDebugString("해당 몬스터의 타겟을 지정할수없습니다!!!!\n");
		return;
	}

	// 예외처리
	if (true == GetHitOffFlag_)
	{
		GameEngineDebug::OutPutDebugString("해당 몬스터는 이미 사망상태로 공격대상이 아닙니다!!!!\n");
		return;
	}

	// 현재 API의 몬스터의 체력감소
	StateInfo_.HP_ -= _Amount;
	if (0.0f >= StateInfo_.HP_)
	{
		// 0.0f 이하면 사망처리
		StateInfo_.HP_ = 0.0f;

		// 사망중상태로 전환
		ChangeAnimationAndState(MonsterStateType::DEATH);

		// 현재 API의 몬스터 상태전환 패킷전송(동기화처리)
		MonsterStateChangePacket ChangePacket;
		ChangePacket.SetIndex(Index_);
		ChangePacket.SetMonsterType(Type_);
		ChangePacket.SetMonsterStateType(MonsterStateType::DEATH);
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

		return;
	}

	// 현재 API의 몬스터를 피격한 타겟을 지정
	// 단, 이미 타겟이 지정되어있다면 타겟변경없이 처리
	if (nullptr == CurTarget_)
	{
		Character* CurTarget = dynamic_cast<Character*>(_Target);
		CurTarget_ = CurTarget;
		CurTargetIndex_ = CurTarget->GetIndex();
	}

	// 0.0f이하가 아니라면 단순 피격처리
	// 현재 API의 몬스터를 피격상태로 전환
	ChangeAnimationAndState(MonsterStateType::HIT);

	// 현재 API의 몬스터 상태전환 패킷전송(동기화처리)
	MonsterStateChangePacket ChangePacket;
	ChangePacket.SetIndex(Index_);
	ChangePacket.SetMonsterType(Type_);
	ChangePacket.SetMonsterStateType(MonsterStateType::HIT);
	ChangePacket.SetMonsterStatInfo(StateInfo_);
	ChangePacket.SetTargetIndex(CurTargetIndex_);
	if (true == GameServer::GetInstance()->IsOpened())
	{
		GameServer::GetInstance()->Send(&ChangePacket);
	}
	else if (true == GameClient::GetInstance()->IsConnected())
	{
		GameClient::GetInstance()->Send(&ChangePacket);
	}
}

void Monsters::rcvAttack01(MonsterStateInfo _rcvStatInfo)
{
	// 상태정보 갱신
	StateInfo_ = _rcvStatInfo;

}

void Monsters::rcvAttack02(MonsterStateInfo _rcvStatInfo)
{
	// 상태정보 갱신
	StateInfo_ = _rcvStatInfo;

}

void Monsters::rcvDamage(MonsterStateInfo _rcvStatInfo, int _TargetIndex)
{
	// 상태정보 갱신
	StateInfo_ = _rcvStatInfo;

	if (-1 == _TargetIndex)
	{
		GameEngineDebug::MsgBoxError("잘못된 타겟인덱스를 수신받았습니다!!!!");
		return;
	}

	// 이미 타겟이 존재하더라도 수신된 타겟으로 변경
	LumiaLevel* CurLevel = GetLevelConvert<LumiaLevel>();
	if (nullptr == CurLevel)
	{
		GameEngineDebug::MsgBoxError("현재 지정된 레벨이 존재하지않습니다!!!!");
		return;
	}
	CurTarget_ = CurLevel->GetSpecificCharacter(_TargetIndex);
	CurTargetIndex_ = CurTarget_->GetIndex();

	// 피격상태 전환
	ChangeAnimationAndState(MonsterStateType::HIT);
}

void Monsters::rcvDeath(MonsterStateInfo _rcvStatInfo)
{
	// 이미상 사망중 or 사망상태라면 무시
	if (MonsterStateType::DEAD == CurStateType_ || MonsterStateType::DEATH == CurStateType_)
	{
		return;
	}

	// 상태정보 갱신
	StateInfo_ = _rcvStatInfo;

	// 사망중상태로 전환
	ChangeAnimationAndState(MonsterStateType::DEATH);
}

void Monsters::rcvRegen(MonsterStateInfo _rcvStatInfo)
{
	// 단, 이미 리젠상태이거나 재등장완료하여 대기상태이면 처리무시
	if (CurStateType_ == MonsterStateType::REGEN || CurStateType_ == MonsterStateType::IDLE)
	{
		return;
	}

	// 상태정보 갱신
	StateInfo_ = _rcvStatInfo;

	// 리젠상태로 전환
	ChangeAnimationAndState(MonsterStateType::REGEN);
}

void Monsters::rcvHomingInstinct(MonsterStateInfo _rcvStatInfo)
{
	// 단, 이미 귀환중상태이거나 귀환완료하여 대기상태이면 처리무시
	if (CurStateType_ == MonsterStateType::HOMINGINSTINCT || CurStateType_ == MonsterStateType::IDLE)
	{
		return;
	}

	// 상태정보 갱신
	StateInfo_ = _rcvStatInfo;

	// 리젠상태로 전환
	ChangeAnimationAndState(MonsterStateType::HOMINGINSTINCT);
}

void Monsters::InitalizeSpawnPosition(const float4& _SpawnPosition)
{
	MonsterInfoManager* mm = MonsterInfoManager::GetInstance();

	// 현재 몬스터가 위치한 맵 지정
	CurrentMap_ = GetLevelConvert<LumiaLevel>()->GetMap();
	if (nullptr == CurrentMap_)
	{
		GameEngineDebug::MsgBoxError("현재레벨에 생성된 맵이 존재하지않습니다!!!!!");
		return;
	}

	// 액터의 위치 셋팅
	float4 SpawnPos = _SpawnPosition;
	SpawnPos *= CurrentMap_->GetNavMesh()->GetNaviRenderer()->GetTransform()->GetTransformData().WorldWorld_;
	if (true == mm->GetAllMonsterListRef()[Index_].IsGroup_)
	{
		if (0 != Index_)
		{
			SpawnPos = mm->GetAllMonsterListRef()[Index_ - 1].SpawnPosition_;
			SpawnPos *= CurrentMap_->GetNavMesh()->GetNaviRenderer()->GetTransform()->GetTransformData().WorldWorld_;
			SpawnPos += float4(100.f, 0.f, 10.f);
		}
	}
	GetTransform()->SetLocalPosition(SpawnPos);

	// 현재이동목적지 = 스폰위치
	Destination_ = SpawnPos;

	// 해당 몬스터 둥지위치 == 최초 스폰위치
	StateInfo_.NestPosition_ = SpawnPos;
}

void Monsters::ChangeAnimationAndState(MonsterStateType _StateType)
{
	// 상태 상테타입으로 애니메이션 전환 및 상태전환
	switch (_StateType)
	{
		case MonsterStateType::APPEAR:
		{
			NormalState_ << "APPEAR";
			MainState_ << "NORMAL";
			MainRenderer_->ChangeFBXAnimation("APPEAR", true);
			break;
		}
		case MonsterStateType::REGEN:
		{
			NormalState_ << "REGEN";
			MainState_ << "NORMAL";
			MainRenderer_->ChangeFBXAnimation("REGEN", true);
			break;
		}
		case MonsterStateType::IDLE:
		{
			NormalState_ << "IDLE";
			MainState_ << "NORMAL";
			MainRenderer_->ChangeFBXAnimation("IDLE");
			break;
		}
		case MonsterStateType::HOMINGINSTINCT:
		{
			NormalState_ << "HOMINGINSTINCT";
			MainState_ << "NORMAL";
			MainRenderer_->ChangeFBXAnimation("HOMINGINSTINCT");
			break;
		}
		case MonsterStateType::CHASE:
		{
			NormalState_ << "CHASE";
			MainState_ << "NORMAL";
			MainRenderer_->ChangeFBXAnimation("CHASE");
			break;
		}
		case MonsterStateType::HIT:
		{
			CrowdControlState_ << "HIT";
			MainState_ << "CROWDCONTROL";
			MainRenderer_->ChangeFBXAnimation("HIT", true);
			break;
		}
		case MonsterStateType::DEATH:
		{
			CrowdControlState_ << "DEATH";
			MainState_ << "CROWDCONTROL";
			MainRenderer_->ChangeFBXAnimation("DEATH", true);
			break;
		}
		case MonsterStateType::DEAD:
		{
			CrowdControlState_ << "DEAD";
			MainState_ << "CROWDCONTROL";
			//MainRenderer_->ChangeFBXAnimation("DEAD");
			break;
		}
		case MonsterStateType::ATK01:
		{
			AttackState_ << "ATK01";
			MainState_ << "ATTACK";
			MainRenderer_->ChangeFBXAnimation("ATK01", true);
			break;
		}
		case MonsterStateType::ATK02:
		{
			AttackState_ << "ATK02";
			MainState_ << "ATTACK";
			MainRenderer_->ChangeFBXAnimation("ATK02", true);
			break;
		}
		case MonsterStateType::SKILLATTACK:
		{
			AttackState_ << "SKILLATTACK";
			MainState_ << "ATTACK";
			MainRenderer_->ChangeFBXAnimation("SKILLATTACK");
			break;
		}
		case MonsterStateType::MAX:
		case MonsterStateType::NONE:
			GameEngineDebug::MsgBox("잘못된 상태타입을 수신하여 애니메이션 및 상태 변경이 불가합니다!!!");
			return;
	}
}

void Monsters::InitalizeFSMState()
{
#pragma region MainState
	MainState_.CreateState("NORMAL", std::bind(&Monsters::UpdateNormalState, this, std::placeholders::_1), std::bind(&Monsters::StartNormalState, this), std::bind(&Monsters::EndNormalState, this));
	MainState_.CreateState("CROWDCONTROL", std::bind(&Monsters::UpdateCrowdControlState, this, std::placeholders::_1), std::bind(&Monsters::StartCrowdControlState, this), std::bind(&Monsters::EndCrowdControlState, this));
	MainState_.CreateState("ATTACK", std::bind(&Monsters::UpdateAttackState, this, std::placeholders::_1), std::bind(&Monsters::StartAttackState, this), std::bind(&Monsters::EndAttackState, this));
	MainState_.ChangeState("NORMAL");
#pragma endregion

#pragma region NormalState
	// 첫등장, 리젠, 대기, 추적, 귀화
	NormalState_.CreateState("APPEAR", std::bind(&Monsters::UpdateAppearState, this, std::placeholders::_1), std::bind(&Monsters::StartAppearState, this), std::bind(&Monsters::EndAppearState, this));
	NormalState_.CreateState("REGEN", std::bind(&Monsters::UpdateRegenState, this, std::placeholders::_1), std::bind(&Monsters::StartRegenState, this), std::bind(&Monsters::EndRegenState, this));
	NormalState_.CreateState("IDLE", std::bind(&Monsters::UpdateIdleState, this, std::placeholders::_1), std::bind(&Monsters::StartIdleState, this), std::bind(&Monsters::EndIdleState, this));
	NormalState_.CreateState("CHASE", std::bind(&Monsters::UpdateChaseState, this, std::placeholders::_1), std::bind(&Monsters::StartChaseState, this), std::bind(&Monsters::EndChaseState, this));
	NormalState_.CreateState("HOMINGINSTINCT", std::bind(&Monsters::UpdateHomingInstinctState, this, std::placeholders::_1), std::bind(&Monsters::StartHomingInstinctState, this), std::bind(&Monsters::EndHomingInstinctState, this));

	NormalState_.ChangeState("APPEAR");
#pragma endregion

#pragma region CrowdControlState
	// 피격, 사망중, 완전사망
	CrowdControlState_.CreateState("HIT", std::bind(&Monsters::UpdateHitState, this, std::placeholders::_1), std::bind(&Monsters::StartHitState, this), std::bind(&Monsters::EndHitState, this));
	CrowdControlState_.CreateState("DEATH", std::bind(&Monsters::UpdateDeathState, this, std::placeholders::_1), std::bind(&Monsters::StartDeathState, this), std::bind(&Monsters::EndDeathState, this));
	CrowdControlState_.CreateState("DEAD", std::bind(&Monsters::UpdateDeadState, this, std::placeholders::_1), std::bind(&Monsters::StartDeadState, this), std::bind(&Monsters::EndDeadState, this));

	CrowdControlState_.ChangeState("HIT");
#pragma endregion

#pragma region AttackState
	// 공격1, 공격2, 스킬(예외존재)
	AttackState_.CreateState("ATK01", std::bind(&Monsters::UpdateAttack01State, this, std::placeholders::_1), std::bind(&Monsters::StartAttack01State, this), std::bind(&Monsters::EndAttack01State, this));
	AttackState_.CreateState("ATK02", std::bind(&Monsters::UpdateAttack02State, this, std::placeholders::_1), std::bind(&Monsters::StartAttack02State, this), std::bind(&Monsters::EndAttack02State, this));
	AttackState_.CreateState("SKILLATTACK", std::bind(&Monsters::UpdateSkillAttackState, this, std::placeholders::_1), std::bind(&Monsters::StartSkillAttackState, this), std::bind(&Monsters::EndSkillAttackState, this));

	AttackState_.ChangeState("ATK01");
#pragma endregion
}

void Monsters::CheckMonsterStateInfo(float _DeltaTime)
{
	// 상태정보를 이용하여 강제전환해야하는 경우를 아래와 같이 처리

	// 1. 귀소본능수치 갱신
	HomingInstinctValueUpdate(_DeltaTime);

	// 2. ...



}

void Monsters::CheckAllCollision(float _DeltaTime)
{
	// 몸체 충돌체 충돌체크
	CheckBodyCollision(_DeltaTime);

	// 공격 충돌체 충돌체크
	CheckAttackCollision(_DeltaTime);
}

void Monsters::CheckBodyCollision(float _DeltaTime)
{
	// 몸체 충돌체 충돌체크
	if (nullptr != BodyCollider_ && true == BodyCollider_->IsUpdate())
	{
		if (false == GetHitOffFlag_)
		{
			GetLevel()->PushDebugRender(BodyCollider_->GetTransform(), CollisionType::OBBBox3D, float4::BLUE);
		}

		// 221018 SJH : 임시주석처리
		//// 마우스 그룹과의 충돌
		//// 단, 몬스터 완전사망상태이며 피격판정무시상태라면 충돌체크하며, 해당 충돌시 드랍된아이템을 표시
		//if (MonsterStateType::DEAD == CurStateType_ && true == GetHitOffFlag_)
		//{
		//	if (true == BodyCollider_->Collision(static_cast<int>(eCollisionGroup::MousePointer)) && true == GameEngineInput::GetInst().Down("LButton"))
		//	{
		//		// ItemBox View
		//	}
		//}
	}
}

void Monsters::CheckAttackCollision(float _DeltaTime)
{
	// 공격 충돌체 충돌체크
	if (nullptr != AtkCollider_ && true == AtkCollider_->IsUpdate())
	{
#ifdef _DEBUG
		GetLevel()->PushDebugRender(AtkCollider_->GetTransform(), CollisionType::AABBBox3D, float4::RED);
#endif // _DEBUG

		// 플레이어 그룹과의 충돌
		if (true == AtkCollider_->Collision(static_cast<int>(eCollisionGroup::Player)))
		{
			// 플레이어 공격




		}
	}
}

void Monsters::HomingInstinctValueUpdate(float _DeltaTime)
{
	// 타겟이 지정되어있다면 귀소본능수치 감소
	if (nullptr != CurTarget_)
	{
		StateInfo_.HomingInstinctValue_ -= _DeltaTime;
		if (0.0f >= StateInfo_.HomingInstinctValue_)
		{
			// 귀환상태로 전환
			ChangeAnimationAndState(MonsterStateType::HOMINGINSTINCT);

			// 귀소본능수치 초기화
			StateInfo_.HomingInstinctValue_ = StateInfo_.HomingInstinctValueMax_;

			// 타겟지정해제
			CurTarget_ = nullptr;
			CurTargetIndex_ = -1;

			// 패킷전송
			MonsterStateChangePacket Packet;
			Packet.SetIndex(Index_);
			Packet.SetMonsterType(Type_);
			Packet.SetMonsterStateType(MonsterStateType::HOMINGINSTINCT);
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
}

void Monsters::DebugWindowUpdate()
{
	// LevelControl
}

void Monsters::Start()
{
	// 초기화
	InitalizeFSMState();					// Initalize FSM State
	InitalizeStateInfo();					// Initalize StateInfo
	InitalizeResourceLoad();				// Initalize Resource Load
	InitalizeRenderAndAnimation();			// Initalize Renderer And Animation Setting
	InitalizeCollider();					// Initalize Collider

	// ...
}

void Monsters::Update(float _deltaTime)
{
	// 정보에의한 상태 전환체크
	CheckMonsterStateInfo(_deltaTime);

	// 충돌체 충돌체크
	CheckAllCollision(_deltaTime);

	// Main State Update
	MainState_.Update(_deltaTime);

	// ...

}

Monsters::Monsters()
	: MainRenderer_(nullptr)
	, EffectRenderer_(nullptr)
	, BodyCollider_(nullptr)
	, AtkCollider_(nullptr)
	, Index_(-1)
	, Type_(MonsterType::NONE)
	, AreaType_(Location::NONE)
	, CurrentNavFace_(nullptr)
	, CurrentNavMesh_(nullptr)
	, CurrentMap_(nullptr)
	, StateInfo_{}
	, Destination_(float4::ZERO)
	, MainState_()
	, NormalState_()
	, CrowdControlState_()
	, AttackState_()
	, CurTarget_(nullptr)
	, CurTargetIndex_(-1)
	, CurStateBasicType_(MonsterStateBasicType::NONE)
	, PrevStateType_(MonsterStateType::NONE)
	, CurStateType_(MonsterStateType::NONE)
	, GetHitOffFlag_(false)
{
	// 유닛타입 = 몬스터
	UnitType_ = UnitType::MONSTER;
}

Monsters::~Monsters()
{
}
