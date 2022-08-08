#include "PreCompile.h"
#include "YSJ_Mouse.h"

#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "Enums.h"
#include "SJH_Ray.h"
#include "YSJ_Player.h"

YSJ_Mouse* YSJ_Mouse::MainMouse = nullptr;

YSJ_Mouse::YSJ_Mouse()
	: Renderer_(nullptr)
	, Collider_(nullptr)
	, Ray_(nullptr)
	, ClickPoint_(float4::ZERO)
	, Picking_(nullptr)
{
	MainMouse = this;
}

YSJ_Mouse::~YSJ_Mouse()
{
}

void YSJ_Mouse::Start()
{
	Renderer_ = CreateTransformComponent<GameEngineUIRenderer>();
	Renderer_->SetImage(GameEngineString::toupper("Cursor_01.png"));
	Renderer_->GetTransform()->SetLocalPosition({ 12.0f, -10.0f, 0.0f });
	Renderer_->GetTransform()->SetLocalScaling({ 30.0f, 30.0f, 30.0f });

	Collider_ = CreateTransformComponent<GameEngineCollision>();
	Collider_->GetTransform()->SetLocalScaling(float4{ 30.0f, 30.0f, 30.0f });
	Collider_->SetCollisionInfo(static_cast<int>(CollisionGroup::MousePointer), CollisionType::AABBBox3D);
	
	// ���� ����
	Ray_ = GetLevel()->CreateActor<SJH_Ray>();
}

void YSJ_Mouse::Update(float _DeltaTime)
{
	// ���콺 ��ġ����
	GetTransform()->SetWorldPosition(GameEngineInput::GetInst().GetMouse3DPos());

	// Űüũ
	if (true == GameEngineInput::GetInst().Press("LBUTTON"))
	{
		// ��, ȭ������� ����� ����
		if (true == GameEngineWindow::GetInst().IsWindowRangeOut(GameEngineInput::GetInst().GetMousePos()))
		{
			return;
		}

#pragma region Mesh To Ray CrossCheck
		if (nullptr != Picking_)
		{
			if (true == Ray_->IsPicked(GameEngineInput::GetInst().GetMousePos(), ClickPoint_, Picking_))
			{
				YSJ_Player::MainPlayer->GetTransform()->SetWorldPosition(ClickPoint_);
			}
		}
#pragma endregion

	}
}
