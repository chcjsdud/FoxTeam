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
	// ���ʹ� ��� ������ ȣ��Ұ�!!!
	GameEngineDebug::MsgBox("���� ���Ͱ� Ÿ���� �𸨴ϴ�!!! Ÿ���� �˷��ּ���!!!!");
	return;
}

void Monsters::Damage(float _Amount, GameEngineActor* _Target)
{
	// ����ó��
	if (0.0f >= _Amount)
	{
		return;
	}

	// ����ó��
	if (nullptr == _Target)
	{
		return;
	}

	// ���� API�� ������ ü�°���
	StateInfo_.HP_ -= _Amount;
	if (0.0f >= StateInfo_.HP_)
	{
		// 0.0f ���ϸ� ���ó��
		StateInfo_.HP_ = 0.0f;

		// ����߻��·� ��ȯ
		ChangeAnimationAndState(MonsterStateType::DEATH);

		// ���� API�� ���� ������ȯ ��Ŷ����(����ȭó��)
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

	// ���� API�� ���͸� �ǰ��� Ÿ���� ����
	// ��, �̹� Ÿ���� �����Ǿ��ִٸ� Ÿ�ٺ������ ó��
	if (nullptr == CurTarget_)
	{
		Character* CurTarget = dynamic_cast<Character*>(_Target);
		CurTarget_ = CurTarget;
		CurTargetIndex_ = CurTarget->GetIndex();
	}

	// 0.0f���ϰ� �ƴ϶�� �ܼ� �ǰ�ó��
	// ���� API�� ���͸� �ǰݻ��·� ��ȯ
	ChangeAnimationAndState(MonsterStateType::HIT);

	// ���� API�� ���� ������ȯ ��Ŷ����(����ȭó��)
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
	// �������� ����
	StateInfo_ = _rcvStatInfo;

}

void Monsters::rcvAttack02(MonsterStateInfo _rcvStatInfo)
{
	// �������� ����
	StateInfo_ = _rcvStatInfo;

}

void Monsters::rcvDamage(MonsterStateInfo _rcvStatInfo, int _TargetIndex)
{
	// �������� ����
	StateInfo_ = _rcvStatInfo;

	if (-1 == _TargetIndex)
	{
		GameEngineDebug::MsgBoxError("�߸��� Ÿ���ε����� ���Ź޾ҽ��ϴ�!!!!");
		return;
	}

	// �̹� Ÿ���� �����ϴ��� ���ŵ� Ÿ������ ����
	LumiaLevel* CurLevel = GetLevelConvert<LumiaLevel>();
	if (nullptr == CurLevel)
	{
		GameEngineDebug::MsgBoxError("���� ������ ������ ���������ʽ��ϴ�!!!!");
		return;
	}
	CurTarget_ = CurLevel->GetSpecificCharacter(_TargetIndex);
	CurTargetIndex_ = CurTarget_->GetIndex();

	// �ǰݻ��� ��ȯ
	ChangeAnimationAndState(MonsterStateType::HIT);
}

void Monsters::rcvDeath(MonsterStateInfo _rcvStatInfo)
{
	// �������� ����
	StateInfo_ = _rcvStatInfo;

	// ����߻��·� ��ȯ
	ChangeAnimationAndState(MonsterStateType::DEATH);
}

void Monsters::rcvRegen(MonsterStateInfo _rcvStatInfo)
{
	// ��, �̹� ���������̰ų� �����Ϸ��Ͽ� �������̸� ó������
	if (CurStateType_ == MonsterStateType::REGEN || CurStateType_ == MonsterStateType::IDLE)
	{
		return;
	}

	// �������� ����
	StateInfo_ = _rcvStatInfo;

	// �������·� ��ȯ
	ChangeAnimationAndState(MonsterStateType::REGEN);
}

void Monsters::rcvHomingInstinct(MonsterStateInfo _rcvStatInfo)
{
	// ��, �̹� ��ȯ�߻����̰ų� ��ȯ�Ϸ��Ͽ� �������̸� ó������
	if (CurStateType_ == MonsterStateType::HOMINGINSTINCT || CurStateType_ == MonsterStateType::IDLE)
	{
		return;
	}

	// �������� ����
	StateInfo_ = _rcvStatInfo;

	// �������·� ��ȯ
	ChangeAnimationAndState(MonsterStateType::HOMINGINSTINCT);
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
			CrowdControlState_ << "HIT";
			MainState_ << "CROWDCONTROL";
			MainRenderer_->ChangeFBXAnimation("HIT");
			break;
		}
		case MonsterStateType::DEATH:
		{
			CrowdControlState_ << "DEATH";
			MainState_ << "CROWDCONTROL";
			MainRenderer_->ChangeFBXAnimation("DEATH");
			break;
		}
		case MonsterStateType::DEAD:
		{
			CrowdControlState_ << "DEAD";
			MainState_ << "CROWDCONTROL";
			MainRenderer_->ChangeFBXAnimation("DEAD");
			break;
		}
		case MonsterStateType::ATK01:
		{
			AttackState_ << "ATK01";
			MainState_ << "ATTACK";
			MainRenderer_->ChangeFBXAnimation("ATK01");
			break;
		}
		case MonsterStateType::ATK02:
		{
			AttackState_ << "ATK02";
			MainState_ << "ATTACK";
			MainRenderer_->ChangeFBXAnimation("ATK02");
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

	// 1. �ͼҺ��ɼ�ġ ����
	HomingInstinctValueUpdate(_DeltaTime);

	// 2. ...



}

void Monsters::CheckAllCollision(float _DeltaTime)
{
	// ��ü �浹ü �浹üũ
	CheckBodyCollision(_DeltaTime);

	// ���� �浹ü �浹üũ
	CheckAttackCollision(_DeltaTime);
}

void Monsters::CheckBodyCollision(float _DeltaTime)
{
	// ��ü �浹ü �浹üũ
	if (nullptr != BodyCollider_ && true == BodyCollider_->IsUpdate())
	{
//#ifdef _DEBUG
		GetLevel()->PushDebugRender(BodyCollider_->GetTransform(), CollisionType::OBBBox3D, float4::RED);
//#endif // _DEBUG

		//// 221017 SJH DEL : Ÿ������ �����Լ�ó���� ����
		//// ����߻��°� �ƴϸ鼭 ������°� �ƴҶ� ������ Ÿ���� ���°��
		//if (nullptr == CurTarget_ && (MonsterStateType::DEAD != CurStateType_ && MonsterStateType::DEATH != CurStateType_))
		//{
		//	// Ÿ�������� ���� ���� ���� �����Ѵ���� Ÿ������ ����
		//	GameEngineCollision* HitCollider = BodyCollider_->GetCollision(static_cast<int>(eCollisionGroup::PlayerAttack));
		//	if (nullptr != HitCollider)
		//	{
		//		GameEngineActor* TargetActor = HitCollider->GetActor();
		//		if (nullptr != TargetActor)
		//		{
		//			// Ÿ������ �� Ÿ���� �ε��� ����
		//			CurTarget_ = dynamic_cast<Character*>(TargetActor);
		//			CurTargetIndex_ = CurTarget_->GetIndex();
		//		}
		//	}
		//}

		// ���콺 �׷���� �浹
		// ��, ���� ������������϶��� �浹üũ -> Ŭ���� ItemBox View
		if (MonsterStateType::DEAD == CurStateType_)
		{
			if (true == BodyCollider_->Collision(static_cast<int>(eCollisionGroup::MousePointer)) && true == GameEngineInput::GetInst().Down("LButton"))
			{
				// ItemBox View





			}
		}
	}
}

void Monsters::CheckAttackCollision(float _DeltaTime)
{
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

void Monsters::HomingInstinctValueUpdate(float _DeltaTime)
{
	// Ÿ���� �����Ǿ��ִٸ� �ͼҺ��ɼ�ġ ����
	if (nullptr != CurTarget_)
	{
		StateInfo_.HomingInstinctValue_ -= _DeltaTime;
		if (0.0f >= StateInfo_.HomingInstinctValue_)
		{
			// ��ȯ���·� ��ȯ
			ChangeAnimationAndState(MonsterStateType::HOMINGINSTINCT);

			// �ͼҺ��ɼ�ġ �ʱ�ȭ
			StateInfo_.HomingInstinctValue_ = StateInfo_.HomingInstinctValueMax_;

			// Ÿ����������
			CurTarget_ = nullptr;
			CurTargetIndex_ = -1;

			// ��Ŷ����
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
	, CurStateType_(MonsterStateType::NONE)
{
	// ����Ÿ�� = ����
	UnitType_ = UnitType::MONSTER;
}

Monsters::~Monsters()
{
}
