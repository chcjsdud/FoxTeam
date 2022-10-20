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

void Monsters::Damage(float _Amount, IUnit* _Target)
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
	if (true == IsDeath_)
	{
		GameEngineDebug::OutPutDebugString("해당 몬스터는 이미 사망상태로 공격대상이 아닙니다!!!!\n");
		return;
	}

	// 예외처리
	if (UnitType::MONSTER == _Target->UnitType_)
	{
		GameEngineDebug::OutPutDebugString("같은 몬스터가 나를 공격했으므로 무시합니다!!!!!!\n");
		return;
	}

	// 현재 API의 몬스터의 체력감소
	StateInfo_.HP_ -= _Amount;
	if (0.0f >= StateInfo_.HP_)
	{
		// 0.0f 이하면 사망처리
		StateInfo_.HP_ = 0.0f;

		// 사망중상태로 전환
		// 단, 서버이면 상태전환 후 패킷 송신
		//     클라이면 패킷 송신 후 서버로부터 패킷수신시 상태전환
		if (true == GameServer::GetInstance()->IsOpened())
		{
			ChangeAnimationAndState(MonsterStateType::DEATH);
		}

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
	if (true == GameServer::GetInstance()->IsOpened())
	{
		ChangeAnimationAndState(MonsterStateType::HIT);
	}

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
	UpdateSpecialStat(_rcvStatInfo);

	// 일반공격01 상태 전환
	ChangeAnimationAndState(MonsterStateType::ATK01);
}

void Monsters::rcvAttack02(MonsterStateInfo _rcvStatInfo)
{
	// 상태정보 갱신
	UpdateSpecialStat(_rcvStatInfo);

	// 일반공격02 상태 전환
	ChangeAnimationAndState(MonsterStateType::ATK02);
}

void Monsters::rcvDamage(MonsterStateInfo _rcvStatInfo, int _TargetIndex)
{
	// 상태정보 갱신
	UpdateSpecialStat(_rcvStatInfo);

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
	if (MonsterStateType::DEATH == CurStateType_)
	{
		return;
	}

	// 상태정보 갱신
	UpdateSpecialStat(_rcvStatInfo);

	// 사망중상태로 전환
	ChangeAnimationAndState(MonsterStateType::DEATH);
}

void Monsters::rcvDead(MonsterStateInfo _rcvStatInfo)
{
	// 이미상 사망중 or 사망상태라면 무시
	if (MonsterStateType::DEAD == CurStateType_)
	{
		return;
	}

	// 상태정보 갱신
	UpdateSpecialStat(_rcvStatInfo);

	// 사망중상태로 전환
	ChangeAnimationAndState(MonsterStateType::DEAD);
}

void Monsters::rcvRegen(MonsterStateInfo _rcvStatInfo)
{
	// 단, 이미 리젠상태이거나 재등장완료하여 대기상태이면 처리무시
	if (CurStateType_ == MonsterStateType::REGEN || CurStateType_ == MonsterStateType::IDLE)
	{
		return;
	}

	// 상태정보 갱신
	UpdateSpecialStat(_rcvStatInfo);

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
	UpdateSpecialStat(_rcvStatInfo);

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
	MoveTarget_ = SpawnPos;

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

void Monsters::UpdateSpecialStat(MonsterStateInfo _UpdateStat)
{
	// 갱신용 스텟만을 수신하여 갱신
	StateInfo_.Level_ = _UpdateStat.Level_;
	StateInfo_.OffencePower_ = _UpdateStat.OffencePower_;
	StateInfo_.Defense_ = _UpdateStat.Defense_;
	StateInfo_.HPMax_ = _UpdateStat.HPMax_;
	StateInfo_.HP_ = _UpdateStat.HP_;
	StateInfo_.MoveSpeed_ = _UpdateStat.MoveSpeed_;
	StateInfo_.HomingInstinctValue_ = _UpdateStat.HomingInstinctValue_;
	StateInfo_.RegenTime_ = _UpdateStat.RegenTime_;
	StateInfo_.SkillCoolDown_ = _UpdateStat.SkillCoolDown_;
}

void Monsters::UpdateAllStat(MonsterStateInfo _UpdateStat)
{
	// 전체 스텟을 수신하여 갱신
	StateInfo_ = _UpdateStat;
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

	// 2. 스킬쿨타임 갱신
	UpdateSkillCoolDown(_DeltaTime);

	// 3. ...

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
		if (false == IsDeath_)
		{
			GetLevel()->PushDebugRender(BodyCollider_->GetTransform(), CollisionType::OBBBox3D, float4::BLUE);
		}

		// 221018 SJH : 임시주석처리
		//// 마우스 그룹과의 충돌
		//// 단, 몬스터 완전사망상태이며 피격판정무시상태라면 충돌체크하며, 해당 충돌시 드랍된아이템을 표시
		//if (MonsterStateType::DEAD == CurStateType_ && true == IsDeath_)
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
		GetLevel()->PushDebugRender(AtkCollider_->GetTransform(), CollisionType::OBBBox3D, float4::BLUE);
#endif // _DEBUG

		// 플레이어 그룹과의 충돌
		if (true == AtkCollider_->Collision(static_cast<int>(eCollisionGroup::Player)) && true == IsAttack_)
		{
			if (nullptr != CurTarget_)
			{
				// 현재 타겟으로 지정된 플레이어에게 데미지를 입힘
				CurTarget_->Damage(static_cast<float>(StateInfo_.OffencePower_), this);

				// 공격완료로 공격상태 Flag Off
				IsAttack_ = false;
				AtkCollider_->Off();
			}
			else
			{
				// 타겟이 없으므로 공격상태 Flag Off
				IsAttack_ = false;
				AtkCollider_->Off();
			}
		}
		// 충돌하지않았다면 공격상태 바로 Off
		else
		{
			IsAttack_ = false;
			AtkCollider_->Off();
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

void Monsters::UpdateSkillCoolDown(float _DeltaTime)
{
	// 단, 해당 몬스터가 스킬공격이 있을때만 쿨타임 갱신
	if (1 == StateInfo_.IsSkill_)
	{
		StateInfo_.SkillCoolDown_ -= _DeltaTime;
		if (0.0f >= StateInfo_.SkillCoolDown_ && MonsterStateType::SKILLATTACK != CurStateType_)
		{
			StateInfo_.SkillCoolDown_ = 0.0f;
		}
	}
}

void Monsters::StartMove(const float4& _Position)
{
	// 이동경로 Get
	if (nullptr == CurrentMap_)
	{
		GameEngineDebug::MsgBoxError("현재 맵이 지정되어있지않습니다!!!!!!!");
		return;
	}

	MovePath_.clear();
	MovePath_ = CurrentMap_->FindPath(GetTransform()->GetWorldPosition(), _Position);
	if (true == MovePath_.empty())
	{
		GameEngineDebug::OutPutDebugString("이동경로 생성에 실패하였습니다.");
		return;
	}

	// 현재 이동경로의 이동목적지를 설정
	MoveTarget_ = MovePath_.back();
	MovePath_.pop_back();
}

void Monsters::UpdateMove(float _DeltaTime)
{
	// 현재 이동목적지 - 몬스터위치의 거리 측정
	float4 MyPosition = transform_.GetWorldPosition();
	MyPosition.y = MoveTarget_.y;
	if ((MoveTarget_ - MyPosition).Len3D() > 10.0f)
	{
		// 이동처리
		MoveProcessing(_DeltaTime, MyPosition);
	}
	else
	{
		// 남아있는 경로가 존재할때 새로운 목적지 설정
		if (false == MovePath_.empty())
		{
			MoveTarget_ = MovePath_.back();
			MovePath_.pop_back();
		}
		// 남아있는 경로가 존재하지않을때 이동종료
		else
		{
			EndMove();
		}
	}
}

void Monsters::EndMove()
{
	// 경로관련 초기화
	MoveTarget_ = float4::ZERO;
	MovePath_.clear();

	// 대기상태 돌입
	ChangeAnimationAndState(MonsterStateType::IDLE);
}

void Monsters::MoveProcessing(float _DeltaTime, const float4& _Position)
{
	// 이동방향 계산 및 회전
	CalcMoveDir(_Position);

	// 이동위치 계산
	float4 MoveSpeed = MoveDir_ * StateInfo_.MoveSpeed_ * _DeltaTime * 100.0f;		// 이동속도 기본계수가 너무 작기때문에 *100을 해서 높인다
	float4 NextMovePos = _Position + MoveSpeed;

	// 이동가능체크 후 이동가능하다면 이동처리
	float Dist = 0.0f;
	if (true == CurrentMap_->GetNavMesh()->CheckIntersects(NextMovePos + float4{ 0.0f, FT::Map::MAX_HEIGHT, 0.0f }, float4::DOWN, Dist))
	{
		NextMovePos.y = FT::Map::MAX_HEIGHT - Dist;
		GetTransform()->SetWorldPosition(NextMovePos);
	}
	else
	{
		MoveTarget_ = _Position;
	}
}

void Monsters::CalcMoveDir(const float4& _Position)
{
	// 이동방향 계산 및 단위행렬화
	MoveDir_ = MoveTarget_ - _Position;
	MoveDir_.Normalize3D();

	// 회전처리
	float4 Cross = float4::Cross3D(MoveDir_, { 0.0f, 0.0f, 1.0f });
	Cross.Normalize3D();

	float Angle = float4::DegreeDot3DToACosAngle(MoveDir_, {0.0f, 0.0f, 1.0f});
	GetTransform()->SetLocalRotationDegree({ 0.0f, Angle * -Cross.y, 0.0f });
}

void Monsters::AttackProcessing()
{
	// 스킬쿨이 우선순위
	if (1 == StateInfo_.IsSkill_)										// 스킬공격이 있는경우
	{
		if (0.0f >= StateInfo_.SkillCoolDown_)							// Update(float _DeltaTime)에서 감소중
		{
			// 쿨타임 초기화
			StateInfo_.SkillCoolDown_ = StateInfo_.SkillCoolDownMax_;

			// 스킬상태 전환
			//ChangeAnimationAndState(MonsterStateType::SKILLATTACK);
		}
		else															// 스킬시전쿨타임중이므로 일반공격
		{
			ChangeAnimationAndState(MonsterStateType::ATK01);
			//ChangeAnimationAndState(MonsterStateType::ATK02);
		}
	}
	else									// 일반공격만 가능한 경우
	{
		ChangeAnimationAndState(MonsterStateType::ATK01);
		//ChangeAnimationAndState(MonsterStateType::ATK02);
	}
}

void Monsters::DebugWindowUpdate()
{
	// MonsterDebugWindow



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
	, MoveDir_(float4::ZERO)
	, MoveTarget_(float4::ZERO)
	, MainState_()
	, NormalState_()
	, CrowdControlState_()
	, AttackState_()
	, CurTarget_(nullptr)
	, CurTargetIndex_(-1)
	, CurStateBasicType_(MonsterStateBasicType::NONE)
	, PrevStateType_(MonsterStateType::NONE)
	, CurStateType_(MonsterStateType::NONE)
	, IsDeath_(false)
	, IsAttack_(false)
{
}

Monsters::~Monsters()
{
}
