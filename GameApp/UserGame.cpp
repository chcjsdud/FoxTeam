#include "PreCompile.h"
#include "UserGame.h"
#include <conio.h>

#include <GameEngineBase/GameEngineTime.h>
#include <GameEngine/GameEngineWindow.h>
#include <GameEngine/GameEngineRenderingPipeLine.h>
#include <GameEngine/GameEngineLevelControlWindow.h>
#include <GameEngine/GameEngineRenderWindow.h>

#include "MeshLoadLevel.h"
#include "SettingItemLevel.h"
#include "ServerTestLevel.h"
#include "LobbyLevel.h"
#include "KeyboardClass.h"	
#include "LumiaLevel.h"
#include "TitleLevel.h"
#include "LoadingLevel.h"

std::atomic<int> UserGame::LoadingFolder = 0;
bool UserGame::IsServer_ = false;

UserGame::UserGame()
{

}

UserGame::~UserGame()
{
	KeyboardClass::GetInst().Destroy();
}

UserGame::UserGame(UserGame&& _other) noexcept
{

}


void UserGame::Initialize()
{
	GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineLevelControlWindow>("LevelControlWindow");
	//GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineRenderWindow>("RenderWindow");

	LevelCreate<TitleLevel>("TitleLevel");
	LevelCreate<LobbyLevel>("LobbyLevel");
	LevelCreate<LoadingLevel>("LoadingLevel");
	LevelCreate<LumiaLevel>("LumiaLevel");

	LevelChange("TitleLevel");
	return;
}

void UserGame::Release()
{
}