#include "PreCompile.h"
#include "YSJ_Mouse.h"

#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "Enums.h"
#include "YSJ_Ray.h"
#include "YSJ_Player.h"
#include "YSJ_LumiaMap.h"

YSJ_Mouse* YSJ_Mouse::MainMouse = nullptr;

bool AStarFunc(PathIndex _Index)
{
	float4 CheckPos = { static_cast<float>(_Index.X_), 100.0f, static_cast<float>(_Index.Y_) };

	float tmp = 0.0f;

	// 교차하지 않음, navimesh 바깥쪽이므로 가면 안됨
	if (false == YSJ_LumiaMap::MainMap->GetFBXNaviRenderer()->CheckIntersects(CheckPos, float4::DOWN, tmp))
	{
		return true;
	}

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

GameEngineCollision* YSJ_Mouse::GetPickCollision(const float4& _MousePos, int _Order)
{
	return Ray_->GetPickCollision(_MousePos, _Order);
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
	/*float4 PlayerPos = YSJ_Player::MainPlayer->GetTransform()->GetWorldPosition();

	PathIndex Start = { PlayerPos.ix(), PlayerPos.iz() };
	PathIndex End = { ClickPoint_.ix(), ClickPoint_.iz() };

	if (Start == End)
	{
		return;
	}

	AStarlist_ = AStar_.AStarFind8Way(Start, End, AStarFunc, 10000);

	int a = 0;*/
}

void YSJ_Mouse::PlayerMove(float _DeltaTime)
{	
	static float Time = 0.0f;

	Time += _DeltaTime;

	if (0.001f >= Time)
	{
		return;
	}

	PathIndex NextIndex = AStarlist_.front();

	YSJ_Player::MainPlayer->GetTransform()->SetWorldPosition({ 
		static_cast<float>(NextIndex.X_), 0.0f, static_cast<float>(NextIndex.Y_) });

	AStarlist_.pop_front();
	Time = 0.0f;
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
	if (true == GameEngineInput::GetInst().Down("LBUTTON"))
	{
		MeshPicking();
	}

	if (false == AStarlist_.empty())
	{
		PlayerMove(_DeltaTime);
	}
}
