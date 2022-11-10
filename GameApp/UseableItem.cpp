#include "PreCompile.h"
#include "UseableItem.h"

UseableItem::UseableItem()
	: Type(UseableItemType::HP)
	, HPRegen_(300.0f)
	, SPRegen_(0.0f)
	, RegenTime_(15.0f)
{

}

UseableItem::~UseableItem()
{

}

void UseableItem::Start()
{
	ItemBase::Start();
}

void UseableItem::Update(float _DeltaTime)
{
	ItemBase::Update(_DeltaTime);
}

