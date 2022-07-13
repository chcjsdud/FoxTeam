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

	FBXRenderer_->CreateFBXAnimation("TestRun", "ALS_N_RUN_F.FBX");// 달리기
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
	CameraUpdate(_DeltaTime);
}

void Player::Attack()
{
}

void Player::CameraUpdate(float _DeltaTime)
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

void Player::MoveUpdate(float _DeltaTime)
{
	float4 MoveDir = float4::ZERO;

	if (true == GameEngineInput::GetInst().Press("D"))
	{
		GetTransform()->AddLocalRotationDegreeY(1.f);
		GetLevel()->GetMainCameraActor()->GetTransform()->AddLocalRotationDegreeY(1.f);
	}
	if (true == GameEngineInput::GetInst().Press("A"))
	{
		GetTransform()->AddLocalRotationDegreeY(-1.f);
		GetLevel()->GetMainCameraActor()->GetTransform()->AddLocalRotationDegreeY(-1.f);
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

void Player::RotateFunc(float _DeltaTime)
{
	if (CurFowordDir_ != KeyDir_)
	{
		// 1. 현재 바라보는 각도
		// 2. Key 로 인해 바라보게될 각도
		// 3. 두 각도 차를 구해 + - 를 계산해 가장 가까운 방향으로 Rotate 시킴
		// 4. 바라볼 각도와 현재 각도가 일치 (혹은 아주 미세한 차이)하게 되면 중지
		// 5. 가중치를 설정하여 빠르게 or 느리게 설정 가능
		// 6. Move 함수에서 현재 바라볼 각도 설정
		// 7. RotateFunc 에서 현재 바라보고 있는 각도 계산하기 (transform의 rotate 값 이용)
	}


}
