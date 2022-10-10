#include "Precompile.h"
#include "Monsters.h"

#include <GameEngineBase/GameEngineRandom.h>
#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "LumiaLevel.h"

void Monsters::InitalizeSpawnPosition(const float4& _SpawnPosition)
{
	// 액터의 위치 셋팅
	GetTransform()->SetLocalPosition(_SpawnPosition);

	// 현재이동목적지 = 스폰위치
	Destination_ = _SpawnPosition;

	// 해당 몬스터 둥지위치 == 최초 스폰위치
	StateInfo_.NestPosition_ = _SpawnPosition;

	// 현재 몬스터가 위치한 맵 지정
	CurrentMap_ = GetLevelConvert<LumiaLevel>()->GetMap();
	if (nullptr == CurrentMap_)
	{
		GameEngineDebug::MsgBoxError("현재레벨에 생성된 맵이 존재하지않습니다!!!!!");
		return;
	}
}

void Monsters::ChangeAnimationAndState(MonsterStateType _StateType)
{
	// 상태 상테타입으로 애니메이션 전환 및 상태전환
	switch (_StateType)
	{
		case MonsterStateType::APPEAR:
		{
			MainState_ << "NORMAL";
			NormalState_ << "APPEAR";
			MainRenderer_->ChangeFBXAnimation("APPEAR");
			break;
		}
		case MonsterStateType::REGEN:
		{
			MainState_ << "NORMAL";
			NormalState_ << "REGEN";
			MainRenderer_->ChangeFBXAnimation("REGEN");
			break;
		}
		case MonsterStateType::IDLE:
		{
			MainState_ << "NORMAL";
			NormalState_ << "IDLE";
			MainRenderer_->ChangeFBXAnimation("IDLE");
			break;
		}
		case MonsterStateType::RUN:
		{
			MainState_ << "NORMAL";
			NormalState_ << "RUN";
			MainRenderer_->ChangeFBXAnimation("RUN");
			break;
		}
		case MonsterStateType::HOMINGINSTINCT:
		{
			MainState_ << "NORMAL";
			NormalState_ << "HOMINGINSTINCT";
			MainRenderer_->ChangeFBXAnimation("HOMINGINSTINCT");
			break;
		}
		case MonsterStateType::CHASE:
		{
			MainState_ << "NORMAL";
			NormalState_ << "CHASE";
			MainRenderer_->ChangeFBXAnimation("CHASE");
			break;
		}
		case MonsterStateType::HIT:
		{
			MainState_ << "CROWDCONTROL";
			NormalState_ << "HIT";
			MainRenderer_->ChangeFBXAnimation("HIT");
			break;
		}
		case MonsterStateType::DEATH:
		{
			MainState_ << "CROWDCONTROL";
			NormalState_ << "DEATH";
			MainRenderer_->ChangeFBXAnimation("DEATH");
			break;
		}
		case MonsterStateType::DEAD:
		{
			MainState_ << "CROWDCONTROL";
			NormalState_ << "DEAD";
			MainRenderer_->ChangeFBXAnimation("DEAD");
			break;
		}
		case MonsterStateType::ATK01:
		{
			MainState_ << "ATTACK";
			NormalState_ << "ATK01";
			MainRenderer_->ChangeFBXAnimation("ATK01");
			break;
		}
		case MonsterStateType::ATK02:
		{
			MainState_ << "ATTACK";
			NormalState_ << "ATK02";
			MainRenderer_->ChangeFBXAnimation("ATK02");
			break;
		}
		case MonsterStateType::SKILLATTACK:
		{
			MainState_ << "ATTACK";
			NormalState_ << "SKILLATTACK";
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
	NormalState_.CreateState("APPEAR", std::bind(&Monsters::UpdateAppearState, this, std::placeholders::_1), std::bind(&Monsters::StartAppearState, this), std::bind(&Monsters::EndAppearState, this));
	NormalState_.CreateState("REGEN", std::bind(&Monsters::UpdateRegenState, this, std::placeholders::_1), std::bind(&Monsters::StartRegenState, this), std::bind(&Monsters::EndRegenState, this));
	NormalState_.CreateState("IDLE", std::bind(&Monsters::UpdateIdleState, this, std::placeholders::_1), std::bind(&Monsters::StartIdleState, this), std::bind(&Monsters::EndIdleState, this));
	NormalState_.CreateState("RUN", std::bind(&Monsters::UpdateRunState, this, std::placeholders::_1), std::bind(&Monsters::StartRunState, this), std::bind(&Monsters::EndRunState, this));
	NormalState_.CreateState("HOMINGINSTINCT", std::bind(&Monsters::UpdateHomingInstinctState, this, std::placeholders::_1), std::bind(&Monsters::StartHomingInstinctState, this), std::bind(&Monsters::EndHomingInstinctState, this));

	NormalState_.ChangeState("APPEAR");
#pragma endregion

#pragma region CrowdControlState
	CrowdControlState_.CreateState("HIT", std::bind(&Monsters::UpdateHitState, this, std::placeholders::_1), std::bind(&Monsters::StartHitState, this), std::bind(&Monsters::EndHitState, this));
	CrowdControlState_.CreateState("DEATH", std::bind(&Monsters::UpdateDeathState, this, std::placeholders::_1), std::bind(&Monsters::StartDeathState, this), std::bind(&Monsters::EndDeathState, this));
	CrowdControlState_.CreateState("DEAD", std::bind(&Monsters::UpdateDeadState, this, std::placeholders::_1), std::bind(&Monsters::StartDeadState, this), std::bind(&Monsters::EndDeadState, this));

	CrowdControlState_.ChangeState("HIT");
#pragma endregion

#pragma region AttackState
	AttackState_.CreateState("ATK01", std::bind(&Monsters::UpdateAttack01State, this, std::placeholders::_1), std::bind(&Monsters::StartAttack01State, this), std::bind(&Monsters::EndAttack01State, this));
	AttackState_.CreateState("ATK02", std::bind(&Monsters::UpdateAttack02State, this, std::placeholders::_1), std::bind(&Monsters::StartAttack02State, this), std::bind(&Monsters::EndAttack02State, this));
	AttackState_.CreateState("SKILLATTACK", std::bind(&Monsters::UpdateSkillAttackState, this, std::placeholders::_1), std::bind(&Monsters::StartSkillAttackState, this), std::bind(&Monsters::EndSkillAttackState, this));

	AttackState_.ChangeState("ATK01");
#pragma endregion
}

void Monsters::CheckMonsterStateInfo(float _DeltaTime)
{
	// 상태정보를 이용하여 강제전환해야하는 경우를 아래와 같이 처리




}

void Monsters::CheckAllCollision(float _DeltaTime)
{
	// 몸체 충돌체 충돌체크
	if (nullptr != BodyCollider_ && true == BodyCollider_->IsUpdate())
	{
#ifdef _DEBUG
		GetLevel()->PushDebugRender(BodyCollider_->GetTransform(), CollisionType::AABBBox3D, float4::RED);
#endif // _DEBUG

		// 플레이어 그룹과의 충돌
		GameEngineCollision* HitCollider = BodyCollider_->GetCollision(static_cast<int>(eCollisionGroup::Player));
		if (nullptr != HitCollider)
		{
			// 타겟지정
			TargetCollider_ = HitCollider;

			// 피격상태 전환

		}

		// 마우스 그룹과의 충돌
		if (true == BodyCollider_->Collision(static_cast<int>(eCollisionGroup::MousePointer)))
		{
			// 현재 사망중 OR 사망상태라면 View ItemBox

		}
	}

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

void Monsters::Start()
{
	// 초기화
	InitalizeFSMState();					// Initalize FSM State
	InitalizeStateInfo();					// Initalize StateInfo
	InitalizeResourceLoad();				// Initalize Resource Load
	InitalizeRenderAndAnimation();			// Initalize Renderer And Animation Setting
	InitalizeCollider();					// Initalize Collider

	// 
}

void Monsters::Update(float _deltaTime)
{
	// 정보에의한 상태 전환체크
	CheckMonsterStateInfo(_deltaTime);

	// 충돌체 충돌체크
	CheckAllCollision(_deltaTime);

	// Main State Update
	MainState_.Update(_deltaTime);

	// 

}

Monsters::Monsters()
	: MainRenderer_(nullptr)
	, EffectRenderer_(nullptr)
	, BodyCollider_(nullptr)
	, AtkCollider_(nullptr)
	, TargetCollider_(nullptr)
	, Index_(-1)
	, Type_(MonsterType::NONE)
	, AreaType_(Location::NONE)
	, CurrentNavFace_(nullptr)
	, CurrentNavMesh_(nullptr)
	, CurrentMap_(nullptr)
	, DetectTarget_(false)
	, StateInfo_{}
	, Destination_(float4::ZERO)
	, MainState_()
	, NormalState_()
	, CrowdControlState_()
	, AttackState_()
	, CurStateBasicType_(MonsterStateBasicType::NONE)
	, CurStateType_(MonsterStateType::NONE)
{
}

Monsters::~Monsters()
{
}
