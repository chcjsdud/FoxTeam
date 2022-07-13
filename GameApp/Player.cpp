#include "PreCompile.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>
#include "Player.h"
#include "Monster.h"
#include "Bullet.h"
#include "Map.h"

Player* Player::MainPlayer;

Player::Player()
	: Speed(300.0f)
{
	MainPlayer = this;
}

Player::~Player()
{
}

void Player::Start()
{
	FBXRenderer = CreateTransformComponent<GameEngineFBXRenderer>();
	FBXRenderer->SetFBXMesh("AnimMan.FBX", "ColorAni");

	for (UINT i = 0; i < FBXRenderer->GetRenderSetCount(); i++)
	{
		FBXRenderer->GetRenderSet(i).ShaderHelper->SettingConstantBufferSet("ResultColor", float4::RED);
	}

	FBXRenderer->CreateFBXAnimation("ALS_N_RUN_F.FBX", "ALS_N_RUN_F.FBX");
	FBXRenderer->ChangeFBXAnimation("ALS_N_RUN_F.FBX");
	// GetTransform()->SetWorldPosition({200.0f, 0.0f});

	//{
	//	GameEngineRenderer* ParentRenderer = nullptr;

	//	{
	//		GameEngineRenderer* Renderer = CreateTransformComponent<GameEngineRenderer>(GetTransform());
	//		Renderer->SetRenderingPipeLine("Color");
	//		Renderer->SetMesh("Sphere");

	//		Renderer->GetTransform()->SetLocalScaling({ 10.0f, 10.0f, 10.0f });
	//		Renderer->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	//		Renderer->ShaderHelper.SettingConstantBufferSet("ResultColor", float4(1.0f, 0.0f, 0.0f));
	//		ParentRenderer = Renderer;
	//	}

	//	GetLevel()->GetMainCameraActor()->GetTransform()->AttachTransform(GetTransform());
	//	GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalPosition({ 10, 10.0f, -20.0f });
	//	GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalRotationDegree({ 15, 0.0f, 0.0f });
	//}

	if (false == GameEngineInput::GetInst().IsKey("PlayerMove"))
	{
		GameEngineInput::GetInst().CreateKey("MoveLeft", 'A');
		GameEngineInput::GetInst().CreateKey("MoveRight", 'D');
		GameEngineInput::GetInst().CreateKey("MoveForward", 'W');
		GameEngineInput::GetInst().CreateKey("MoveBack", 'S');
		GameEngineInput::GetInst().CreateKey("MoveUp", 'Q');
		GameEngineInput::GetInst().CreateKey("MoveDown", 'E');
		//GameEngineInput::GetInst().CreateKey("FreeCameraOn", 'o');
	}

	//GetLevel()->GetMainCameraActor()->FreeCameraModeSwitch();
}

void Player::Update(float _DeltaTime)
{
	if (true == GameEngineInput::GetInst().Down("FreeCameraOn"))
	{
		GetLevel()->GetMainCameraActor()->FreeCameraModeSwitch();
	}

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
	float4 MoveDir = float4::ZERO;

	if (true == GameEngineInput::GetInst().Press("MoveLeft"))
	{
		GetTransform()->AddLocalRotationDegreeY(-1.f);
		GetLevel()->GetMainCameraActor()->GetTransform()->AddLocalRotationDegreeY(-1.f);
	}
	if (true == GameEngineInput::GetInst().Press("MoveRight"))
	{
		GetTransform()->AddLocalRotationDegreeY(1.f);
		GetLevel()->GetMainCameraActor()->GetTransform()->AddLocalRotationDegreeY(1.f);
	}
	if (true == GameEngineInput::GetInst().Press("MoveForward"))
	{
		MoveDir += GetTransform()->GetWorldForwardVector();
	}
	if (true == GameEngineInput::GetInst().Press("MoveBack"))
	{
		MoveDir += GetTransform()->GetWorldBackVector();
	}

	MoveDir.Normalize3D();

	GetTransform()->SetWorldDeltaTimeMove(MoveDir * Speed);
}

void Player::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
	if (std::string::npos != _NextLevel->GetName().find("Title"))
	{
		return;
	}

	GetLevel()->SetLevelActorMove(_NextLevel, this);
	_NextLevel->GetMainCameraActor()->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + float4(0.0f, 0.0F, -100.0F));

}