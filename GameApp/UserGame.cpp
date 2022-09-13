#include "PreCompile.h"
#include "UserGame.h"
#include <conio.h>

#include <GameEngineBase/GameEngineTime.h>
#include <GameEngine/GameEngineWindow.h>
#include <GameEngine/GameEngineRenderingPipeLine.h>
#include <GameEngine\GameEngineLevelControlWindow.h>
#include <GameEngine\GameEngineRenderWindow.h>

#include "MeshLoadLevel.h"
#include "LH_PlayLevel.h"

#include "SJH_PlayLevel.h"
#include "PJW_Level.h"
#include "LGH_PlayLevel.h"
#include "YSJ_PlayLevel.h"
#include "SettingItemLevel.h"
#include "ServerTestLevel.h"
#include "LobbyLevel.h"
#include "KeyboardClass.h"	
#include <GameApp/LumiaLevel.h>
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
	//GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineLevelControlWindow>("LevelControlWindow");
	//GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineRenderWindow>("RenderWindow");


	//LevelCreate<PlayLevel>("Play");
	//LevelCreate<PJW_Level>("PJW_Play"); // 220728 PJW Test 레벨
	//LevelCreate<SJH_PlayLevel>("SJH_Play");	// 220727 SJH : TEST용 레벨
	//LevelCreate<YSJ_PlayLevel>("YSJ_Play");
	//LevelCreate<SettingItemLevel>("SettingItemLevel");
	//LevelCreate<GHRayTestLevel>("RayTestLevel");
	//LevelCreate<MeshLoadLevel>("MeshLoadLevel");
	//LevelCreate<LobbyLevel>("LobbyLevel");
	
	LevelCreate<LumiaLevel>("LumiaLevel");

	LevelChange("LumiaLevel");
	//LevelChange("RayTestLevel");
	//LevelChange("SJH_Play");
	//LevelChange("Play");
	//LevelChange("SJH_Play");
	//LevelChange("PJW_Play");
	//LevelChange("YSJ_Play");
	//LevelChange("LobbyLevel");
	return;
}

void UserGame::Release()
{
}