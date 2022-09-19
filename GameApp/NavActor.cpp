#include "PreCompile.h"
#include "NavActor.h"
#include "LumiaLevel.h"
#include "LumiaMap.h"
#include <GameEngine/GameEngineCollision.h>

NavActor::NavActor()
	: Collision_(nullptr)
	, currentNavFace_(nullptr)
	, currentNavMesh_(nullptr)
{
}

NavActor::~NavActor()
{

}

void NavActor::SetNaviMesh(NavMesh* _NaviMesh)
{
	currentNavMesh_ = _NaviMesh;
	currentNavFace_ = currentNavMesh_->CurrentCheck(GetTransform(), float4::DOWN);
}

void NavActor::Start()
{
	Collision_ = CreateTransformComponent<GameEngineCollision>();
	Collision_->GetTransform()->SetLocalScaling(100.0f);
	Collision_->SetCollisionGroup(CollisionGroup::Player);
}

void NavActor::Update(float _DeltaTime)
{
	if (nullptr != Collision_ &&
		Collision_->IsUpdate())
	{
		GetLevel()->PushDebugRender(Collision_->GetTransform(), CollisionType::AABBBox3D, float4::RED);
	}

	if (nullptr != currentNavFace_)
	{
		if (true == currentNavFace_->OutCheck(GetTransform()))
		{
			currentNavFace_ = currentNavFace_->MoveCheck(GetTransform());
		}
	}
	else
	{
		if (nullptr != GetLevelConvert<LumiaLevel>()->GetMap())
		{
			SetCurrentNavi(GetLevelConvert<LumiaLevel>()->GetMap()->GetNavMesh()->CurrentCheck(GetTransform(), float4::DOWN));
		}
	}
}

void NavActor::OpenItemBox()
{
	// Player가 조건이 만족하는 경우 박스를 연다.
	// 조건 : Player와 SelectBox가 서로 충돌상태임

}

void NavActor::GetItem()
{
	// 박스가 열려있는 상태에서
	// 열려있는 UI의 아이템을 클릭하는 경우
	// 그 아이템을 가지고온다.
}

void NavActor::CloseItemBox()
{
	// Player가 다른 곳을 클릭하는 경우
	// UI가 닫힌다.
	// SelectBox도 nullptr로 초기화
	// 초기화하지 않으면 SelectBox 근처에 다가가면 UI가 계속 열리게 됨
}

