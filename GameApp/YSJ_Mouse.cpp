#include "PreCompile.h"
#include "YSJ_Mouse.h"

#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "Enums.h"
#include "YSJ_Ray.h"
#include "YSJ_Player.h"

YSJ_Mouse* YSJ_Mouse::MainMouse = nullptr;

bool AStarFunc(PathIndex _Index)
{
	return false;
}

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

void YSJ_Mouse::MeshPicking()
{
	// 단, 화면범위를 벗어나면 리턴
	if (true == GameEngineWindow::GetInst().IsWindowRangeOut(GameEngineInput::GetInst().GetMousePos()))
	{
		return;
	}

	if (nullptr == Picking_)
	{
		return;
	}

	if (false == Ray_->IsPicked(GameEngineInput::GetInst().GetMousePos(), ClickPoint_, Picking_))
	{
		return;
	}

	if (nullptr == YSJ_Player::MainPlayer)
	{
		GameEngineDebug::MsgBoxError("MainPlayer가 설정되어있지 않습니다.");
		return;
	}

	//YSJ_Player::MainPlayer->GetTransform()->SetWorldPosition(ClickPoint_);
	float4 PlayerPos = YSJ_Player::MainPlayer->GetTransform()->GetWorldPosition();

	PathIndex Start = { PlayerPos.ix(), PlayerPos.iz() };
	PathIndex End = { ClickPoint_.ix(), ClickPoint_.iz() };

	if (Start == End)
	{
		return;
	}

	AStarlist_ = AStar_.AStarFind8Way(Start, End, AStarFunc);
}

void YSJ_Mouse::PlayerMove()
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
	
	// 광선 생성
	Ray_ = CreateComponent<YSJ_Ray>();
}

void YSJ_Mouse::Update(float _DeltaTime)
{
	// 마우스 위치갱신
	GetTransform()->SetWorldPosition(GameEngineInput::GetInst().GetMouse3DPos());

	// 키체크
	if (true == GameEngineInput::GetInst().Press("LBUTTON"))
	{
		MeshPicking();
	}

	if (false == AStarlist_.empty())
	{
		PlayerMove();
	}
}
