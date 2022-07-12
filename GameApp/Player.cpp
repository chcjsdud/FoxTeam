#include "PreCompile.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include "Player.h"

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
	// GetTransform()->SetWorldPosition({200.0f, 0.0f});

	{
		GameEngineRenderer* Renderer = CreateTransformComponent<GameEngineRenderer>(nullptr);
		Renderer->SetRenderingPipeLine("Color");
		Renderer->SetMesh("Box");

		Renderer->GetTransform()->SetLocalScaling({ 1000.0f, 10.0f, 1000.0f });
		Renderer->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
		Renderer->ShaderHelper.SettingConstantBufferSet("ResultColor", float4(0.0f, 0.2f, 0.0f));
	}

	{
		GameEngineRenderer* ParentRenderer = nullptr;

		{
			GameEngineRenderer* Renderer = CreateTransformComponent<GameEngineRenderer>(GetTransform());
			Renderer->SetRenderingPipeLine("Color");
			Renderer->SetMesh("Sphere");

			Renderer->GetTransform()->SetLocalScaling({ 10.0f, 10.0f, 10.0f });
			Renderer->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
			Renderer->ShaderHelper.SettingConstantBufferSet("ResultColor", float4(1.0f, 0.0f, 0.0f));
			ParentRenderer = Renderer;
		}

		GetLevel()->GetMainCameraActor()->GetTransform()->AttachTransform(GetTransform());
		GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalPosition({ 10, 10.0f, -20.0f });
		GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalRotationDegree({ 15, 0.0f, 0.0f });
	}

	if (false == GameEngineInput::GetInst().IsKey("PlayerMove"))
	{
		GameEngineInput::GetInst().CreateKey("MoveLeft", 'A');
		GameEngineInput::GetInst().CreateKey("MoveRight", 'D');
		GameEngineInput::GetInst().CreateKey("MoveForward", 'W');
		GameEngineInput::GetInst().CreateKey("MoveBack", 'S');
		GameEngineInput::GetInst().CreateKey("MoveUp", 'Q');
		GameEngineInput::GetInst().CreateKey("MoveDown", 'E');
		GameEngineInput::GetInst().CreateKey("FreeCameraOn", 'o');
	}

	GetLevel()->GetMainCameraActor()->FreeCameraModeSwitch();
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

	float4 MoveDir = float4::ZERO;

	if (true == GameEngineInput::GetInst().Press("MoveLeft"))
	{
		MoveDir += GetTransform()->GetWorldLeftVector();
	}
	if (true == GameEngineInput::GetInst().Press("MoveRight"))
	{
		MoveDir += GetTransform()->GetWorldRightVector();
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