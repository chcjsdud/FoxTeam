#include "PreCompile.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include "YSJ_Player.h"

YSJ_Player* YSJ_Player::MainPlayer;

YSJ_Player::YSJ_Player()
	: Speed(100.0f)
	, Color_(float4::RED)
	, PlayerCollision(nullptr)
{
	MainPlayer = this;
}

YSJ_Player::~YSJ_Player()
{
}

void YSJ_Player::Start()
{
	NaviActor::Start();
	// GetTransform()->SetWorldPosition({200.0f, 0.0f});

	{
		GameEngineRenderer* ParentRenderer = nullptr;

		{
			GameEngineRenderer* Renderer = CreateTransformComponent<GameEngineRenderer>(GetTransform());
			Renderer->SetRenderingPipeLine("Color");
			Renderer->SetMesh("Sphere");

			Renderer->GetTransform()->SetLocalScaling({ 10.0f, 10.0f, 10.0f });
			Renderer->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
			Renderer->ShaderHelper.SettingConstantBufferLink("ResultColor", Color_);
			ParentRenderer = Renderer;
		}

		//GetLevel()->GetMainCameraActor()->GetTransform()->AttachTransform(GetTransform());
		GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalPosition({ 10, 10.0f, -20.0f });
		GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalRotationDegree({ 15, 0.0f, 0.0f });
	}

	GetLevel()->GetMainCameraActor()->FreeCameraModeSwitch();
}

void YSJ_Player::Update(float _DeltaTime)
{
	NaviActor::Update(_DeltaTime);

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

	if (true == GameEngineInput::GetInst().Press("MoveUp"))
	{
		MoveDir += GetTransform()->GetWorldUpVector();
	}
	if (true == GameEngineInput::GetInst().Press("MoveDown"))
	{
		MoveDir += GetTransform()->GetWorldDownVector();
	}

	MoveDir.Normalize3D();

	GetTransform()->SetWorldDeltaTimeMove(MoveDir * Speed);
}

void YSJ_Player::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
	if (std::string::npos != _NextLevel->GetName().find("Title"))
	{
		return;
	}

	GetLevel()->SetLevelActorMove(_NextLevel, this);
	_NextLevel->GetMainCameraActor()->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + float4(0.0f, 0.0F, -100.0F));

}