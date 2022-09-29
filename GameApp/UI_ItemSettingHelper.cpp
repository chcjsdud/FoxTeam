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
	//�ٳ����ؼ� �ڿ� .png�� �̹��� �̸��� �ִ°�
	//UI�� �� ������ ������ �̹����� ��� ������ Getname���ϰ��� _UI.png�� �־ �̹��� �̸��� ���ϴ°ŷ� ��
	string name = _Item->GetName();
	string Ext = "_UI.png";

	return name + Ext;
}
