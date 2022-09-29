#include "Precompile.h"
#include "UI_ItemSettingHelper.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"

UI_ItemSettingHelper::UI_ItemSettingHelper()
{
}

UI_ItemSettingHelper::~UI_ItemSettingHelper()
{
}

string UI_ItemSettingHelper::GetItemTextureName(ItemBase* _Item)
{
	//겟네임해서 뒤에 .png로 이미지 이름을 넣는것
	//UI에 들어갈 사이즈 조정된 이미지의 경우 무조건 Getname리턴값의 _UI.png를 넣어서 이미지 이름을 정하는거로 함
	string name = _Item->GetName();
	string Ext = "_UI.png";

	return name + Ext;
}
