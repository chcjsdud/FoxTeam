#include "PreCompile.h"
#include "ItemBox.h"
#include "ItemBase.h"

ItemBox::ItemBox()
	: col(nullptr)
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
			return item;
		}

		++findIndex;
	}

	return nullptr;
}

void ItemBox::Start()
{
}

void ItemBox::Update(float _DeltaTime)
{
}

