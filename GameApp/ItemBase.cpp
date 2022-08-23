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

void ItemBase::SetImage(const std::string& _ImageName, const std::string& _Sampler)
{
	Renderer->SetImage(_ImageName, _Sampler);
}

ItemBase* ItemBase::Copy()
{
	ItemBase* CopyItem = GetLevel()->CreateActor<ItemBase>();
	CopyItem->SetName(GetName());
	CopyItem->Type = Type;
	CopyItem->Renderer = Renderer;

	return CopyItem;
}

void ItemBase::Start()
{
	Renderer = CreateTransformComponent<GameEngineImageRenderer>();
}

void ItemBase::Update(float _DeltaTime)
{
}
