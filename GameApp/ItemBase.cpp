#include "PreCompile.h"
#include "ItemBase.h"

ItemBase::ItemBase()
	: Type(ItemType::None)
	, Renderer(nullptr)
{

}

ItemBase::~ItemBase()
{

}

void ItemBase::Start()
{
}

void ItemBase::Update(float _DeltaTime)
{
}

