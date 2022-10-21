#include "PreCompile.h"
#include "Boar.h"

#include <GameEngine/GameEngineCollision.h>

#include "GameServer.h"
#include "GameClient.h"
#include "CharCrowdControlPacket.h"

#include "LumiaLevel.h"
#include "Character.h"

bool Boar::ResourceLoadFlag = false;

void Boar::InitalizeStateInfo()
{
	// 기본정보
	Type_ = MonsterType::BOAR;											// 몬스터타입

	//====================================== 스텟의 최대치(고정)
	StateInfo_.LevelMin_ = 2;											// 시작 레벨(리젠시 해당 데이터로 레벨초기화)
	StateInfo_.LevelMax_ = 12;											// 최대 레벨
	StateInfo_.HomingInstinctValueMax_ = 100.0f;						// 최대 귀소본능 수치
	StateInfo_.RegenTimeMax_ = 195.0f;									// 리젠타임
	StateInfo_.SkillCoolDownMax_ = 7.0f;								// 최대 스킬쿨타임

	//====================================== 레벨당 증가량(고정)
	StateInfo_.OffencePowerIncrement_ = 23;								// 레벨당 공격력 증가량
	StateInfo_.DefenseIncrement_ = 7;									// 레벨당 방어력 증가량
	StateInfo_.MoveSpeedIncrement_ = 0.05f;								// 레벨당 이동속도 증가량
	StateInfo_.HPIncrement_ = 167.0f;									// 레벨당 레벨당 체력 증가량

	//====================================== 기본스텟(고정)
	StateInfo_.NestPosition_ = float4::ZERO;							// 둥지위치(= 스폰위치)
	StateInfo_.AttackSpeed_ = 0.5f;										// 공격속도
	StateInfo_.AttackRange_ = 100.0f;									// 공격 시야거리
	StateInfo_.DetectRange_ = 300.0f;									// 감지 시야거리

	//====================================== 기본스텟(갱신) - 초기: 2레벨기준 스텟
	StateInfo_.Level_ = 2;												// 레벨(늦게 생성된 몬스터(야생동물)일수록 레벨이 높게 설정) - 생성과 동시에 지정(지정없이 생성된 몬스터의 경우 기본 1레벨고정)
	StateInfo_.OffencePower_ = 51;										// 공격력
	StateInfo_.Defense_ = 16;											// 방어력
	StateInfo_.HPMax_ = 942.0f;											// 최대 체력(레벨당 변화)
	StateInfo_.HP_ = 942.0f;											// 현재체력
	StateInfo_.MoveSpeed_ = 2.85f;										// 이동속도
	StateInfo_.HomingInstinctValue_ = 100.0f;							// 귀소본능 수치(0.0f보다 작거나같아지면 몬스터는 스폰위치로 이동하면 체력을 회복 -> 타겟이 DetectRange_에 벗어난 시간에 따라 감소)
	StateInfo_.RegenTime_ = 195.0f;										// 리젠타임(갱신) -> 0.0f이하일시 RegenTime_으로 초기화

	//====================================== 스킬관련
	StateInfo_.IsSkill_ = 1;											// 스킬여부(0: 스킬없음)
	StateInfo_.SkillCoolDown_ = 7.0f;									// 스킬재사용시간

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

void Boar::InitalizeResourceLoad()
{
	// 리소스 로드는 한번만 처리
	if (false == ResourceLoadFlag)
	{
		// BaseMesh Resource Load & (Create Vertex & Index Buffer)
		GameEngineDirectory MeshDir;
		MeshDir.MoveParent("FoxTeam");
		MeshDir / "Resources" / "FBX" / "Monster" / "Boar";

		GameEngineFBXMesh* Basemesh = GameEngineFBXMeshManager::GetInst().Load(MeshDir.PathToPlusFileName("Boar_BaseMesh.fbx"));
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

void Boar::InitalizeRenderAndAnimation()
{
	// 렌더러 셋팅
	MainRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	MainRenderer_->SetFBXMesh("Boar_BaseMesh.fbx", "TextureDeferredLightAni");

	MainRenderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	MainRenderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	// 애니메이션 생성
	MainRenderer_->CreateFBXAnimation("APPEAR", "Boar_appear.UserAnimation", 0, false);			// 첫등장상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("REGEN", "Boar_appear.UserAnimation", 0, false);			// 리젠상태(몬스터 사망 후 리젠타임에 의해 리젠한 상태)의 애니메이션
	MainRenderer_->CreateFBXAnimation("IDLE", "Boar_wait.UserAnimation", 0);						// 대기상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("CHASE", "Boar_run.UserAnimation", 0);						// 추적상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("HOMINGINSTINCT", "Boar_run.UserAnimation", 0);				// 귀환상태의 애니메이션

	MainRenderer_->CreateFBXAnimation("HIT", "Boar_wait.UserAnimation", 0);						// 피격상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("DEATH", "Boar_death.UserAnimation", 0, false);				// 사망중상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("DEAD", "Boar_death.UserAnimation", 0, false);				// 사망(리젠대기)상태의 애니메이션

	MainRenderer_->CreateFBXAnimation("ATK01", "Boar_atk01.UserAnimation", 0, false);				// 일반공격01상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("ATK02", "Boar_atk02.UserAnimation", 0, false);				// 일반공격02상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("SKILLATTACK", "Boar_skill_ready.UserAnimation", 0, false);		// 스킬공격상태의 애니메이션
	MainRenderer_->ChangeFBXAnimation("IDLE");

	// 기본상태 셋팅
	ChangeAnimationAndState(MonsterStateType::APPEAR);
}

void Boar::InitalizeCollider()
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

void Boar::SkillAttackProcessing(float _DeltaTime)
{
	// 돌진 : 일직선으로 돌격하며 적을 밀어내며 피해를 입힘
	//		  단, 적이 벽에 부딪혔다면 기절
	// 피해량 - 기본(150) + 공격력의 300%
	// 사정거리 - 5m
	// 시전시간 - 1.5초
	// 쿨다운 - 7초
	// 1m == 100.0f로 계산
	
	//if (false == SkillAtk_)
	//{
	//	// 현재 타겟이 스킬사정거리내 진입했으며 스킬시전시간을 모두 소모했다면 스킬공격 시작
	//	SkillAtk_CastTime_ -= _DeltaTime;
	//	if (0.0f >= SkillAtk_CastTime_)
	//	{
	//		float4 MyPosition = GetTransform()->GetWorldPosition();
	//		float4 TargetPosition = CurTarget_->GetTransform()->GetWorldPosition();
	//		if ((TargetPosition - MyPosition).Len3D() <= SkillAtk_Range_)
	//		{
	//			// 스킬공격 애니메이션 실행
	//			MainRenderer_->ChangeFBXAnimation("SKILLATTACK");

	//			// 스킬공격처리를 위한 준비
	//			SkillAtk_ = true;

	//			// 공격충돌체와의 플레이어의 피격충돌체와 충돌체크를 위해 On
	//			IsAttack_ = true;
	//			AtkCollider_->On();

	//			// 타겟 넉백 방향 계산
	//			float4 KnockbackDir = TargetPosition - MyPosition;

	//			// 타겟 넉백 패킷전송
	//			CharCrowdControlPacket ccPacket;
	//			ccPacket.SetTargetIndex(CurTarget_->GetIndex());
	//			ccPacket.SetWallSlam(0.2f, KnockbackDir * 3000.f, 1.0f);
	//			FT::SendPacket(ccPacket);

	//			// 사운드?
	//			//GameEngineSoundManager::GetInstance()->PlaySoundByName("hyunwoo_Skill03_Hit.wav");
	//			//PacketSoundPlay packet;
	//			//packet.SetSound("hyunwoo_Skill03_Hit.wav", transform_.GetWorldPosition());
	//			//FT::SendPacket(packet);


	//			CurTarget_->Knockback();

	//			// 타겟 데미지
	//			float CurDmage = SkillAtk_FixedDamage_ + (static_cast<float>(StateInfo_.OffencePower_) * 3.0f);
	//			CurTarget_->Damage(CurDmage, this);
	//		}

	//		// 스킬시전시간 초기화
	//		SkillAtk_CastTime_ = SkillAtk_CastTimeMax_;
	//	}
	//}

	// 모션종료시 
	if ("SKILLATTACK" == MainRenderer_->GetCurAnimationName() && true == MainRenderer_->CheckCurrentAnimationEnd())
	{
		// 스킬공격완료
		SkillAtk_ = false;

		// 모션종료시 대기상태 전환
		ChangeAnimationAndState(MonsterStateType::IDLE);
	}
}

Boar::Boar()
	: SkillAtk_(false)
	, SkillAtk_Range_(500.0f)
	, SkillAtk_CastTimeMax_(1.5f)
	, SkillAtk_CastTime_(1.5f)
	, SkillAtk_FixedDamage_(150.0f)
{
}

Boar::~Boar()
{
}
