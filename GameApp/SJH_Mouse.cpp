#include "PreCompile.h"
#include "SJH_Mouse.h"

#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "Enums.h"
#include "SJH_Ray.h"
#include "SJH_Yuki.h"

void SJH_Mouse::Start()
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

void SJH_Mouse::Update(float _DeltaTime)
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

		// ī�޶�κ��� ���콺Ŭ���������� ���ϴ� ���⺤�͸� �����ϰ� �׿� �����ϴ� �浹ü�� Ž���Ͽ�
		// �����ϴ� �浹ü �߽߰� �ش� ��ġ�� ��ȯ���ش�
		SJH_Ray* Ray =  Ray_->RayAtViewSpace(GameEngineInput::GetInst().GetMousePos());
		float4 MovePos = float4::ZERO;
		if (true == Ray->IsPicked(MovePos))
		{
			SJH_Yuki::MainPlayer->GetTransform()->SetWorldPosition(MovePos);
		}
	}

	// 
	if (true == GameEngineInput::GetInst().Down("RBUTTON"))
	{

	}
}

SJH_Mouse::SJH_Mouse()
	: Renderer_(nullptr)
	, Collider_(nullptr)
	, Ray_(nullptr)
{
}

SJH_Mouse::~SJH_Mouse()
{
}
