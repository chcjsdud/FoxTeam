#include "PreCompile.h"
#include "UserGame.h"
#include <conio.h>

#include <GameEngineBase/GameEngineTime.h>
#include <GameEngine/GameEngineWindow.h>
#include <GameEngine/GameEngineRenderingPipeLine.h>
#include <GameEngine\GameEngineLevelControlWindow.h>
#include <GameEngine\GameEngineRenderWindow.h>

#include "MeshLoadLevel.h"

std::atomic<int> UserGame::LoadingFolder = 0;


UserGame::UserGame() // default constructer 디폴트 생성자
{

}

UserGame::~UserGame() // default destructer 디폴트 소멸자
{

}

UserGame::UserGame(UserGame&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}


void UserGame::Initialize()
{
	GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineLevelControlWindow>("LevelControlWindow");
	GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineRenderWindow>("RenderWindow");


	LevelCreate<MeshLoadLevel>("MeshLoadLevel");
	LevelChange("MeshLoadLevel");

	return;
}

void UserGame::Release()
{

}