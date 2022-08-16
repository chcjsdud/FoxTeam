#include "PreCompile.h"
#include "ItemBoxWindow.h"
#include "ItemBoxManager.h"

ItemBoxWindow::ItemBoxWindow()
	: ItemBoxManager_(nullptr)
	, SelectBox_(nullptr)
	, ListSelectItem(-1)
{

}

ItemBoxWindow::~ItemBoxWindow()
{

}

void ItemBoxWindow::OnGUI()
{
	SelectBox_ = ItemBoxManager_->GetSelectBox();

	std::vector<const char*> vecArr;
	
	vecArr.push_back("Test1");
	vecArr.push_back("Test2");
	vecArr.push_back("Test3");

	if (nullptr != SelectBox_)
	{
		const char* Arr = SelectBox_->Area.c_str();
		ImGui::Text(Arr);
		ImGui::SameLine();
		ImGui::Text(std::to_string(SelectBox_->Info.Index).c_str());

		Arr = SelectBox_->Info.BoxType.c_str();
		ImGui::Text(Arr);

		ImGui::PushItemWidth(200);
		ImGui::ListBox("##ItemList", &ListSelectItem, &vecArr[0], static_cast<ImGuiID>(vecArr.size()));
		ImGui::SameLine();
		ImGui::PushItemWidth(200);
		ImGui::ListBox("##ItemBoxList", &ListSelectItem, &vecArr[0], static_cast<ImGuiID>(vecArr.size()));

		ImGui::Button("PushItem");
		ImGui::SameLine();
		ImGui::Button("DeleteItem");
	}

}

