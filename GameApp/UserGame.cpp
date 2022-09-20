#include "PreCompile.h"
#include "UserGame.h"
#include <conio.h>

#include <GameEngineBase/GameEngineTime.h>
#include <GameEngine/GameEngineWindow.h>
#include <GameEngine/GameEngineRenderingPipeLine.h>
#include <GameEngine\GameEngineLevelControlWindow.h>
#include <GameEngine\GameEngineRenderWindow.h>

#include "MeshLoadLevel.h"
#include "YSJ_PlayLevel.h"
#include "SettingItemLevel.h"
#include "ServerTestLevel.h"
#include "LobbyLevel.h"
#include "KeyboardClass.h"	
#include <GameApp/LumiaLevel.h>
#include "TitleLevel.h"
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


	//LevelCreate<PlayLevel>("Play");
	//LevelCreate<SJH_PlayLevel>("SJH_Play");	// 220727 SJH : TEST�� ����
	//LevelCreate<YSJ_PlayLevel>("YSJ_Play");
	//LevelCreate<SettingItemLevel>("SettingItemLevel");
	//LevelCreate<GHRayTestLevel>("RayTestLevel");
	//LevelCreate<MeshLoadLevel>("MeshLoadLevel");
	LevelCreate<LobbyLevel>("LobbyLevel");
	LevelCreate<TitleLevel>("TitleLevel");
	LevelCreate<LumiaLevel>("LumiaLevel");

	//LevelChange("LumiaLevel");
	//LevelChange("RayTestLevel");
	//LevelChange("SJH_Play");
	//LevelChange("Play");
	//LevelChange("SJH_Play");
	//LevelChange("YSJ_Play");
	//LevelChange("LobbyLevel");
	LevelChange("TitleLevel");
	return;
}

void UserGame::Release()
{
}