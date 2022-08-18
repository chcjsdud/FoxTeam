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
	//: FBXRenderer_(nullptr)
{
}

Player_Yuki::~Player_Yuki()
{
}

void Player_Yuki::Start()
{
	Unit_Set_State_Init();
	ComponenetInit();
	UIInit();
}

void Player_Yuki::Unit_Set_State_Init()
{
	{
		// 이 밑부분은 나중에 플레이어.h 로 이동하게 될 것이다.
		ActionState_.CreateState<Player_Yuki>
			("Action_Q", this, &Player_Yuki::Action_Q_Start, &Player_Yuki::Action_Q_Update, &Player_Yuki::Action_Q_End);
		ActionState_.CreateState<Player_Yuki>
			("Action_W", this, &Player_Yuki::Action_W_Start, &Player_Yuki::Action_W_Update, &Player_Yuki::Action_W_End);
		ActionState_.CreateState<Player_Yuki>
			("Action_E", this, &Player_Yuki::Action_E_Start, &Player_Yuki::Action_E_Update, &Player_Yuki::Action_E_End);
		ActionState_.CreateState<Player_Yuki>
			("Action_R", this, &Player_Yuki::Action_R_Start, &Player_Yuki::Action_R_Update, &Player_Yuki::Action_R_End);
		ActionState_.CreateState<Player_Yuki>
			("Action_D", this, &Player_Yuki::Action_D_Start, &Player_Yuki::Action_D_Update, &Player_Yuki::Action_D_End);
	}

	{
		// 이 밑부분은 나중에 플레이어.h 로 이동하게 될 것이다.
		OrderState_.CreateState<Player_Yuki>
			("Order_Q", this, &Player_Yuki::Order_Q_Start, &Player_Yuki::Order_Q_Update, &Player_Yuki::Order_Q_End);

		OrderState_.CreateState<Player_Yuki>
			("Order_W", this, &Player_Yuki::Order_W_Start, &Player_Yuki::Order_W_Update, &Player_Yuki::Order_W_End);

		OrderState_.CreateState<Player_Yuki>
			("Order_E", this, &Player_Yuki::Order_E_Start, &Player_Yuki::Order_E_Update, &Player_Yuki::Order_E_End);

		OrderState_.CreateState<Player_Yuki>
			("Order_R", this, &Player_Yuki::Order_R_Start, &Player_Yuki::Order_R_Update, &Player_Yuki::Order_R_Start);

		OrderState_.CreateState<Player_Yuki>
			("Order_D", this, &Player_Yuki::Order_D_Start, &Player_Yuki::Order_D_Update, &Player_Yuki::Order_D_Start);
	}

	{
		Unit_Status_Base_.Stat_AttackPower_ = 0;
		Unit_Status_Base_.Stat_Health_ = 0;
		Unit_Status_Base_.Stat_AttackDist_ = 0.f;
		Unit_Status_Base_.Stat_HealthRecovery_ = 0.f;
		Unit_Status_Base_.Stat_Stamina_ = 0;
		Unit_Status_Base_.Stat_StaminaRecovery_ = 0.f;
		Unit_Status_Base_.Stat_Defense_ = 0.f;
		Unit_Status_Base_.Stat_AttackSpeed_ = 0.f;
		Unit_Status_Base_.Stat_CriticalHit_ = 0.f;
		Unit_Status_Base_.Stat_MoveSpeed_ = 0.f;
		Unit_Status_Base_.Stat_Eyesight_ = 0.f;
	}
}

void Player_Yuki::Update(float _DeltaTime)
{
	Unit::Update(_DeltaTime);

	DEBUGUpdate(_DeltaTime);

	Unit_UpdateBuff(_DeltaTime);

	OrderState_.Update(_DeltaTime);
	ActionState_.Update(_DeltaTime);
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

	{
		UnitGroundCollision_ = CreateTransformComponent<GameEngineCollision>();

		UnitSightCollision_ = CreateTransformComponent<GameEngineCollision>();

		UnitHitBoxCollision_ = CreateTransformComponent<GameEngineCollision>();
	}

	FBXRenderer_->CreateFBXAnimation("TestRun", "ALS_N_RUN_F.FBX");// 달리기
	FBXRenderer_->ChangeFBXAnimation("TestRun");

	UnitGroundCollision_->GetTransform()->SetLocalScaling(float4{ 100.f,200.f,100.f ,1.f });

	UnitHitBoxCollision_->GetTransform()->SetLocalScaling(float4{ 100.f,200.f,100.f ,1.f });

	UnitHitBoxCollision_->GetTransform()->SetLocalPosition({ 0.f,100.f,0.f });

	UnitSightCollision_->GetTransform()->SetLocalScaling(float4{ 0.f,Unit_Status_Base_.Stat_Eyesight_,0.f });

	UnitGroundCollision_->SetCollisionInfo(static_cast<int>(CollisionGroup::PlayerGround), CollisionType::AABBBox3D);
	UnitSightCollision_->SetCollisionInfo(static_cast<int>(CollisionGroup::PlayerSight), CollisionType::CirCle);
	UnitHitBoxCollision_->SetCollisionInfo(static_cast<int>(CollisionGroup::Player), CollisionType::AABBBox3D);
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
}

void Player_Yuki::DEBUGUpdate(float _DeltaTime)
{
	GetLevel()->PushDebugRender(UnitHitBoxCollision_->GetTransform(), CollisionType::AABBBox3D);
	GetLevel()->PushDebugRender(UnitSightCollision_->GetTransform(), CollisionType::CirCle);
}
