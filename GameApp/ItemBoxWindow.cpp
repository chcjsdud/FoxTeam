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
{
}

ItemBoxWindow::~ItemBoxWindow()
{

}

void ItemBoxWindow::OnGUI()
{
	SelectBox_ = ItemBoxManager_->GetSelectBox();

	if (nullptr != SelectBox_)
	{
		const char* Arr = SelectBox_->area.c_str();
		ImGui::Text(Arr);
		ImGui::SameLine();
		ImGui::Text(std::to_string(SelectBox_->info.Index).c_str());

		Arr = SelectBox_->info.BoxType.c_str();
		ImGui::Text(Arr);

		std::vector<const char*> listName;

		for (auto& Item : SelectBox_->itemList)
		{
			//listName.push_back(GameEngineString::AnsiToUTF8Return(Item->GetName()).c_str());
			if (true == Item->IsDestroyed_)
			{
				continue;
			}

			listName.push_back(Item->GetName().c_str());
		}

		if (listName.empty())
		{
			ImGui::Text("ItemBox Empty");
		}
		else
		{
			int selectIndex = -1;
			ImGui::PushItemWidth(200);
			ImGui::ListBox("##ItemList", &selectIndex, &listName[0],
				static_cast<ImGuiID>(listName.size()));
		}
	}
}
