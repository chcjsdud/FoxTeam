#include "PreCompile.h"

#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

#include "UI_Skill.h"
#include "LH_LockOnUI.h"
#include "LH_Inventory.h"

#include "Enums.h"

#include "LH_Yuki.h"

Player_Yuki::Player_Yuki()
{
}

Player_Yuki::~Player_Yuki()
{
}

void Player_Yuki::Start()
{
	StateInit();
	ComponenetInit();
	UIInit();
}

void Player_Yuki::Update(float _DeltaTime)
{
	if (Iscontrolled_ == false)
	{
		//AIController_->Update(_DeltaTime);
	}

	DEBUGUpdate(_DeltaTime);

	UpdateBuff(_DeltaTime);

	EnemyInSight_ = UnitSightCollision_->Collision(static_cast<int>(CollisionGroup::Monster));

	State_.Update(_DeltaTime);
	//CameraState_.Update(_DeltaTime);
}

void Player_Yuki::ComponenetInit()
{
	// 렌더러
	FBXRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	FBXRenderer_->SetFBXMesh("AnimMan.FBX", "ColorAni");

	for (UINT i = 0; i < FBXRenderer_->GetRenderSetCount(); i++)
	{
		FBXRenderer_->GetRenderSet(i).ShaderHelper->SettingConstantBufferSet("ResultColor", float4::RED);
	}

	FBXRenderer_->CreateFBXAnimation("TestRun", "ALS_N_RUN_F.FBX");// 달리기
	FBXRenderer_->ChangeFBXAnimation("TestRun");

	//타격 히트 박스
	AttackHitBoxCollision_->GetTransform()->SetLocalScaling(float4{ 200.f,50.f,100.f ,1.f });
	AttackHitBoxCollision_->GetTransform()->SetLocalPosition({ 0.f,100.f,100.f });
	AttackHitBoxCollision_->SetCollisionGroup(CollisionGroup::PlayerAttack);
	AttackHitBoxCollision_->Off();

	//피격 히트박스 겸사겸사 맵 컬리전도 가능할듯
	UnitHitBoxCollision_->GetTransform()->SetLocalScaling(float4{ 100.f,200.f,100.f ,1.f });
	UnitHitBoxCollision_->GetTransform()->SetLocalPosition({ 0.f,100.f,0.f });
	UnitHitBoxCollision_->SetCollisionInfo(CINT(CollisionGroup::Player), CollisionType::AABBBox3D);
	//PlayerHitBoxCollision_->SetCollisionGroup(static_cast<int>(CollisionGroup::Player));


	UnitSightCollision_->GetTransform()->SetLocalScaling(float4{ 800.f,0.f,800.f });
	UnitSightCollision_->SetCollisionInfo(static_cast<int>(CollisionGroup::PlayerSight), CollisionType::CirCle);
}

void Player_Yuki::StateInit()
{
	//State 함수
	//{
	//	PlayerState_.CreateState<Player>("Idle", this, &Player::Idle_Start, &Player::Idle_Update, &Player::Idle_End);;
	//	PlayerState_.CreateState<Player>("Walk", this, &Player::Walk_Start, &Player::Walk_Update, &Player::Walk_End);;
	//	PlayerState_.CreateState<Player>("Run", this, &Player::Run_Start, &Player::Run_Update, &Player::Run_End);;
	//	PlayerState_.CreateState<Player>("Attack", this, &Player::Attack_Start, &Player::Attack_Update, &Player::Attack_End);;

		// 마우스 커서 이동 시험용 스테이트
	//State_.CreateState<Player>("Stand", this, &Player::Stand_Start, &Player::Stand_Update, &Player::Stand_End);;
	//State_.CreateState<Player>("Move", this, &Player::Move_Start, &Player::Move_Update, &Player::Move_End);;

	//State_.ChangeState("Stand");

}

void Player_Yuki::UIInit()
{
	SkillUI_ = GetLevel()->CreateActor<UI_Skill>();
	SkillUI_->GetTransform()->SetWorldPosition(float4(0.0f, 0.0f, 0.0f, 0.f));
	SkillUI_->SetPlayer(this);

	LockOnUI_ = GetLevel()->CreateActor<LockOnUI>();
	LockOnUI_->GetTransform()->SetWorldPosition(float4(0.0f, 0.0f, 0.0f));
	LockOnUI_->SetPlayer(this);
	LockOnUI_->Off();

	//Inventory_ = GetLevel()->CreateActor<Inventory>();
	//Inventory_->SetPlayer(this);
}

void Player_Yuki::DEBUGUpdate(float _DeltaTime)
{
	GetLevel()->PushDebugRender(UnitHitBoxCollision_->GetTransform(), CollisionType::AABBBox3D);
	GetLevel()->PushDebugRender(UnitSightCollision_->GetTransform(), CollisionType::CirCle);

	if (AttackHitBoxCollision_->IsUpdate())
	{
		GetLevel()->PushDebugRender(AttackHitBoxCollision_->GetTransform(), CollisionType::AABBBox3D, float4::RED);
	}
}

//void Yuki::Initialize()
//{
//	// 고유정보 셋팅
//	BaseInfoSetting();
//
//	// 스킬정보 셋팅
//	SkillInfoSetting();
//}
