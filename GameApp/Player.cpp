#include "PreCompile.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

#include "Player.h"
#include "TopUI.h"

#include "Enums.h"

//
//Player Global Func
//

Player::Player() 
	: FBXRenderer_(nullptr)
	, PlayerGroundCollision_(nullptr)
	, PlayerHitBoxCollision_(nullptr)
	, PlayerAttackHitBoxCollision_(nullptr)
	, Speed_(100.f)
	, CurFowordDir_{0.f,0.f,1.f}
	, KeyDir_{0.f,0.f,1.f}
	, YRotateSpeed_(900.f)
	, IsMove_(false)
	, AttackTurm_(0.f)
	, AttackTime_(0.f)
	, AttackHitTime_(0.f)
	, AttackLevel_(0)
	, Hp_(0)
	, Stamina_(0.f)
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

	TopUI_ = GetLevel()->CreateActor<TopUI>();
	TopUI_->GetTransform()->SetWorldPosition(float4(0.0f, 0.0f, 0.0f));
	TopUI_->SetPlayer(this);

	//ChangeCamFunc(&Player::CameraUpdate_UpPosition);

	//GetLevel()->GetMainCameraActor()->FreeCameraModeSwitch();
}

void Player::Update(float _DeltaTime)
{
	DEBUGUpdate(_DeltaTime);
	
	PlayerState_.Update(_DeltaTime);
	CameraState_.Update(_DeltaTime);
	//CamFunc_(_DeltaTime);
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

	//타격 히트 박스
	PlayerAttackHitBoxCollision_->GetTransform()->SetLocalScaling(float4{ 200.f,50.f,100.f ,1.f});
	PlayerAttackHitBoxCollision_->GetTransform()->SetLocalPosition({ 0.f,100.f,100.f });
	PlayerAttackHitBoxCollision_->Off();

	//피격 히트박스 겸사겸사 맵 컬리전도 가능할듯
	PlayerHitBoxCollision_->GetTransform()->SetLocalScaling(float4{ 100.f,200.f,100.f ,1.f});
	PlayerHitBoxCollision_->GetTransform()->SetLocalPosition({ 0.f,100.f,0.f });
	//PlayerHitBoxCollision_->SetCollisionGroup(static_cast<int>(CollisionGroup::Player));

	PlayerHitBoxCollision_->SetCollisionInfo(CINT(CollisionGroup::Player), CollisionType::AABBBox3D);

	//RockOnImageRenderer_ = CreateTransformComponent<GameEngineImageRenderer>();
	//RockOnImageRenderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 1.f });
	//RockOnImageRenderer_->SetImage("TestRockOn.png");

	//RockOnImageRenderer_->GetTransform()->SetWorldPosition(Target_->GetTransform()->GetWorldPosition());
	//RockOnImageRenderer_->GetTransform()->SetLocalRotationDegree((Target_->GetTransform()->GetLocalRotation());

	//RockOnImageRenderer_->SetRenderingPipeLine("Color")
	// 렌더링 파이프 라인 설정에 문제가 있음, 블랜드인지 뎁스인지 뭔지 있던거 같은데 해결은 쉬움
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
	//GameEngineInput::GetInst().CreateKey("FreeCameraOn", 'o');
}

void Player::KeyDirUpdate(float _DeltaTime)
{
	float4 MoveDir = float4::ZERO;

	//IsMove_ = false;

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
	if (CurFowordDir_ == KeyDir_)
	{
		return;
	}

	float4 dir = float4::Cross3D(CurFowordDir_, KeyDir_);

	float goaldegree = GameEngineMath::UnitVectorToDegree(KeyDir_.z, KeyDir_.x);

	if (dir.y >= 0.f)
	{

		GetTransform()->AddLocalRotationDegreeY(YRotateSpeed_ * _DeltaTime);
		CurFowordDir_.RotateYDegree(YRotateSpeed_ * _DeltaTime);

		dir = float4::Cross3D(CurFowordDir_, KeyDir_);
		if (dir.y < 0.f)
		{
			GetTransform()->SetLocalRotationDegree({ 0.f,goaldegree,0.f });
			CurFowordDir_ = KeyDir_;
		}
	}

	else if (dir.y < 0.f)
	{
		GetTransform()->AddLocalRotationDegreeY(-YRotateSpeed_ * _DeltaTime);
		CurFowordDir_.RotateYDegree(-YRotateSpeed_ * _DeltaTime);

		dir = float4::Cross3D(CurFowordDir_, KeyDir_);
		if (dir.y > 0.f)
		{
			GetTransform()->SetLocalRotationDegree({ 0.f,goaldegree,0.f });
			CurFowordDir_ = KeyDir_;
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

	if (PlayerAttackHitBoxCollision_->IsUpdate())
	{
		GetLevel()->PushDebugRender(PlayerAttackHitBoxCollision_->GetTransform(), CollisionType::AABBBox3D,float4::RED);
	}

	//if (PlayerHitBoxCollision_->Collision(CINT(CollisionGroup::MonsterSight)))
	//{
	//	int a = 0;
	//}
}