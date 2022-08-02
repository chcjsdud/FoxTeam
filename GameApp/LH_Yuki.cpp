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
	KeyInit();
	UIInit();
}

void Player_Yuki::Update(float _DeltaTime)
{
	DEBUGUpdate(_DeltaTime);

	State_.Update(_DeltaTime);
	CameraState_.Update(_DeltaTime);
}


void Player_Yuki::ComponenetInit()
{
	// ������
	FBXRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	FBXRenderer_->SetFBXMesh("AnimMan.FBX", "ColorAni");

	for (UINT i = 0; i < FBXRenderer_->GetRenderSetCount(); i++)
	{
		FBXRenderer_->GetRenderSet(i).ShaderHelper->SettingConstantBufferSet("ResultColor", float4::RED);
	}

	FBXRenderer_->CreateFBXAnimation("TestRun", "ALS_N_RUN_F.FBX");// �޸���
	FBXRenderer_->ChangeFBXAnimation("TestRun");


	//�ø���//
	PlayerGroundCollision_ = CreateTransformComponent<GameEngineCollision>();
	PlayerHitBoxCollision_ = CreateTransformComponent<GameEngineCollision>();
	PlayerAttackHitBoxCollision_ = CreateTransformComponent<GameEngineCollision>();
	PlayerLockOnCollision_ = CreateTransformComponent<GameEngineCollision>();

	//Ÿ�� ��Ʈ �ڽ�
	PlayerAttackHitBoxCollision_->GetTransform()->SetLocalScaling(float4{ 200.f,50.f,100.f ,1.f });
	PlayerAttackHitBoxCollision_->GetTransform()->SetLocalPosition({ 0.f,100.f,100.f });
	PlayerAttackHitBoxCollision_->SetCollisionGroup(CollisionGroup::PlayerAttack);
	PlayerAttackHitBoxCollision_->Off();

	//�ǰ� ��Ʈ�ڽ� ����� �� �ø����� �����ҵ�
	PlayerHitBoxCollision_->GetTransform()->SetLocalScaling(float4{ 100.f,200.f,100.f ,1.f });
	PlayerHitBoxCollision_->GetTransform()->SetLocalPosition({ 0.f,100.f,0.f });
	//PlayerHitBoxCollision_->SetCollisionGroup(static_cast<int>(CollisionGroup::Player));

	PlayerHitBoxCollision_->SetCollisionInfo(CINT(CollisionGroup::Player), CollisionType::AABBBox3D);
	PlayerLockOnCollision_->GetTransform()->SetLocalScaling(float4{ 800.f,0.f,800.f });
	PlayerLockOnCollision_->SetCollisionInfo(static_cast<int>(CollisionGroup::PlayerSight), CollisionType::CirCle);
}

void Player_Yuki::StateInit()
{
	//State �Լ�
	//{
	//	PlayerState_.CreateState<Player>("Idle", this, &Player::Idle_Start, &Player::Idle_Update, &Player::Idle_End);;
	//	PlayerState_.CreateState<Player>("Walk", this, &Player::Walk_Start, &Player::Walk_Update, &Player::Walk_End);;
	//	PlayerState_.CreateState<Player>("Run", this, &Player::Run_Start, &Player::Run_Update, &Player::Run_End);;
	//	PlayerState_.CreateState<Player>("Attack", this, &Player::Attack_Start, &Player::Attack_Update, &Player::Attack_End);;

		// ���콺 Ŀ�� �̵� ����� ������Ʈ
	State_.CreateState<Player>("Stand", this, &Player::Stand_Start, &Player::Stand_Update, &Player::Stand_End);;
	State_.CreateState<Player>("Move", this, &Player::Move_Start, &Player::Move_Update, &Player::Move_End);;

	State_.ChangeState("Stand");


	//ī�޶� �Լ�
	{
		CameraState_.CreateState<Player>("Up", this, nullptr, &Player::CameraUpdate_UpPosition, nullptr);
		CameraState_.CreateState<Player>("Back", this, nullptr, &Player::CameraUpdate_BackPosition, nullptr);
		CameraState_.CreateState<Player>("EternalReturn", this, nullptr, &Player::CameraUpdate_EternalReturn, nullptr);
		CameraState_.ChangeState("EternalReturn");
	}
}

void Player_Yuki::KeyInit()
{
	GameEngineInput::GetInst().CreateKey("W", 'W');
	GameEngineInput::GetInst().CreateKey("A", 'A');
	GameEngineInput::GetInst().CreateKey("D", 'D');
	GameEngineInput::GetInst().CreateKey("S", 'S');
	GameEngineInput::GetInst().CreateKey("Space", VK_SPACE);
	GameEngineInput::GetInst().CreateKey("MoveUp", 'Q');
	GameEngineInput::GetInst().CreateKey("MoveDown", 'E');
	GameEngineInput::GetInst().CreateKey("Attack", 'J');
	GameEngineInput::GetInst().CreateKey("Esc", VK_ESCAPE);
	GameEngineInput::GetInst().CreateKey("RockOn", VK_LSHIFT);
	//GameEngineInput::GetInst().CreateKey("FreeCameraOn", 'o');
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

void Player_Yuki::BaseInfoSetting()
{
	//// �⺻����
	//JobType JobType_;					// ����Ÿ��
	//float OffensePower_;				// ���ݷ�
	//int Health_;						// ü��
	//float HealthRecovery_;			// ü�����
	//int Stamina_;						// ���¹̳�
	//float StaminaRecovery_;			// ���¹̳� ���
	//float Defense_;					// ����
	//float AttackSpeed_;				// ���ݼӵ�
	//float CriticalHit_;				// ġ��Ÿ
	//float MoveSpeed_;					// �̵��ӵ�
	//float Eyesight_;					// �þ�

	//// �������� �������
	//float Lv_OffensePower_;			// ���ݷ�
	//int Lv_Health_;					// ü��
	//float Lv_HealthRecovery_;			// ü�����
	//int Lv_Stamina_;					// ���¹̳�
	//float Lv_StaminaRecovery_;		// ���¹̳� ���
	//float Lv_Defense_;				// ����
	//float Lv_AttackSpeed_;			// ���ݼӵ�
	//float Lv_CriticalHit_;			// ġ��Ÿ
	//float Lv_MoveSpeed_;				// �̵��ӵ�
	//float Lv_Eyesight_;				// �þ�


	int a = 0;


}

void Player_Yuki::SkillInfoSetting()
{



	int a = 0;


}

//void Yuki::Initialize()
//{
//	// �������� ����
//	BaseInfoSetting();
//
//	// ��ų���� ����
//	SkillInfoSetting();
//}
