#include "PreCompile.h"
#include "Boar.h"

#include <GameEngine/GameEngineCollision.h>

bool Boar::ResourceLoadFlag = false;

void Boar::InitalizeStateInfo()
{
	// 기본정보
	Type_ = MonsterType::BOAR;											// 몬스터타입

	// 기본스텟
	StateInfo_.HPMax_ = 100.0f;											// 최대체력
	StateInfo_.HP_ = 100.0f;											// 현재체력
	StateInfo_.HPRegenPercent_ = 0.5f;									// 체력회복량(% 수치)
	StateInfo_.SPMax_ = 100.0f;											// 최대마력
	StateInfo_.SP_ = 100.0f;											// 현재마력
	StateInfo_.SPRegenPercent_ = 0.5f;									// 마력회복량(% 수치)
	StateInfo_.Defence_ = 100.0f;										// 방어력

	// 상세스텟
	StateInfo_.NestPosition_ = float4::ZERO;							// 둥지위치(= 스폰위치)
	StateInfo_.HomingInstinctValueMax_ = 100.0f;						// 최대 귀소본능 수치
	StateInfo_.HomingInstinctValue_ = 100.0f;							// 귀소본능 수치(0.0f보다 작거나같아지면 몬스터는 스폰위치로 이동하면 체력을 회복 -> 타겟이 DetectRange_에 벗어난 시간에 따라 감소)
	StateInfo_.AttackRange_ = 100.0f;									// 공격 시야거리
	StateInfo_.DetectRange_ = 300.0f;									// 감지 시야거리
	StateInfo_.AttackSpeed_ = 0.5f;										// 공격 속도
	StateInfo_.MoveSpeed_ = 100.0f;										// 이동 속도

	// 스킬스텟
	StateInfo_.SkillCoolDown_ = 0.5f;									// 스킬재사용시간

	// 젠스텟
	StateInfo_.RegenTimeMax_ = 125.f;									// 리젠타임(고정)
	StateInfo_.RegenTime_ = 125.f;										// 리젠타임(갱신) -> 0.0f이하일시 RegenTime_으로 초기화
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

		// Animation Resource Load
		GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Boar_appear.fbx"));
		GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Boar_wait.fbx"));
		GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Boar_run.fbx"));
		GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Boar_death.fbx"));
		GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Boar_atk01.fbx"));
		GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Boar_atk02.fbx"));
		GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Boar_skill_assault.fbx"));		// 로드만함 구현은 추후 생각좀...
		GameEngineFBXAnimationManager::GetInst().Load(MeshDir.PathToPlusFileName("Boar_skill_ready.fbx"));			// 로드만함 구현은 추후 생각좀...

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
	MainRenderer_->CreateFBXAnimation("APPEAR", "Boar_appear.fbx", 0, false);			// 첫등장상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("REGEN", "Boar_appear.fbx", 0, false);			// 리젠상태(몬스터 사망 후 리젠타임에 의해 리젠한 상태)의 애니메이션
	MainRenderer_->CreateFBXAnimation("IDLE", "Boar_wait.fbx", 0);						// 대기상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("CHASE", "Boar_run.fbx", 0);						// 추적상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("HOMINGINSTINCT", "Boar_run.fbx", 0);				// 귀환상태의 애니메이션

	MainRenderer_->CreateFBXAnimation("HIT", "Boar_wait.fbx", 0);						// 피격상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("DEATH", "Boar_death.fbx", 0, false);				// 사망중상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("DEAD", "Boar_death.fbx", 0, false);				// 사망(리젠대기)상태의 애니메이션

	MainRenderer_->CreateFBXAnimation("ATK01", "Boar_atk01.fbx", 0, false);				// 일반공격01상태의 애니메이션
	MainRenderer_->CreateFBXAnimation("ATK02", "Boar_atk02.fbx", 0, false);				// 일반공격02상태의 애니메이션
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
	//AtkCollider_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	//AtkCollider_->SetCollisionGroup(eCollisionGroup::MonsterAttack);
	//AtkCollider_->SetCollisionType(CollisionType::AABBBox3D);
	//AtkCollider_->GetTransform()->SetLocalScaling(MainRenderer_->GetTransform()->GetLocalScaling());
	//AtkCollider_->GetTransform()->SetLocalPosition(MainRenderer_->GetTransform()->GetLocalPosition());
	//AtkCollider_->Off();
}

Boar::Boar()
{
}

Boar::~Boar()
{
}
