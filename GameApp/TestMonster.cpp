#include "PreCompile.h"
#include "TestMonster.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

#include "Enums.h"
#include "Player.h"

TestMonster::TestMonster() // default constructer 디폴트 생성자
	: FBXRenderer_(nullptr)
	, monsterGroundCollision_(nullptr)
	, monsterHitBoxCollision_(nullptr)
	, monsterAttackHitBoxCollision_(nullptr)
	, monsterSightBoxCollision_(nullptr)
	, Speed_(10.f)
	, CurFowordDir_{ 0.f,0.f,1.f }
	, KeyDir_{ 0.f,0.f,1.f }
	, YRotateSpeed_(900.f)
	, IsMove_(false)
	, AttackTurm_(0.f)
	, Hp_(0)
	, Stamina_(0.f)
	, targetPlayer_(nullptr)
{

}

TestMonster::~TestMonster() // default destructer 디폴트 소멸자
{

}

void TestMonster::Start()
{
	StateInit();
	ComponenetInit();
}

void TestMonster::Update(float _DeltaTime)
{
	DEBUGUpdate(_DeltaTime);
	monsterState_.Update(_DeltaTime);
}

void TestMonster::ComponenetInit()
{
	// 렌더러
	FBXRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	FBXRenderer_->SetFBXMesh("AnimMan.FBX", "ColorAni");

	for (UINT i = 0; i < FBXRenderer_->GetRenderSetCount(); i++)
	{
		FBXRenderer_->GetRenderSet(i).ShaderHelper->SettingConstantBufferSet("ResultColor", float4::BLACK);
	}

	FBXRenderer_->CreateFBXAnimation("TestRun", "ALS_N_RUN_F.FBX");// 달리기
	FBXRenderer_->ChangeFBXAnimation("TestRun");

	//컬리전//
	monsterGroundCollision_ = CreateTransformComponent<GameEngineCollision>();
	monsterHitBoxCollision_ = CreateTransformComponent<GameEngineCollision>();
	monsterAttackHitBoxCollision_ = CreateTransformComponent<GameEngineCollision>();
	monsterSightBoxCollision_ = CreateTransformComponent<GameEngineCollision>();

	//타격 히트 박스
	monsterAttackHitBoxCollision_->GetTransform()->SetLocalScaling(float4{ 200.f,50.f,100.f ,1.f });
	monsterAttackHitBoxCollision_->GetTransform()->SetLocalPosition({ 0.f,100.f,100.f });
	monsterAttackHitBoxCollision_->Off();

	//피격 히트박스 겸사겸사 맵 컬리전도 가능할듯
	monsterHitBoxCollision_->GetTransform()->SetLocalScaling(float4{ 100.f,200.f,100.f ,1.f });
	monsterHitBoxCollision_->GetTransform()->SetLocalPosition({ 0.f,100.f,0.f });

	monsterSightBoxCollision_->GetTransform()->SetLocalScaling(float4{ 800.f,0.f,800.f});
	monsterSightBoxCollision_->GetTransform()->SetLocalPosition(float4{0.0f,0.0f,0.0f});
	monsterSightBoxCollision_->SetCollisionGroup(static_cast<int>(CollisionGroup::MonsterSight));
}

void TestMonster::StateInit()
{
	//State 함수
	{
		monsterState_.CreateState<TestMonster>("Idle", this, &TestMonster::Idle_Start, &TestMonster::Idle_Update, &TestMonster::Idle_End);;
		monsterState_.CreateState<TestMonster>("Pursuit", this, &TestMonster::Pursuit_Start, &TestMonster::Pursuit_Update, &TestMonster::Pursuit_End);;
		monsterState_.CreateState<TestMonster>("Attack", this, &TestMonster::Attack_Start, &TestMonster::Attack_Update, &TestMonster::Attack_End);;
		monsterState_.ChangeState("Idle");
	}
}

void TestMonster::MoveUpdate(float _DeltaTime)
{
	GetTransform()->SetLocalMove(moveVector_ * Speed_ * _DeltaTime);
}

void TestMonster::MoveRotateUpdate(float _DeltaTime)
{
	if (prevmoveVector_ == moveVector_) // 전 벡터와의 변동량이 없다면... 리턴
	{
		return;
	}

	float4 dir = float4::Cross3D(prevmoveVector_, moveVector_); // 외적 구하기

	float goaldegree = GameEngineMath::UnitVectorToDegree(moveVector_.z, moveVector_.x);
	// 전의 방향 벡터와 후의 방향 벡터와의 각도 비교

	if (dir.y >= 0.f)
	{

		GetTransform()->AddLocalRotationDegreeY(YRotateSpeed_ * _DeltaTime);
		prevmoveVector_.RotateYDegree(YRotateSpeed_ * _DeltaTime);

		dir = float4::Cross3D(prevmoveVector_, moveVector_);
		if (dir.y < 0.f)
		{
			GetTransform()->SetLocalRotationDegree({ 0.f,goaldegree,0.f });
			prevmoveVector_ = moveVector_;
		}
	}

	else if (dir.y < 0.f)
	{
		GetTransform()->AddLocalRotationDegreeY(-YRotateSpeed_ * _DeltaTime);
		prevmoveVector_.RotateYDegree(-YRotateSpeed_ * _DeltaTime);

		dir = float4::Cross3D(prevmoveVector_, moveVector_);
		if (dir.y > 0.f)
		{
			GetTransform()->SetLocalRotationDegree({ 0.f,goaldegree,0.f });
			prevmoveVector_ = moveVector_;
		}
	}
}



void TestMonster::DEBUGUpdate(float _DeltaTime)
{
	GetLevel()->PushDebugRender(monsterHitBoxCollision_->GetTransform(), CollisionType::AABBBox3D);
	GetLevel()->PushDebugRender(monsterSightBoxCollision_->GetTransform(), CollisionType::CirCle);
	if (monsterAttackHitBoxCollision_->IsUpdate())
	{
		GetLevel()->PushDebugRender(monsterAttackHitBoxCollision_->GetTransform(), CollisionType::AABBBox3D, float4::RED);
	}
}