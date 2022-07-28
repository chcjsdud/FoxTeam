#include "PreCompile.h"
#include "SJH_Mouse.h"

#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "Enums.h"

bool SJH_Mouse::IsPicked()
{
	float4 MouseClickPos = float4::ZERO;

	// 







	return false;
}

void SJH_Mouse::Start()
{
	Renderer_ = CreateTransformComponent<GameEngineUIRenderer>();
	Renderer_->SetImage(GameEngineString::toupper("Cursor_01.png"));
	Renderer_->GetTransform()->SetLocalPosition({ 12.0f, -10.0f, 0.0f });
	Renderer_->GetTransform()->SetLocalScaling({ 30.0f, 30.0f, 30.0f });

	Collider_ = CreateTransformComponent<GameEngineCollision>();
	Collider_->GetTransform()->SetLocalScaling(float4{ 30.0f, 30.0f, 30.0f });
	Collider_->SetCollisionInfo(static_cast<int>(CollisionGroup::MousePointer), CollisionType::AABBBox3D);
}

void SJH_Mouse::Update(float _DeltaTime)
{
	// ���콺 ��ġ����
	GetTransform()->SetWorldPosition(GameEngineInput::GetInst().GetMouse3DPos());

	// Űüũ
	if (true == GameEngineInput::GetInst().Down("LBUTTON"))
	{
		// �׽�Ʈ : Yuki�� �ش���ǥ�� �̵�




	}

	if (true == GameEngineInput::GetInst().Down("RBUTTON"))
	{

	}
}

SJH_Mouse::SJH_Mouse()
	: Renderer_(nullptr)
	, Collider_(nullptr)
	, Target_(nullptr)
	, MouseClickPos_(float4::ZERO)
{
}

SJH_Mouse::~SJH_Mouse()
{
}
