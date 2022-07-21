#include "PreCompile.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

#include "Player.h"
#include "TopUI.h"
#include "LockOnUI.h"
#include "Inventory.h"

#include "Enums.h"

//
//Player Global Func
//

Player::Player() 
	: FBXRenderer_(nullptr)
	, PlayerGroundCollision_(nullptr)
	, PlayerHitBoxCollision_(nullptr)
	, PlayerAttackHitBoxCollision_(nullptr)
	, LockOnUI_(nullptr)
	, Speed_(100.f)
	, CurFowordDir_{0.f,0.f,1.f,0.f}
	, KeyDir_{0.f,0.f,1.f,0.f}
	, FowordDir_{0.f,0.f,1.f,0.f}
	, YRotateSpeed_(900.f)
	, IsMove_(false)
	, AttackTurm_(0.f)
	, AttackTime_(0.f)
	, AttackHitTime_(0.f)
	, AttackLevel_(0)
	, Hp_(0)
	, Stamina_(0.f)
	, AttackPower_(20)
{
}

Player::~Player() 
{
}

void Player::Start()
{
	StateInit();
	ComponenetInit();
	KeyInit();
	UIInit();

	//GetLevel()->GetMainCameraActor()->FreeCameraModeSwitch();
}

void Player::Update(float _DeltaTime)
{
	DEBUGUpdate(_DeltaTime);
	
	PlayerState_.Update(_DeltaTime);
	CameraState_.Update(_DeltaTime);
}

void Player::ComponenetInit()
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

	
	//컬리전//
	PlayerGroundCollision_ = CreateTransformComponent<GameEngineCollision>();
	PlayerHitBoxCollision_ = CreateTransformComponent<GameEngineCollision>();	
	PlayerAttackHitBoxCollision_ = CreateTransformComponent<GameEngineCollision>();
	PlayerRockOnCollision_ = CreateTransformComponent<GameEngineCollision>();

	//타격 히트 박스
	PlayerAttackHitBoxCollision_->GetTransform()->SetLocalScaling(float4{ 200.f,50.f,100.f ,1.f});
	PlayerAttackHitBoxCollision_->GetTransform()->SetLocalPosition({ 0.f,100.f,100.f });
	PlayerAttackHitBoxCollision_->SetCollisionGroup(CollisionGroup::PlayerAttack);
	PlayerAttackHitBoxCollision_->Off();

	//피격 히트박스 겸사겸사 맵 컬리전도 가능할듯
	PlayerHitBoxCollision_->GetTransform()->SetLocalScaling(float4{ 100.f,200.f,100.f ,1.f});
	PlayerHitBoxCollision_->GetTransform()->SetLocalPosition({ 0.f,100.f,0.f });
	//PlayerHitBoxCollision_->SetCollisionGroup(static_cast<int>(CollisionGroup::Player));

	PlayerHitBoxCollision_->SetCollisionInfo(CINT(CollisionGroup::Player), CollisionType::AABBBox3D);
	PlayerRockOnCollision_->GetTransform()->SetLocalScaling(float4{ 800.f,0.f,800.f });
	PlayerRockOnCollision_->SetCollisionInfo(static_cast<int>(CollisionGroup::PlayerSight), CollisionType::CirCle);
}

void Player::StateInit()
{
	//State 함수
	{
		PlayerState_.CreateState<Player>("Idle", this, &Player::Idle_Start, &Player::Idle_Update, &Player::Idle_End);;
		PlayerState_.CreateState<Player>("Walk", this, &Player::Walk_Start, &Player::Walk_Update, &Player::Walk_End);;
		PlayerState_.CreateState<Player>("Run", this, &Player::Run_Start, &Player::Run_Update, &Player::Run_End);;
		PlayerState_.CreateState<Player>("Attack", this, &Player::Attack_Start, &Player::Attack_Update, &Player::Attack_End);;

		PlayerState_.ChangeState("Idle");
	}


	//카메라 함수
	{
		CameraState_.CreateState<Player>("Up", this, nullptr, &Player::CameraUpdate_UpPosition, nullptr);
		CameraState_.CreateState<Player>("Back", this, nullptr, &Player::CameraUpdate_BackPosition, nullptr);

		CameraState_.ChangeState("Up");
	}
}

void Player::KeyInit()
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

void Player::UIInit()
{
	TopUI_ = GetLevel()->CreateActor<TopUI>();
	TopUI_->GetTransform()->SetWorldPosition(float4(0.0f, 0.0f, 0.0f, 0.f));
	TopUI_->SetPlayer(this);

	LockOnUI_ = GetLevel()->CreateActor<LockOnUI>();
	LockOnUI_->GetTransform()->SetWorldPosition(float4(0.0f, 0.0f, 0.0f));
	LockOnUI_->SetPlayer(this);
	LockOnUI_->Off();

	Inventory_ = GetLevel()->CreateActor<Inventory>();
	Inventory_->SetPlayer(this);
}

void Player::CurDirUpdate(float _DeltaTime)
{
	RockOnUpdate(_DeltaTime);
	KeyDirUpdate(_DeltaTime);
}

void Player::RockOnUpdate(float _DeltaTime)
{
	if (true == GameEngineInput::GetInst().Press("RockOn"))
	{
		LockOnUI_->On();

		GameEngineCollision* RockOnPtr = PlayerRockOnCollision_->CollisionPtr(CINT(CollisionGroup::Monster));

		//엔진 lib에서 app에서 설정해둔 값을 참고하게 할 순 없을까?


		//PlayerRockOnCollision_->Collision(CollisionType::CirCle, CollisionType::AABBBox3D, static_cast<int>(CollisionGroup::Player), std::bind(&Player::test, this));


		if (RockOnPtr != nullptr)
		{
			Target_ = RockOnPtr->GetActor();

			RockOnDirUpdate(_DeltaTime);
		}
		else
		{
			Target_ = nullptr;
		}
	}

	else
	{
		LockOnUI_->Off();
		Target_ = nullptr;
	}
}

void Player::RockOnDirUpdate(float _DeltaTime)
{
	if (nullptr != Target_)
	{
		float4 MoveDir = CalculateTargetDir(Target_->GetTransform()->GetWorldPosition());

		MoveDir.Normalize3D();
		TargetDir_ = MoveDir;
		FowordDir_ = MoveDir;
	}
}

void Player::KeyDirUpdate(float _DeltaTime)
{
	float4 MoveDir = float4::ZERO;

	bool Key = false;

	if (true == GameEngineInput::GetInst().Press("D"))
	{
		MoveDir.x += 1.f;

		//IsMove_ = true;
		Key = true;
	}
	if (true == GameEngineInput::GetInst().Press("A"))
	{
		MoveDir.x -= 1.f;

		//IsMove_ = true;
		Key = true;
	}
	if (true == GameEngineInput::GetInst().Press("W"))
	{
		MoveDir.z += 1.f;

		//IsMove_ = true;
		Key = true;
	}
	if (true == GameEngineInput::GetInst().Press("S"))
	{
		MoveDir.z -= 1.f;

		//IsMove_ = true;
		Key = true;
	}

	//if (true == IsMove_)
	if (true == Key)
	{
		MoveDir.Normalize3D();
		KeyDir_ = MoveDir;

		if (Target_ == nullptr)
		{
			FowordDir_ = KeyDir_;
		}
		//IsMove_ = false;
	}
}

void Player::MoveUpdate(float _DeltaTime)
{
	if (true == IsMove_)
	{
		GetTransform()->SetWorldMove(KeyDir_ * Speed_ * _DeltaTime);
	}
}

void Player::MoveRotateUpdate(float _DeltaTime)
{
	//if (Target_ == nullptr && IsMove_ == false)
	//{
	//	return;
	//}

	if (CurFowordDir_ == FowordDir_)
	{
		return;
	}

	float4 dir = float4::Cross3D(CurFowordDir_, FowordDir_);

	float goaldegree = GameEngineMath::UnitVectorToDegree(FowordDir_.z, FowordDir_.x);

	if (dir.y >= 0.f)
	{

		GetTransform()->AddLocalRotationDegreeY(YRotateSpeed_ * _DeltaTime);
		CurFowordDir_.RotateYDegree(YRotateSpeed_ * _DeltaTime);

		dir = float4::Cross3D(CurFowordDir_, FowordDir_);
		if (dir.y < 0.f)
		{
			GetTransform()->SetLocalRotationDegree({ 0.f,goaldegree,0.f });
			CurFowordDir_ = FowordDir_;
		}
	}

	else if (dir.y < 0.f)
	{
		GetTransform()->AddLocalRotationDegreeY(-YRotateSpeed_ * _DeltaTime);
		CurFowordDir_.RotateYDegree(-YRotateSpeed_ * _DeltaTime);

		dir = float4::Cross3D(CurFowordDir_, FowordDir_);
		if (dir.y > 0.f)
		{
			GetTransform()->SetLocalRotationDegree({ 0.f,goaldegree,0.f });
			CurFowordDir_ = FowordDir_;
		}
	}
}

void Player::CameraUpdate_BackPosition(float _DeltaTime)
{
	if (true == GetLevel()->GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}
	else
	{
		float4 rot = GetLevel()->GetMainCameraActor()->GetTransform()->GetLocalRotation();

		float4 CamPos = GetTransform()->GetWorldPosition();
		CamPos.y += 200.f;
		CamPos.x -= 100 * sinf(rot.y * GameEngineMath::DegreeToRadian);
		CamPos.z -= 100 * cosf(rot.y * GameEngineMath::DegreeToRadian);

		GetLevel()->GetMainCameraActor()->GetTransform()->SetWorldPosition(CamPos);
	}
}

void Player::CameraUpdate_UpPosition(float _DeltaTime)
{
	if (true == GetLevel()->GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}
	else
	{
		//float4 rot = GetLevel()->GetMainCameraActor()->GetTransform()->GetLocalRotation();

		float4 CamPos = GetTransform()->GetWorldPosition();
		CamPos.y += 400.f;
		CamPos.z -= 400;

		GetLevel()->GetMainCameraActor()->GetTransform()->SetWorldPosition(CamPos);
		GetLevel()->GetMainCameraActor()->GetTransform()->SetWorldRotationDegree({ 25.f,0.f,0.f });
	}
}

void Player::DEBUGUpdate(float _DeltaTime)
{
	GetLevel()->PushDebugRender(PlayerHitBoxCollision_->GetTransform(), CollisionType::AABBBox3D);
	GetLevel()->PushDebugRender(PlayerRockOnCollision_->GetTransform(), CollisionType::CirCle);

	if (PlayerAttackHitBoxCollision_->IsUpdate())
	{
		GetLevel()->PushDebugRender(PlayerAttackHitBoxCollision_->GetTransform(), CollisionType::AABBBox3D,float4::RED);
	}

	//if (PlayerHitBoxCollision_->Collision(CINT(CollisionGroup::MonsterSight)))
	//{
	//	int a = 0;
	//}
}