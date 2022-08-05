#include "PreCompile.h"

#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

#include "UI_Skill.h"
#include "LH_LockOnUI.h"
#include "LH_Inventory.h"

#include "Enums.h"

#include "YSJ_Char.h"

YSJ_Char::YSJ_Char()
{
}

YSJ_Char::~YSJ_Char()
{
}

void YSJ_Char::Start()
{
	GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalRotationDegree({ 45, 0.0f, 0.0f });

	StateInit();
	ComponenetInit();
	UIInit();
}

void YSJ_Char::Update(float _DeltaTime)
{
	GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition() + float4{ 0.0f, 150.0f, -150.0f });
	DEBUGUpdate(_DeltaTime);

	State_.Update(_DeltaTime);
	//CameraState_.Update(_DeltaTime);

	if (GameEngineInput::GetInst().Press("MoveUp"))
	{
		GetTransform()->SetWorldMove(float4::UP * Status_Final_.Stat_MoveSpeed_ * _DeltaTime);
	}
	if (GameEngineInput::GetInst().Press("MoveDown"))
	{
		GetTransform()->SetWorldMove(float4::DOWN * Status_Final_.Stat_MoveSpeed_ * _DeltaTime);
	}
}


void YSJ_Char::ComponenetInit()
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
	AttackHitBoxCollision_ = CreateTransformComponent<GameEngineCollision>();
	AttackHitBoxCollision_->GetTransform()->SetLocalScaling(float4{ 200.f,50.f,100.f ,1.f });
	AttackHitBoxCollision_->GetTransform()->SetLocalPosition({ 0.f,100.f,100.f });
	AttackHitBoxCollision_->SetCollisionGroup(CollisionGroup::PlayerAttack);
	AttackHitBoxCollision_->Off();

	//피격 히트박스 겸사겸사 맵 컬리전도 가능할듯
	UnitHitBoxCollision_ = CreateTransformComponent<GameEngineCollision>();
	UnitHitBoxCollision_->GetTransform()->SetLocalScaling(float4{ 100.f,200.f,100.f ,1.f });
	UnitHitBoxCollision_->GetTransform()->SetLocalPosition({ 0.f,100.f,0.f });
	UnitHitBoxCollision_->SetCollisionInfo(CINT(CollisionGroup::Player), CollisionType::AABBBox3D);
	//PlayerHitBoxCollision_->SetCollisionGroup(static_cast<int>(CollisionGroup::Player));

	UnitSightCollision_ = CreateTransformComponent<GameEngineCollision>();
	UnitSightCollision_->GetTransform()->SetLocalScaling(float4{ 800.f,0.f,800.f });
	UnitSightCollision_->SetCollisionInfo(static_cast<int>(CollisionGroup::PlayerSight), CollisionType::CirCle);

	if (false == GameEngineInput::GetInst().IsKey("M1"))
	{
		GameEngineInput::GetInst().CreateKey("M1", VK_LBUTTON); // 마우스 왼쪽 키
		GameEngineInput::GetInst().CreateKey("M2", VK_RBUTTON); // 마우스 오른쪽 키
	}

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

	Status_Final_.Stat_MoveSpeed_ = 100.0f;
}

void YSJ_Char::StateInit()
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

void YSJ_Char::UIInit()
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

void YSJ_Char::DEBUGUpdate(float _DeltaTime)
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
