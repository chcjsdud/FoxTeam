#include "PreCompile.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>
#include "Player.h"

#include <GameEngine/GameEngineFBXMeshManager.h>
#include <GameEngine/GameEngineFBXWindow.h>

Player::Player() 
	: PlayerCollision_(nullptr)
	, FBXRenderer_(nullptr)
	, PlayerGroundCollision_(nullptr)
	, PlayerHitBoxCollision_(nullptr)
	, Speed_(100.f)
	
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


	//GetLevel()->GetMainCameraActor()->FreeCameraModeSwitch();
}

void Player::ComponenetInit()
{
	FBXRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	FBXRenderer_->SetFBXMesh("AnimMan.FBX", "ColorAni");

	for (UINT i = 0; i < FBXRenderer_->GetRenderSetCount(); i++)
	{
		FBXRenderer_->GetRenderSet(i).ShaderHelper->SettingConstantBufferSet("ResultColor", float4::RED);
	}

	FBXRenderer_->CreateFBXAnimation("TestRun", "ALS_N_RUN_F.FBX");// ´Þ¸®±â
	FBXRenderer_->ChangeFBXAnimation("TestRun");

	PlayerGroundCollision_ = CreateTransformComponent<GameEngineCollision>();
	PlayerHitBoxCollision_ = CreateTransformComponent<GameEngineCollision>();	
}

void Player::StateInit()
{
	PlayerState_.CreateState<Player>("Idle", this, &Player::Idle_Start, &Player::Idle_Update, &Player::Idle_End);;
	PlayerState_.CreateState<Player>("Walk", this, &Player::Walk_Start, &Player::Walk_Update, &Player::Walk_End);;
	PlayerState_.CreateState<Player>("Run", this, &Player::Run_Start, &Player::Run_Update, &Player::Run_End);;
	PlayerState_.CreateState<Player>("Attack", this, &Player::Attack_Start, &Player::Attack_Update, &Player::Attack_End);;

	PlayerState_.ChangeState("Idle");
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
		//GameEngineInput::GetInst().CreateKey("FreeCameraOn", 'o');
	}
}

void Player::Update(float _DeltaTime)
{
	PlayerState_.Update(_DeltaTime);

	MoveUpdate();

	if (true == GetLevel()->GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}
	else
	{
		float4 rot = GetLevel()->GetMainCameraActor()->GetTransform()->GetLocalRotation();

		float4 CamPos = GetTransform()->GetWorldPosition();
		CamPos.y += 200.f;
		CamPos.x -= 100*sinf(rot.y * GameEngineMath::DegreeToRadian);
		CamPos.z -= 100 * cosf(rot.y * GameEngineMath::DegreeToRadian);

		GetLevel()->GetMainCameraActor()->GetTransform()->SetWorldPosition(CamPos);
		
	}
	
}


void Player::Attack()
{
}

void Player::MoveUpdate()
{
	float4 MoveDir = float4::ZERO;

	if (true == GameEngineInput::GetInst().Press("D"))
	{
		GetTransform()->AddLocalRotationDegreeY(-1.f);
		GetLevel()->GetMainCameraActor()->GetTransform()->AddLocalRotationDegreeY(-1.f);
	}
	if (true == GameEngineInput::GetInst().Press("A"))
	{
		GetTransform()->AddLocalRotationDegreeY(1.f);
		GetLevel()->GetMainCameraActor()->GetTransform()->AddLocalRotationDegreeY(1.f);
	}
	if (true == GameEngineInput::GetInst().Press("W"))
	{
		MoveDir += GetTransform()->GetWorldForwardVector();
	}
	if (true == GameEngineInput::GetInst().Press("S"))
	{
		MoveDir += GetTransform()->GetWorldBackVector();
	}

	MoveDir.Normalize3D();

	GetTransform()->SetWorldDeltaTimeMove(MoveDir * Speed_);
}
