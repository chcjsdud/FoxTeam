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
	
	// 광선 생성
	Ray_ = GetLevel()->CreateActor<SJH_Ray>();
}

void SJH_Mouse::Update(float _DeltaTime)
{
	// 마우스 위치갱신
	GetTransform()->SetWorldPosition(GameEngineInput::GetInst().GetMouse3DPos());

	// 키체크
	if (true == GameEngineInput::GetInst().Press("LBUTTON"))
	{
		// 단, 화면범위를 벗어나면 리턴
		if (true == GameEngineWindow::GetInst().IsWindowRangeOut(GameEngineInput::GetInst().GetMousePos()))
		{
			return;
		}

		// 카메라로부터 마우스클릭지점으로 향하는 방향벡터를 생성하고 그와 교차하는 충돌체를 탐색하여
		// 교차하는 충돌체 발견시 해당 위치를 반환해준다
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
