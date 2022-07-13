#include "PreCompile.h"
#include "UserGame.h"
#include <conio.h>

#include <GameEngineBase/GameEngineTime.h>
#include <GameEngine/GameEngineWindow.h>
#include <GameEngine/GameEngineRenderingPipeLine.h>
#include <GameEngine\GameEngineLevelControlWindow.h>
#include <GameEngine\GameEngineRenderWindow.h>

#include "TitleLevel.h"
#include "PlayLevel.h"
#include "PlayLevel2.h"

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
	//GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineLevelControlWindow>("LevelControlWindow");
	//GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineRenderWindow>("RenderWindow");


	//LevelCreate<TitleLevel>("Title");
	LevelCreate<PlayLevel>("Play");
	//LevelCreate<PlayLevel2>("Play2");
	LevelChange("Play");

	return;
}

void UserGame::Release()
{

}


//
//void UserGame::GameLoop()
//{
//	GameEngineRenderingPipeLine* Pipe = GameEngineRenderingPipeLineManager::GetInst().Find("ColorRendering");
//	{
//		Pos.x += 0.001f;
//		TransData.Position.Translation(Pos);
//		TransData.CalWorld();
//
//
//		// 랜더링 파이프라인이 하나가 돌고
//		// 이안에 세팅정보가 존재하겠죠?
//		GameEngineDevice::RenderStart();
//
//		// 매프레임마다 세팅해줘야 하는게 되죠?
//		// 
//		Pipe->Rendering();
//		GameEngineDevice::RenderEnd();
//	}
//}
//
//
