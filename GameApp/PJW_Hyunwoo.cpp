#include "PreCompile.h"
#include "PJW_Hyunwoo.h"

#include <GameEngine/GameEngineFBXRenderer.h>

PJW_Hyunwoo::PJW_Hyunwoo() // default constructer ����Ʈ ������
{

}

PJW_Hyunwoo::~PJW_Hyunwoo() // default destructer ����Ʈ �Ҹ���
{

}

PJW_Hyunwoo::PJW_Hyunwoo(PJW_Hyunwoo&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}

void PJW_Hyunwoo::Start()
{
	Init_FBX();
	Init_FSM();
	Init_Collision();
}

void PJW_Hyunwoo::Init_FBX()
{
	// ������
	FBXRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	FBXRenderer_->SetFBXMesh("Hyunwoo_01.FBX", "TextureDeferredLight");

	for (UINT i = 0; i < FBXRenderer_->GetRenderSetCount(); i++)
	{
		//FBXRenderer_->GetRenderSet(i).ShaderHelper->SettingTexture("Hyunw)
	}

	FBXRenderer_->CreateFBXAnimation("TestRun", "ALS_N_RUN_F.FBX");// �޸���
	FBXRenderer_->ChangeFBXAnimation("TestRun");
}

void PJW_Hyunwoo::Init_FSM()
{
	//State_.CreateState<Player>("Stand", this, &Player::Stand_Start, &Player::Stand_Update, &Player::Stand_End);;
	//State_.CreateState<Player>("Move", this, &Player::Move_Start, &Player::Move_Update, &Player::Move_End);;
	//
	//State_.ChangeState("Stand");
}

void PJW_Hyunwoo::Init_Collision()
{
	//�ø���//
	//PlayerGroundCollision_ = CreateTransformComponent<GameEngineCollision>();
	//PlayerHitBoxCollision_ = CreateTransformComponent<GameEngineCollision>();
	//PlayerAttackHitBoxCollision_ = CreateTransformComponent<GameEngineCollision>();
	//PlayerLockOnCollision_ = CreateTransformComponent<GameEngineCollision>();
	//
	////Ÿ�� ��Ʈ �ڽ�
	//PlayerAttackHitBoxCollision_->GetTransform()->SetLocalScaling(float4{ 200.f,50.f,100.f ,1.f });
	//PlayerAttackHitBoxCollision_->GetTransform()->SetLocalPosition({ 0.f,100.f,100.f });
	//PlayerAttackHitBoxCollision_->SetCollisionGroup(CollisionGroup::PlayerAttack);
	//PlayerAttackHitBoxCollision_->Off();
	//
	////�ǰ� ��Ʈ�ڽ� ����� �� �ø����� �����ҵ�
	//PlayerHitBoxCollision_->GetTransform()->SetLocalScaling(float4{ 100.f,200.f,100.f ,1.f });
	//PlayerHitBoxCollision_->GetTransform()->SetLocalPosition({ 0.f,100.f,0.f });
	////PlayerHitBoxCollision_->SetCollisionGroup(static_cast<int>(CollisionGroup::Player));
	//
	//PlayerHitBoxCollision_->SetCollisionInfo(CINT(CollisionGroup::Player), CollisionType::AABBBox3D);
	//PlayerLockOnCollision_->GetTransform()->SetLocalScaling(float4{ 800.f,0.f,800.f });
	//PlayerLockOnCollision_->SetCollisionInfo(static_cast<int>(CollisionGroup::PlayerSight), CollisionType::CirCle);
}


void PJW_Hyunwoo::Update(float _DeltaTime)
{

}