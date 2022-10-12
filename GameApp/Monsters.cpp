#include "Precompile.h"
#include "Monsters.h"

#include <GameEngineBase/GameEngineRandom.h>
#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "MonsterInfoManager.h"

#include "LumiaLevel.h"
#include "LumiaMap.h"
#include "NavMesh.h"

int Monsters::GetIndex()
{
	return Index_;
}

void Monsters::InitalizeSpawnPosition(const float4& _SpawnPosition)
{
	MonsterInfoManager* mm = MonsterInfoManager::GetInstance();

	// ���� ���Ͱ� ��ġ�� �� ����
	CurrentMap_ = GetLevelConvert<LumiaLevel>()->GetMap();
	if (nullptr == CurrentMap_)
	{
		GameEngineDebug::MsgBoxError("���緹���� ������ ���� ���������ʽ��ϴ�!!!!!");
		return;
	}

	// ������ ��ġ ����
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

	// �����̵������� = ������ġ
	Destination_ = SpawnPos;

	// �ش� ���� ������ġ == ���� ������ġ
	StateInfo_.NestPosition_ = SpawnPos;
}

void Monsters::ChangeAnimationAndState(MonsterStateType _StateType)
{
	// ���� ����Ÿ������ �ִϸ��̼� ��ȯ �� ������ȯ
	switch (_StateType)
	{
		case MonsterStateType::APPEAR:
		{
			NormalState_ << "APPEAR";
			MainState_ << "NORMAL";
			MainRenderer_->ChangeFBXAnimation("APPEAR");
			break;
		}
		case MonsterStateType::REGEN:
		{
			NormalState_ << "REGEN";
			MainState_ << "NORMAL";
			MainRenderer_->ChangeFBXAnimation("REGEN");
			break;
		}
		case MonsterStateType::IDLE:
		{
			NormalState_ << "IDLE";
			MainState_ << "NORMAL";
			MainRenderer_->ChangeFBXAnimation("IDLE");
			break;
		}
		case MonsterStateType::RUN:
		{
			NormalState_ << "RUN";
			MainState_ << "NORMAL";
			MainRenderer_->ChangeFBXAnimation("RUN");
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
			NormalState_ << "HIT";
			MainState_ << "CROWDCONTROL";
			MainRenderer_->ChangeFBXAnimation("HIT");
			break;
		}
		case MonsterStateType::DEATH:
		{
			NormalState_ << "DEATH";
			MainState_ << "CROWDCONTROL";
			MainRenderer_->ChangeFBXAnimation("DEATH");
			break;
		}
		case MonsterStateType::DEAD:
		{
			NormalState_ << "DEAD";
			MainState_ << "CROWDCONTROL";
			MainRenderer_->ChangeFBXAnimation("DEAD");
			break;
		}
		case MonsterStateType::ATK01:
		{
			NormalState_ << "ATK01";
			MainState_ << "ATTACK";
			MainRenderer_->ChangeFBXAnimation("ATK01");
			break;
		}
		case MonsterStateType::ATK02:
		{
			NormalState_ << "ATK02";
			MainState_ << "ATTACK";
			MainRenderer_->ChangeFBXAnimation("ATK02");
			break;
		}
		case MonsterStateType::SKILLATTACK:
		{
			NormalState_ << "SKILLATTACK";
			MainState_ << "ATTACK";
			MainRenderer_->ChangeFBXAnimation("SKILLATTACK");
			break;
		}
		case MonsterStateType::MAX:
		case MonsterStateType::NONE:
			GameEngineDebug::MsgBox("�߸��� ����Ÿ���� �����Ͽ� �ִϸ��̼� �� ���� ������ �Ұ��մϴ�!!!");
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
	// ���������� �̿��Ͽ� ������ȯ�ؾ��ϴ� ��츦 �Ʒ��� ���� ó��




}

void Monsters::CheckAllCollision(float _DeltaTime)
{
	// ��ü �浹ü �浹üũ
	if (nullptr != BodyCollider_ && true == BodyCollider_->IsUpdate())
	{
#ifdef _DEBUG
		GetLevel()->PushDebugRender(BodyCollider_->GetTransform(), CollisionType::AABBBox3D, float4::RED);
#endif // _DEBUG

		// �÷��̾� �׷���� �浹
		GameEngineCollision* HitCollider = BodyCollider_->GetCollision(static_cast<int>(eCollisionGroup::Player));
		if (nullptr != HitCollider)
		{
			// Ÿ������
			TargetCollider_ = HitCollider;

			// �ǰݻ��� ��ȯ

		}

		// ���콺 �׷���� �浹
		if (true == BodyCollider_->Collision(static_cast<int>(eCollisionGroup::MousePointer)))
		{
			// ���� ����� OR ������¶�� View ItemBox

		}
	}

	// ���� �浹ü �浹üũ
	if (nullptr != AtkCollider_ && true == AtkCollider_->IsUpdate())
	{
#ifdef _DEBUG
		GetLevel()->PushDebugRender(AtkCollider_->GetTransform(), CollisionType::AABBBox3D, float4::RED);
#endif // _DEBUG

		// �÷��̾� �׷���� �浹
		if (true == AtkCollider_->Collision(static_cast<int>(eCollisionGroup::Player)))
		{
			// �÷��̾� ����

		}
	}
}

void Monsters::Start()
{
	// �ʱ�ȭ
	InitalizeFSMState();					// Initalize FSM State
	InitalizeStateInfo();					// Initalize StateInfo
	InitalizeResourceLoad();				// Initalize Resource Load
	InitalizeRenderAndAnimation();			// Initalize Renderer And Animation Setting
	InitalizeCollider();					// Initalize Collider

	// 
}

void Monsters::Update(float _deltaTime)
{
	// ���������� ���� ��ȯüũ
	CheckMonsterStateInfo(_deltaTime);

	// �浹ü �浹üũ
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
