#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;

//

class UI_ItemSettingHelper : public GameEngineActor
{
public:
	UI_ItemSettingHelper();
	~UI_ItemSettingHelper();

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	UI_ItemSettingHelper(const UI_ItemSettingHelper& _Other) = delete;
	UI_ItemSettingHelper(UI_ItemSettingHelper&& _Other) noexcept = delete;
	UI_ItemSettingHelper& operator=(const UI_ItemSettingHelper& _Other) = delete;
	UI_ItemSettingHelper& operator=(UI_ItemSettingHelper&& _Other) noexcept = delete;
};

