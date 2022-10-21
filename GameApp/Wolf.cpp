#include "Precompile.h"
#include "Wolf.h"

#include <GameEngine/GameEngineCollision.h>

#include "LumiaLevel.h"
#include "Character.h"

bool Wolf::ResourceLoadFlag = false;

void Wolf::MusterSkill(Character* _Target)
{
	// 해당 스킬을 시전한 몬스터가 타겟으로 지정한 타겟을 나의 타겟으로 지정하여 추적 및 공격 시작
	// 단, 이미 지정된 타겟이 존재한다면 무시
	if (nullptr == CurTarget_)
	{
		CurTarget_ = _Target;
		CurTargetIndex_ = _Target->GetIndex();
	}
}

void Wolf::InitalizeStateInfo()
{
	// 기본정보
	Type_ = MonsterType::WOLF;											// 몬스터타입

	//====================================== 스텟의 최대치(고정)
	StateInfo_.LevelMin_ = 3;											// 시작 레벨(리젠시 해당 데이터로 레벨초기화)
	StateInfo_.LevelMax_ = 20;											// 최대 레벨
	StateInfo_.HomingInstinctValueMax_ = 100.0f;						// 최대 귀소본능 수치
	StateInfo_.RegenTimeMax_ = 240.0f;									// 리젠타임
	StateInfo_.SkillCoolDownMax_ = 10.0f;								// 최대 스킬쿨타임

	//====================================== 레벨당 증가량(고정)
	StateInfo_.OffencePowerIncrement_ = 23;								// 레벨당 공격력 증가량
	StateInfo_.DefenseIncrement_ = 4;									// 레벨당 방어력 증가량
	StateInfo_.MoveSpeedIncrement_ = 0.05f;								// 레벨당 이동속도 증가량
	StateInfo_.HPIncrement_ = 83.9375f;									// 레벨당 레벨당 체력 증가량

	//====================================== 기본스텟(고정)
	StateInfo_.NestPosition_ = float4::ZERO;							// 둥지위치(= 스폰위치)
	StateInfo_.AttackSpeed_ = 0.9f;										// 공격속도
	StateInfo_.AttackRange_ = 100.0f;									// 공격 시야거리
	StateInfo_.DetectRange_ = 300.0f;									// 감지 시야거리

	//====================================== 기본스텟(갱신) - 초기: 3레벨기준 스텟
	StateInfo_.Level_ = 3;												// 레벨(늦게 생성된 몬스터(야생동물)일수록 레벨이 높게 설정) - 생성과 동시에 지정(지정없이 생성된 몬스터의 경우 기본 1레벨고정)
	StateInfo_.OffencePower_ = 44;										// 공격력
	StateInfo_.Defense_ = 28;											// 방어력
	StateInfo_.HPMax_ = 638.0f;											// 최대 체력(레벨당 변화)
	StateInfo_.HP_ = 638.0f;											// 현재체력
	StateInfo_.MoveSpeed_ = 2.9f;										// 이동속도
	StateInfo_.HomingInstinctValue_ = 100.0f;							// 귀소본능 수치(0.0f보다 작거나같아지면 몬스터는 스폰위치로 이동하면 체력을 회복 -> 타겟이 DetectRange_에 벗어난 시간에 따라 감소)
	StateInfo_.RegenTime_ = 240.0f;										// 리젠타임(갱신) -> 0.0f이하일시 RegenTime_으로 초기화

	//====================================== 스킬관련
	StateInfo_.IsSkill_ = 1;											// 스킬여부(0: 스킬없음)
	StateInfo_.SkillCoolDown_ = 0.5f;									// 스킬재사용시간

	//====================================== 아이템관련
	StateInfo_.DropItems_[ItemName::MEAT] = 40.0f;
	StateInfo_.DropItems_[ItemName::LEATHER] = 60.0f;

	if (true == GameServer::GetInstance()->IsOpened())					// 서버이면 아이템정보 수집
	{
		// 일단 고정
		StateInfo_.CurDropItems_.push_back(ItemName::MEAT);
		StateInfo_.CurDropItems_.push_back(ItemName::LEATHER);
	}
}

void Wolf::InitalizeResourceLoad()
{
	// 리소스 로드는 한번만 처리
	if (false == ResourceLoadFlag)
	{
		// BaseMesh Resource Load & (Create Vertex & Index Buffer)
		GameEngineDirectory MeshDir;
		MeshDir.MoveParent("FoxTeam");
		MeshDir / "Resources" / "FBX" / "Monster" / "Wolf";

		GameEngineFBXMesh* Basemesh = GameEngineFBXMeshManager::GetInst().Load(MeshDir.PathToPlusFileName("Wolf_BaseMesh.fbx"));
		Basemesh->CreateRenderingBuffer();

		std::vector<GameEngineFile> allFile = MeshDir.GetAllFile("UserAnimation");
		for (GameEngineFile& file : allFile)
		{
			GameEngineFBXAnimationManager::GetInst().LoadUser(file.GetFullPath());
		}

		// Monster Resource Load Flag On
		ResourceLoadFlag = true;
	}
}

void Wolf::InitalizeRenderAndAnimation()
{
	// 렌더러 셋팅
	MainRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	MainRenderer_->SetFBXMesh("Wolf_BaseMesh.fbx", "TextureDeferredLightAni");

	MainRenderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	MainRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f, 0.0f });

	// 애니메이션 생성
	MainRenderer_->CreateFBXAnimation("APPEAR", "Wolf_appear.UserAnimation", 0, false);			// 첫등장상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("REGEN", "Wolf_appear.UserAnimation", 0, false);			// 리젠상태(몬스터 사망 후 리젠타임에 의해 리젠한 상태)의 애니메이션
	MainRenderer_->CreateFBXAnimation("IDLE", "Wolf_wait.UserAnimation", 0);						// 대기상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("CHASE", "Wolf_run.UserAnimation", 0);						// 추적상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("HOMINGINSTINCT", "Wolf_run.UserAnimation", 0);				// 귀환상태의 애니메이션

	MainRenderer_->CreateFBXAnimation("HIT", "Wolf_wait.UserAnimation", 0);						// 피격상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("DEATH", "Wolf_death.UserAnimation", 0, false);				// 사망중상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("DEAD", "Wolf_death.UserAnimation", 0, false);				// 사망(리젠대기)상태의 애니메이션

	MainRenderer_->CreateFBXAnimation("ATK01", "Wolf_atk01.UserAnimation", 0, false);				// 일반공격01상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("ATK02", "Wolf_atk02.UserAnimation", 0, false);				// 일반공격02상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("SKILLATTACK", "Wolf_skill.UserAnimation", 0, false);		// 스킬공격상태의 애니메이션
	MainRenderer_->ChangeFBXAnimation("IDLE");

	// 기본상태 셋팅
	ChangeAnimationAndState(MonsterStateType::APPEAR);
}

void Wolf::InitalizeCollider()
{
	// 기본: 몸체 충돌체 생성
	BodyCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	BodyCollider_->SetCollisionGroup(eCollisionGroup::Monster);
	BodyCollider_->SetCollisionType(CollisionType::OBBBox3D);
	BodyCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	BodyCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());

	// 추가: 공격 충돌체 생성(옵션)
	AtkCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	AtkCollider_->SetCollisionGroup(eCollisionGroup::MonsterAttack);
	AtkCollider_->SetCollisionType(CollisionType::OBBBox3D);
	AtkCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	AtkCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());
	AtkCollider_->Off();
}

void Wolf::SkillAttackProcessing(float _DeltaTime)
{
	if (false == SkillAtk_)
	{
		// 애니메이션 실행 및 Flag On, Attack Collider On
		MainRenderer_->ChangeFBXAnimation("SKILLATTACK");
		IsAttack_ = true;
		AtkCollider_->On();

		// 충돌체 Off상태에서 시전
		// 소집: 반경 20m의 늑대들을 불러모아서 함께 적을 공격(즉시시전)
		// 1m == 100.0f로 계산
		std::vector<Monsters*>& WolfList = GetLevelConvert<LumiaLevel>()->GetMonsterTypeList(MonsterType::WOLF);
		if (true == WolfList.empty())
		{
			// 주변에 늑대가 존재하지않으므로 처리없음....
			GameEngineDebug::OutPutDebugString("늑대가 소집스킬을 시전했으나 주변 20m에 늑대가 존재하지않습니다!!!!\n");
		}
		else
		{
			int WolfCount = static_cast<int>(WolfList.size());
			for (int WolfNum = 0; WolfNum < WolfCount; ++WolfNum)
			{
				// 나와의 거리 측정하여 스킬사정거리내에 존재하는 늑대를 소집
				float4 MyPosition = GetTransform()->GetWorldPosition();
				float4 MusterWolfPosition = WolfList[WolfNum]->GetTransform()->GetWorldPosition();
				if ((MusterWolfPosition - MyPosition).Len3D() <= SkillAtk_Range_ && WolfList[WolfNum]->GetIndex() != Index_)
				{
					Monsters* CheckMonster = WolfList[WolfNum];
					Wolf* CheckWolf = dynamic_cast<Wolf*>(CheckMonster);
					CheckWolf->MusterSkill(CurTarget_);
				}
			}
		}

		SkillAtk_ = true;
	}

	// 모션종료시 
	if ("SKILLATTACK" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->CheckCurrentAnimationEnd())
	{
		// 스킬공격 끝
		SkillAtk_ = false;

		// 모션종료시 대기상태 전환
		ChangeAnimationAndState(MonsterStateType::IDLE);
	}
}

Wolf::Wolf()
	: SkillAtk_(false)
	, SkillAtk_Range_(2000.0f)
	, SkillAtk_CastTime_(0.0f)
{

}

Wolf::~Wolf()
{
}
