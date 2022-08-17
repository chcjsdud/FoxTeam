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
	: FBXRenderer_(nullptr)
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
	DEBUGUpdate(_DeltaTime);

	UpdateBuff(_DeltaTime);

	//EnemyInSight_ = UnitSightCollision_->Collision(static_cast<int>(CollisionGroup::Monster));

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

	UnitHitBoxCollision_->GetTransform()->SetLocalScaling(float4{ 100.f,200.f,100.f ,1.f });
	UnitHitBoxCollision_->GetTransform()->SetLocalPosition({ 0.f,100.f,0.f });

	UnitSightCollision_->GetTransform()->SetLocalScaling(float4{ 800.f,0.f,800.f });

	UnitGroundCollision_->SetCollisionInfo(static_cast<int>(CollisionGroup::PlayerGround), CollisionType::AABBBox3D);
	UnitSightCollision_->SetCollisionInfo(static_cast<int>(CollisionGroup::PlayerSight), CollisionType::CirCle);
	UnitHitBoxCollision_->SetCollisionInfo(static_cast<int>(CollisionGroup::Player), CollisionType::AABBBox3D);
}

void Player_Yuki::StateInit()
{

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