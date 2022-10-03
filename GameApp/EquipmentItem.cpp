#include "PreCompile.h"
#include "EquipmentItem.h"

EquipmentItem::EquipmentItem()
	: equipType_(EquipmentType::NONE)
{

}

EquipmentItem::~EquipmentItem()
{

}

void EquipmentItem::Start()
{
	ItemBase::Start();
}

void EquipmentItem::Update(float _DeltaTime)
{
	ItemBase::Update(_DeltaTime);
}

