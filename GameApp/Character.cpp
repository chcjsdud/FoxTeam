#include "PreCompile.h"

#include <GameEngine/GameEngineCollision.h>

#include "Character.h"
#include "LumiaLevel.h"
#include "MousePointer.h"
#include "LumiaMap.h"
#include "ItemBox.h"

Character::Character()
	: collision_(nullptr)
	, currentNavFace_(nullptr)
	, currentNavMesh_(nullptr)
	, manager_(nullptr)
	, currentMap_(nullptr)
	, mouse_(nullptr)
{
}

Character::~Character()
{

}

void Character::SetNaviMesh(NavMesh* _NaviMesh)
{
	currentNavMesh_ = _NaviMesh;
	currentNavFace_ = currentNavMesh_->CurrentCheck(GetTransform(), float4::DOWN);
}

void Character::Start()
{
	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->GetTransform()->SetLocalScaling(150.0f);
	collision_->SetCollisionGroup(CollisionGroup::Player);
	collision_->SetCollisionType(CollisionType::AABBBox3D);

	manager_ = GetLevelConvert<LumiaLevel>()->GetItemBoxManager();


	LumiaLevel* level = dynamic_cast<LumiaLevel*>(level_);
	if (nullptr == level)
	{
		GameEngineDebug::MsgBoxError("level 변환에 실패했습니다. class GameEngineLevel to " + std::string(typeid(LumiaLevel).name()));
	}

	currentMap_ = level->GetMap();
	mouse_ = level->GetMousePointer();

	if (nullptr == currentMap_)
	{
		GameEngineDebug::MsgBoxError("level에 Map 정보가 없습니다.");
	}

	if (nullptr == mouse_)
	{
		GameEngineDebug::MsgBoxError("level에 MousePointer 정보가 없습니다.");
	}
}

void Character::Update(float _DeltaTime)
{
	if (nullptr != collision_ &&
		collision_->IsUpdate())
	{
		GetLevel()->PushDebugRender(collision_->GetTransform(), CollisionType::AABBBox3D, float4::RED);
	}

	CheckCurrentNav();

	CheckItemBox();
}

void Character::CheckCurrentNav()
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

void Character::GetItem()
{
	// 박스가 열려있는 상태에서
	// 열려있는 UI의 아이템을 클릭하는 경우
	// 그 아이템을 가지고온다.
}

void Character::CheckItemBox()
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
		// Player가 조건이 만족하는 경우 박스를 연다.
		// ItemBox UI 를 열음
		manager_->OpenItemBox();
		return;
	}

	manager_->CloseItemBox();


	// CloseItemBox
	// Player가 아이템박스로부터 멀어진 경우
	// UI가 닫힌다.
	// SelectBox도 nullptr로 초기화
	// 초기화하지 않으면 SelectBox 근처에 다가가면 UI가 계속 열리게 됨
}

