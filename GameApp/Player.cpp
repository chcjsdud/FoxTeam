#include "PreCompile.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

#include "Player.h"

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
	, YRotateSpeed_(900.f)
	
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

	//ChangeCamFunc(&Player::CameraUpdate_UpPosition);

	//GetLevel()->GetMainCameraActor()->FreeCameraModeSwitch();
}

void Player::Update(float _DeltaTime)
{
	GetLevel()->PushDebugRender(PlayerHitBoxCollision_->GetTransform(), CollisionType::AABBBox3D);

	if (PlayerAttackHitBoxCollision_->IsUpdate())
	{
		GetLevel()->PushDebugRender(PlayerAttackHitBoxCollision_->GetTransform(), CollisionType::AABBBox3D);
	}
	
	if (GameEngineInput::GetInst().Press("Attack"))
	{
		PlayerState_.ChangeState("Attack");
	}

	PlayerState_.Update(_DeltaTime);
	CameraState_.Update(_DeltaTime);
	//CamFunc_(_DeltaTime);
}

void Player::ComponenetInit()
{
	FBXRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	FBXRenderer_->SetFBXMesh("AnimMan.FBX", "ColorAni");

	for (UINT i = 0; i < FBXRenderer_->GetRenderSetCount(); i++)
	{
		FBXRenderer_->GetRenderSet(i).ShaderHelper->SettingConstantBufferSet("ResultColor", float4::RED);
	}

	FBXRenderer_->CreateFBXAnimation("TestRun", "ALS_N_RUN_F.FBX");// 달리기
	FBXRenderer_->ChangeFBXAnimation("TestRun");

	PlayerGroundCollision_ = CreateTransformComponent<GameEngineCollision>();
	PlayerHitBoxCollision_ = CreateTransformComponent<GameEngineCollision>();	
	PlayerAttackHitBoxCollision_ = CreateTransformComponent<GameEngineCollision>();

	PlayerAttackHitBoxCollision_->GetTransform()->SetLocalScaling(float4{ 200.f,50.f,100.f ,1.f});
	PlayerAttackHitBoxCollision_->GetTransform()->SetLocalPosition({ 0.f,100.f,100.f });

	PlayerAttackHitBoxCollision_->Off();

	PlayerHitBoxCollision_->GetTransform()->SetLocalScaling(float4{ 100.f,200.f,100.f ,1.f});
	PlayerHitBoxCollision_->GetTransform()->SetLocalPosition({ 0.f,100.f,0.f });
}

void Player::StateInit()
{
	{
		PlayerState_.CreateState<Player>("Idle", this, &Player::Idle_Start, &Player::Idle_Update, &Player::Idle_End);;
		PlayerState_.CreateState<Player>("Walk", this, &Player::Walk_Start, &Player::Walk_Update, &Player::Walk_End);;
		PlayerState_.CreateState<Player>("Run", this, &Player::Run_Start, &Player::Run_Update, &Player::Run_End);;
		PlayerState_.CreateState<Player>("Attack", this, &Player::Attack_Start, &Player::Attack_Update, &Player::Attack_End);;

		PlayerState_.ChangeState("Idle");
	}

	{
		CameraState_.CreateState<Player>("Up", this, nullptr, &Player::CameraUpdate_UpPosition, nullptr);
		CameraState_.CreateState<Player>("Back", this, nullptr, &Player::CameraUpdate_BackPosition, nullptr);

		CameraState_.ChangeState("Up");
	}

}

void Player::KeyInit()
{
	if (false == GameEngineInput::GetInst().IsKey("PlayerMove"))
	{
		GameEngineInput::GetInst().CreateKey("W", 'W');
		GameEngineInput::GetInst().CreateKey("A", 'A');
		GameEngineInput::GetInst().CreateKey("D", 'D');
		GameEngineInput::GetInst().CreateKey("S", 'S');
		GameEngineInput::GetInst().CreateKey("Space", VK_SPACE);
		GameEngineInput::GetInst().CreateKey("MoveUp", 'Q');
		GameEngineInput::GetInst().CreateKey("MoveDown", 'E');
		GameEngineInput::GetInst().CreateKey("Attack", 'J');
		//GameEngineInput::GetInst().CreateKey("FreeCameraOn", 'o');
	}
}

void Player::Attack()
{
}

void Player::MoveUpdate(float _DeltaTime)
{
	float4 MoveDir = float4::ZERO;

	bool isMove = false;

	// 키 민감도(?) 설정을 해야함
	// 현 상황에서는 대각 방향 Idle이 안나옴
	// 대각 방향 Idle 나올 수 있게 수정하기
	// 어떤 키가 몇초 이상 눌렸는가 설정하게,
	// flaot값으로 키 민감도 설정 가능하게 만들기

	if (true == GameEngineInput::GetInst().Press("D"))
	{
		MoveDir.x += 1.f;

		isMove = true;
		//GetTransform()->AddLocalRotationDegreeY(1.f);// 곧 수정
		//GetLevel()->GetMainCameraActor()->GetTransform()->AddLocalRotationDegreeY(1.f);
	}
	if (true == GameEngineInput::GetInst().Press("A"))
	{
		MoveDir.x -= 1.f;

		isMove = true;
		//GetTransform()->AddLocalRotationDegreeY(-1.f);//
		//GetLevel()->GetMainCameraActor()->GetTransform()->AddLocalRotationDegreeY(-1.f);
	}
	if (true == GameEngineInput::GetInst().Press("W"))
	{
		MoveDir.z += 1.f;

		isMove = true;
		//MoveDir += GetTransform()->GetWorldForwardVector();
	}
	if (true == GameEngineInput::GetInst().Press("S"))
	{
		MoveDir.z -= 1.f;

		isMove = true;
		//MoveDir += GetTransform()->GetWorldBackVector();
	}

	if (true == isMove)
	{
		MoveDir.Normalize3D(); 
		KeyDir_ = MoveDir;
	}

	GetTransform()->SetWorldMove(KeyDir_ * Speed_ * _DeltaTime);
}

void Player::MoveRotateUpdate(float _DeltaTime)
{
	//완전히 서로 반대되는 방향일 경우, 외적의 결과값이 나오질 않는 문제


	float4 dir = float4::Cross3D(CurFowordDir_, KeyDir_);

	float goaldegree = UnitVectorToDegree(KeyDir_.z, KeyDir_.x);

	if (dir.y > 0.f)
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
