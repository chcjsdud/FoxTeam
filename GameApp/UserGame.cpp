#include "PreCompile.h"
#include "UserGame.h"
#include <conio.h>

#include <GameEngineBase/GameEngineTime.h>
#include <GameEngine/GameEngineWindow.h>
#include <GameEngine/GameEngineRenderingPipeLine.h>
#include <GameEngine\GameEngineLevelControlWindow.h>
#include <GameEngine\GameEngineRenderWindow.h>

#include "MeshLoadLevel.h"
#include "PlayLevel.h"

#include "SJH_PlayLevel.h"

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
	//GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineLevelControlWindow>("LevelControlWindow");
	//GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineRenderWindow>("RenderWindow");


	LevelCreate<PlayLevel>("Play");
	//LevelCreate<SJH_PlayLevel>("SJH_PlayLevel");	// 220727 SJH : TEST�� ����

	
	LevelChange("Play");
	//LevelChange("SJH_PlayLevel");

	return;
}

void UserGame::Release()
{

}