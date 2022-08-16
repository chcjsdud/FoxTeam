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
	}
	PlayerCollision = CreateTransformComponent<GameEngineCollision>();
	PlayerCollision->SetCollisionType(CollisionType::AABBBox3D);
	PlayerCollision->GetTransform()->SetLocalScaling({ 5.0f, 5.0f, 5.0f });
}

void YSJ_Player::Update(float _DeltaTime)
{
	NaviActor::Update(_DeltaTime);

	GetLevel()->GetMainCamera()->PushDebugRender(PlayerCollision->GetTransform(), CollisionType::AABBBox3D);

	std::list<GameEngineCollision*> Collist = PlayerCollision->CollisionPtrGroup(1);

	if (false == Collist.empty())
	{
		int a = 0;
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

	if (nullptr != GetCurrentNavi())
	{
		float Dist = GetCurrentNavi()->YCheck(GetTransform());
		GetTransform()->SetWorldMove({ 0.0f, -Dist, 0.0f });
	}
	
}

void YSJ_Player::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
}

void YSJ_Player::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{

}