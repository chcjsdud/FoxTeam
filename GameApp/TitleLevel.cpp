#include "Precompile.h"
#include "TitleLevel.h"

#include "TitleIDInput.h"
#include "UI_TestMouse.h"

TitleLevel::TitleLevel()
{
}

TitleLevel::~TitleLevel()
{
}

void TitleLevel::LevelStart()
{
}

void TitleLevel::LevelUpdate(float _DeltaTime)
{
}

void TitleLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
}

void TitleLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	GameEngineFontManager::GetInst().Load("±¼¸²");

	idInput_ = CreateActor<TitleIDInput>();

	UI_TestMouse* Mouse = CreateActor<UI_TestMouse>();


}
