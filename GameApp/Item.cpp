#include "PreCompile.h"
#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "Item.h"

#include "Enums.h"

Item::Item() 
	: UseCount_(-1)
	, ItemUIRenderer_(nullptr)
	, ItemFBXRenderer_(nullptr)
	, ItemCollision_(nullptr)
	, Player_(nullptr)
	, ItemType_(ItemType::None)
	, Equip_(nullptr)
	, Equip_Update_(nullptr)
	, Use_(nullptr)
{

}

Item::~Item() // default destructer 디폴트 소멸자
{

}

void Item::Start()
{
	ItemCollision_ = CreateTransformComponent<GameEngineCollision>();
	ItemCollision_->SetCollisionInfo(CINT(CollisionGroup::Item), CollisionType::AABBBox3D);
}

void Item::Update(float _Time)
{
	//Item_Update(_Time);
}

void Item::Item_EquipStart()
{
	if (nullptr != Equip_)
	{
		Equip_();
	}
}

void Item::Item_Use()
{
	if (nullptr != Use_)
	{
		Use_();
	}
}

void Item::Item_Update(float _Time)
{
	if (nullptr != Equip_Update_)
	{
		Equip_Update_(_Time);
	}
}


