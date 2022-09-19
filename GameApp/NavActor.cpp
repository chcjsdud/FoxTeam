#include "PreCompile.h"
#include "NavActor.h"
#include "LumiaLevel.h"
#include "LumiaMap.h"
#include <GameEngine/GameEngineCollision.h>
#include "ItemBox.h"

NavActor::NavActor()
	: collision_(nullptr)
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
	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->GetTransform()->SetLocalScaling(100.0f);
	collision_->SetCollisionGroup(CollisionGroup::Player);
	collision_->SetCollisionType(CollisionType::AABBBox3D);
}

void NavActor::Update(float _DeltaTime)
{
	if (nullptr != collision_ &&
		collision_->IsUpdate())
	{
		GetLevel()->PushDebugRender(collision_->GetTransform(), CollisionType::AABBBox3D, float4::RED);
	}

	CheckCurrentNav();

	CheckItemBox();
}

void NavActor::CheckCurrentNav()
{
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
	// ItemBox UI 를 열음
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

void NavActor::CheckItemBox()
{
	// Player와 SelectBox가 서로 충돌상태인지를 체크

	GameEngineCollision* OtherCol = collision_->CollisionPtr(static_cast<int>(CollisionGroup::ItemBox));

	if (nullptr != OtherCol)
	{
		ItemBox* Box = GetLevelConvert<LumiaLevel>()->GetItemBoxManager()->GetSelectBox();

		if (nullptr == Box)
		{
			return;
		}

		if (OtherCol != Box->GetCollision())
		{
			return;
		}

		// 현재 박스를 누른 후에 커서를 SelectBox 바깥으로 옮기면 박스가 열리지 않음
		OpenItemBox();
	}
}

