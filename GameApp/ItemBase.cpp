#include "PreCompile.h"
#include "ItemBase.h"

ItemBase::ItemBase()
	: type_(ItemType::NONE)
	, tier_(ItemTier::NONE)
	, renderer_(nullptr)
	, isMyBuild_(false)
{
	
}

ItemBase::~ItemBase()
{

}

void ItemBase::SetImage(const std::string& _imageName, const std::string& _sampler)
{
	renderer_->SetImage(_imageName, _sampler);
}

ItemBase* ItemBase::Copy()
{
	ItemBase* CopyItem = GetLevel()->CreateActor<ItemBase>();
	CopyItem->SetName(GetName());
	CopyItem->type_ = type_;
	CopyItem->renderer_ = renderer_;

	return CopyItem;
}

void ItemBase::Start()
{
	renderer_ = CreateTransformComponent<GameEngineImageRenderer>();
}

void ItemBase::Update(float _DeltaTime)
{
}

