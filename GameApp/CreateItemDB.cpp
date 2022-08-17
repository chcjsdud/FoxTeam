#include "PreCompile.h"
#include "ItemBoxWindow.h"
#include "ItemBoxManager.h"
#include "ItemBase.h"
#include <GameEngine/GameEngineCore.h>

// Item List�� �ۼ�, ���� ���̺� �� ���Ŀ��� �ּ�ó��
// ����Ʈ������ �����ε�� �ҷ���

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