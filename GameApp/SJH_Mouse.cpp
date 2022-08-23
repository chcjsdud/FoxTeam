#include "PreCompile.h"
#include "SJH_Mouse.h"

#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "Enums.h"
#include "SJH_Ray.h"
#include "SJH_Yuki.h"

#include "SJH_FloorMap.h"
#include "SJH_NaviCell.h"

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
	if (true == GameEngineInput::GetInst().Down("LBUTTON"))
	{
		// 단, 화면범위를 벗어나면 리턴
		if (true == GameEngineWindow::GetInst().IsWindowRangeOut(GameEngineInput::GetInst().GetMousePos()))
		{
			return;
		}

		float4 MovePos = float4::ZERO;
		if (nullptr != SJH_FloorMap::FloorMap)
		{
			SJH_NaviCell* PickedNaviCell = Ray_->IsPickedCell(SJH_FloorMap::FloorMap->GetFloorMapRenderer(), GameEngineInput::GetInst().GetMousePos(), MovePos);
			if (nullptr != PickedNaviCell)
			{
				SJH_Yuki::MainPlayer->MoveStart(PickedNaviCell, MovePos);
			}
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
