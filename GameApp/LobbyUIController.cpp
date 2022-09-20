#include "PreCompile.h"
#include "LobbyUIController.h"
#include "LobbyLevel.h"

#include "LobbyBackground.h"
#include "Lobby_ButtonOne.h"
#include "Lobby_ButtonTwo.h"
#include "Lobby_PortraitBg.h"
#include "Lobby_CharFull.h"
#include "Lobby_StartButton.h"
#include "Lobby_Map.h"

#include "PlayerInfoManager.h"

JobType LobbyUIController::SelectedChar = JobType::MAX;
bool LobbyUIController::CharSelectOn = false;


LobbyUIController::LobbyUIController() // default constructer 디폴트 생성자
{

}

LobbyUIController::~LobbyUIController() // default destructer 디폴트 소멸자
{

}

LobbyUIController::LobbyUIController(LobbyUIController&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void LobbyUIController::InitUI()
{
	{
		backGroundUI_ = GetLevel()->CreateActor<LobbyBackground>();
		hostButtonUI_ = GetLevel()->CreateActor<Lobby_ButtonOne>();
		clientButtonUI_ = GetLevel()->CreateActor<Lobby_ButtonTwo>();
		charFullUI_= GetLevel()->CreateActor<Lobby_CharFull>();
		startButtonUI_ = GetLevel()->CreateActor<Lobby_StartButton>();
		mapUI_ = GetLevel()->CreateActor<Lobby_Map>();
	}

	{
		for (int x = 0; x < (int)JobType::MAX; x++)
		{
			Lobby_PortraitBg* Portrait = GetLevel()->CreateActor<Lobby_PortraitBg>();

			float4 BasicPosition = { -580.0f, 200.0f, -102.0f };
			int Vertical = (x / 4);
			int Horizonal = x - (Vertical * 4);

			Portrait->GetTransform()->SetLocalPosition(BasicPosition + float4{ float(Horizonal) * 72.0f, (float(Vertical) * -115.0f), 0.0f });
			Portrait->SetChar(static_cast<JobType>(x));

			PortraitBg.emplace_back(Portrait);
		}
	}
}

Lobby_PortraitBg* LobbyUIController::GetPortraitVector(int _index)
{
	return PortraitBg[_index];
}

void LobbyUIController::Start()
{
	InitUI();
}

void LobbyUIController::Update(float _DeltaTime)
{


	for (int i = 0; i < PlayerInfoManager::GetInstance()->GetPlayerList().size(); i++)
	{
		if (nullptr == mapUI_->GetNodeList()[i])
		{
			continue;
		}

		switch (static_cast<Location>(PlayerInfoManager::GetInstance()->GetPlayerList()[i].startPoint_))
		{
		case Location::NONE:
			mapUI_->GetNodeList()[i]->GetRenderer()->Off();
			break;
		case Location::DOCK:
			mapUI_->GetNodeList()[i]->GetRenderer()->On();
			mapUI_->GetNodeList()[i]->GetRenderer()->SetImage("Map_Harbor_Pin.png", "PointSmp");
			break;
		case Location::POND:
			mapUI_->GetNodeList()[i]->GetRenderer()->On();
			mapUI_->GetNodeList()[i]->GetRenderer()->SetImage("Map_Pond_Pin.png", "PointSmp");
			break;
		case Location::BEACH:
			mapUI_->GetNodeList()[i]->GetRenderer()->On();
			mapUI_->GetNodeList()[i]->GetRenderer()->SetImage("Map_SandyBeach_Pin.png", "PointSmp");
			break;
		case Location::UPTOWN:
			mapUI_->GetNodeList()[i]->GetRenderer()->On();
			mapUI_->GetNodeList()[i]->GetRenderer()->SetImage("Map_UpTown_Pin.png", "PointSmp");
			break;
		case Location::ALLEY:
			mapUI_->GetNodeList()[i]->GetRenderer()->On();
			mapUI_->GetNodeList()[i]->GetRenderer()->SetImage("Map_Alley_Pin.png", "PointSmp");
			break;
		case Location::HOTEL:
			mapUI_->GetNodeList()[i]->GetRenderer()->On();
			mapUI_->GetNodeList()[i]->GetRenderer()->SetImage("Map_Hotel_Pin.png", "PointSmp");
			break;
		case Location::AVENUE:
			mapUI_->GetNodeList()[i]->GetRenderer()->On();
			mapUI_->GetNodeList()[i]->GetRenderer()->SetImage("Map_DownTown_Pin.png", "PointSmp");
			break;
		case Location::HOSPITAL:
			mapUI_->GetNodeList()[i]->GetRenderer()->On();
			mapUI_->GetNodeList()[i]->GetRenderer()->SetImage("Map_Hospital_Pin.png", "PointSmp");
			break;
		case Location::TEMPLE:
			mapUI_->GetNodeList()[i]->GetRenderer()->On();
			mapUI_->GetNodeList()[i]->GetRenderer()->SetImage("Map_Temple_Pin.png", "PointSmp");
			break;
		case Location::ARCHERY_RANGE:
			mapUI_->GetNodeList()[i]->GetRenderer()->On();
			mapUI_->GetNodeList()[i]->GetRenderer()->SetImage("Map_Archery_Pin.png", "PointSmp");
			break;
		case Location::CEMETERY:
			mapUI_->GetNodeList()[i]->GetRenderer()->On();
			mapUI_->GetNodeList()[i]->GetRenderer()->SetImage("Map_Cemetery_Pin.png", "PointSmp");
			break;
		case Location::FOREST:
			mapUI_->GetNodeList()[i]->GetRenderer()->On();
			mapUI_->GetNodeList()[i]->GetRenderer()->SetImage("Map_Forest_Pin.png", "PointSmp");
			break;
		case Location::FACTORY:
			mapUI_->GetNodeList()[i]->GetRenderer()->On();
			mapUI_->GetNodeList()[i]->GetRenderer()->SetImage("Map_Factory_Pin.png", "PointSmp");
			break;
		case Location::CHAPEL:
			mapUI_->GetNodeList()[i]->GetRenderer()->On();
			mapUI_->GetNodeList()[i]->GetRenderer()->SetImage("Map_Church_Pin.png", "PointSmp");
			break;
		case Location::SCHOOL:
			mapUI_->GetNodeList()[i]->GetRenderer()->On();
			mapUI_->GetNodeList()[i]->GetRenderer()->SetImage("Map_School_Pin.png", "PointSmp");
			break;
		case Location::RESERCH_CENTRE:
			mapUI_->GetNodeList()[i]->GetRenderer()->On();
			mapUI_->GetNodeList()[i]->GetRenderer()->SetImage("Map_Laboratory_Pin.png", "PointSmp");
			break;
		case Location::ESCAPE_DOCK:
			mapUI_->GetNodeList()[i]->GetRenderer()->Off();
			break;
		case Location::MAX:
			mapUI_->GetNodeList()[i]->GetRenderer()->Off();
			break;
		default:
			break;
		}
	}
}

