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
#include <GameApp/GHRayTestLevel.h>
#include "YSJ_PlayLevel.h"
#include "SettingItemLevel.h"

std::atomic<int> UserGame::LoadingFolder = 0;


UserGame::UserGame() // default constructer ����Ʈ ������
{

}

UserGame::~UserGame() // default destructer ����Ʈ �Ҹ���
{

}

UserGame::UserGame(UserGame&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}


void UserGame::Initialize()
{
	GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineLevelControlWindow>("LevelControlWindow");
	GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineRenderWindow>("RenderWindow");


	LevelCreate<PlayLevel>("Play");
	//LevelCreate<PJW_Level>("PJW_Play"); // 220728 PJW Test ����
	//LevelCreate<SJH_PlayLevel>("SJH_Play");	// 220727 SJH : TEST�� ����
	//LevelCreate<YSJ_PlayLevel>("YSJ_Play");
	//LevelCreate<SettingItemLevel>("SettingItemLevel");
	//LevelCreate<GHRayTestLevel>("RayTestLevel");
	//LevelCreate<GHRayTestLevel>("RayTestLevel");
	
	//LevelChange("RayTestLevel");
	LevelChange("SJH_Play");
	//LevelChange("Play");
	//LevelChange("SJH_Play");
	//LevelChange("PJW_Play");
	//LevelChange("YSJ_Play");

	return;
}

void UserGame::Release()
{
}