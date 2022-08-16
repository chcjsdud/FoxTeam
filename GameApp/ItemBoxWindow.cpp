#include "PreCompile.h"
#include "ItemBoxWindow.h"
#include "ItemBoxManager.h"

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
		const char* Arr = SelectBox_->Area.c_str();
		ImGui::Text(Arr);

		Arr = SelectBox_->Info.Name.c_str();
		ImGui::Text(Arr);

		//int index = SelectBox_->Info.Index;
		ImGui::Text(std::to_string( SelectBox_->Info.Index).c_str());
	}
}

