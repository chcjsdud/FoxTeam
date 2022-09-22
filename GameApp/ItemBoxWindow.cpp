#include "PreCompile.h"
#include "ItemBoxWindow.h"
#include "ItemBoxManager.h"
#include "ItemBase.h"
#include "ItemBox.h"
#include "UI_ItemBox.h"
#include <GameEngine/GameEngineCore.h>

ItemBoxWindow::ItemBoxWindow()
	: ItemBoxManager_(nullptr)
	, SelectBox_(nullptr)
	, ListSelectItem(-1)
	, ListSelectItemBox(-1)
{
	//CreateAllItemList();
}

ItemBoxWindow::~ItemBoxWindow()
{

}

void ItemBoxWindow::OnGUI()
{
	if (true == ImGui::Button("UserSave"))
	{
		ItemBoxManager_->UserSave_ItemListInfo();
	}
	ImGui::SameLine();
	if (true == ImGui::Button("UserLoad"))
	{
		ItemBoxManager_->UserLoad_ItemListInfo();
	}

	SelectBox_ = ItemBoxManager_->GetSelectBox();

	//이건호 : UI를 매니져와 윈도우가 같이 공유하게 세팅
	BoxUI_ = ItemBoxManager_->GetItemBoxUI();

	if (nullptr != SelectBox_)
	{
		const char* Arr = SelectBox_->area.c_str();
		ImGui::Text(Arr);
		ImGui::SameLine();
		ImGui::Text(std::to_string(SelectBox_->info.Index).c_str());

		Arr = SelectBox_->info.BoxType.c_str();
		ImGui::Text(Arr);

		std::vector<const char*> ItemListName;

		for (auto& Item : SelectBox_->itemList)
		{
			ItemListName.push_back(GameEngineString::AnsiToUTF8Return(Item->GetName()).c_str());
		}

		//if (true == ImGui::Button("PushItem") &&
		//	-1 != ListSelectItem)
		//{
		//	PushItem();
		//}
		//ImGui::SameLine();
		//if (true == ImGui::Button("DeleteItem") &&
		//	-1 != ListSelectItemBox)
		//{
		//	DeleteItem();
		//}

		//ImGui::PushItemWidth(200);
		//ImGui::ListBox("##ItemList", &ListSelectItem, &AllItemName[0], static_cast<ImGuiID>(AllItemName.size()));
	
		//if (false == ItemListName.empty())
		//{
		//	ImGui::SameLine();
		//	ImGui::PushItemWidth(200);
		//	ImGui::ListBox("##ItemBoxList", &ListSelectItemBox, &ItemListName[0], static_cast<ImGuiID>(ItemListName.size()));
		//}
		//
	}

}
//
//void ItemBoxWindow::PushItem()
//{
//	// ItemBase* NewItem = GameEngineCore::CurrentLevel()->CreateActor<ItemBase>();
//	// NewItem->SetName(AllItemName[ListSelectItem]);
//
//	int index = 0;
//
//	std::list<ItemBase*>::iterator iter = AllItemList.begin();
//
//	for (; iter != AllItemList.end(); iter++)
//	{
//		if (index == ListSelectItem)
//		{
//			SelectBox_->itemList.push_back((*iter)->Copy());
//
//			//이건호 : UI에 아이템정보를 0번 슬롯에 넣도록 세팅
//			BoxUI_->PushItem(*iter, 0);
//			break;
//		}
//
//		index++;
//	}
//}
//
//void ItemBoxWindow::DeleteItem()
//{
//	int index = 0;
//
//	std::list<ItemBase*>::iterator iter = SelectBox_->itemList.begin();
//
//	for (; iter != SelectBox_->itemList.end(); iter++)
//	{
//		if (index == ListSelectItemBox)
//		{
//			(*iter)->Death();
//			SelectBox_->itemList.erase(iter);
//			ListSelectItemBox = -1;
//			break;
//		}
//		index++;
//	}
//}
