#include "PreCompile.h"
#include "ItemBox.h"
#include "ItemBase.h"
#include "UI_ItemBox.h"

ItemBox::ItemBox()
	: col(nullptr)
	, isGatherBox(false)
	, itemBoxUI_(nullptr)
	, bOpen_(false)
{

}

ItemBox::~ItemBox()
{

}

ItemBase* ItemBox::GetItem(int _index)
{
	int findIndex = 0;

	for (auto& item : itemList)
	{
		if (findIndex == _index)
		{
			return item->Copy();
		}

		++findIndex;
	}

	return nullptr;
}

bool ItemBox::DeleteItem(int _index)
{

	int findIndex = 0;
	std::list<ItemBase*>::iterator iter = itemList.begin();

	for (; iter != itemList.end(); ++iter)
	{
		if (findIndex == _index)
		{
			(*iter)->Release();
			itemList.erase(iter);
			break;
		}

		++findIndex;
	}

	return true;
}

void ItemBox::Open()
{
	if (true == bOpen_)
	{
		return;
	}

	if (itemBoxUI_ != nullptr)
	{
		itemBoxUI_->Release();
		itemBoxUI_ = nullptr;
	}
	itemBoxUI_ = GetLevel()->CreateActor<UI_ItemBox>();
	itemBoxUI_->RenderOn();
	itemBoxUI_->GetItemBoxInfo(this);

	bOpen_ = true;
}

void ItemBox::Close()
{
	if (false == bOpen_)
	{
		return;
	}

	if (itemBoxUI_ != nullptr)
	{
		itemBoxUI_->Release();
		itemBoxUI_ = nullptr;
	}

	bOpen_ = false;
}

void ItemBox::Start()
{
}

void ItemBox::Update(float _DeltaTime)
{
}

