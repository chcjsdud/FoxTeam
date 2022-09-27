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

void ItemBox::Start()
{
}

void ItemBox::Update(float _DeltaTime)
{
}

