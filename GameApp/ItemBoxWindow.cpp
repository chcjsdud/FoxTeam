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
	CreateAllItemList();
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

	if (nullptr != SelectBox_)
	{
		const char* Arr = SelectBox_->Area.c_str();
		ImGui::Text(Arr);
		ImGui::SameLine();
		ImGui::Text(std::to_string(SelectBox_->Info.Index).c_str());

		Arr = SelectBox_->Info.BoxType.c_str();
		ImGui::Text(Arr);

		std::vector<const char*> ItemListName;

		for (auto& Item : SelectBox_->ItemList)
		{
			ItemListName.push_back(GameEngineString::AnsiToUTF8Return(Item->GetName()).c_str());
		}

		if (true == ImGui::Button("PushItem") &&
			-1 != ListSelectItem)
		{
			PushItem();
		}
		ImGui::SameLine();
		if (true == ImGui::Button("DeleteItem") &&
			-1 != ListSelectItemBox)
		{
			DeleteItem();
		}

		ImGui::PushItemWidth(200);
		ImGui::ListBox("##ItemList", &ListSelectItem, &AllItemName[0], static_cast<ImGuiID>(AllItemName.size()));
	
		if (false == ItemListName.empty())
		{
			ImGui::SameLine();
			ImGui::PushItemWidth(200);
			ImGui::ListBox("##ItemBoxList", &ListSelectItemBox, &ItemListName[0], static_cast<ImGuiID>(ItemListName.size()));
		}
		
	}

}

void ItemBoxWindow::PushItem()
{
	// ItemBase* NewItem = GameEngineCore::CurrentLevel()->CreateActor<ItemBase>();
	// NewItem->SetName(AllItemName[ListSelectItem]);

	int index = 0;

	std::list<ItemBase*>::iterator iter = AllItemList.begin();

	for (; iter != AllItemList.end(); iter++)
	{
		if (index == ListSelectItem)
		{
			SelectBox_->ItemList.push_back((*iter)->Copy());
			break;
		}

		index++;
	}
}

void ItemBoxWindow::DeleteItem()
{
	int index = 0;

	std::list<ItemBase*>::iterator iter = SelectBox_->ItemList.begin();

	for (; iter != SelectBox_->ItemList.end(); iter++)
	{
		if (index == ListSelectItemBox)
		{
			(*iter)->Death();
			SelectBox_->ItemList.erase(iter);
			ListSelectItemBox = -1;
			break;
		}
		index++;
	}
}
