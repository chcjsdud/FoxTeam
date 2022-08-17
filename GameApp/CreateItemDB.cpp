#include "PreCompile.h"
#include "ItemBoxWindow.h"
#include "ItemBoxManager.h"
#include "ItemBase.h"
#include <GameEngine/GameEngineCore.h>

// Item List를 작성, 유저 세이브 그 이후에는 주석처리
// 리스트정보를 유저로드로 불러옴

void ItemBoxWindow::CreateAllItemList()
{
	{
		ItemBase* NewItem = GameEngineCore::CurrentLevel()->CreateActor<ItemBase>();
		NewItem->SetName("Item1");
		NewItem->SetItemType(ItemType::Equipment);
		// NewItem->SetImage();

		AllItemList.push_back(NewItem);
	}
}