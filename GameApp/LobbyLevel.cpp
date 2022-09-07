#include "Precompile.h"
#include "LobbyLevel.h"
#include "ePacketID.h"
#include "PlayerNumberPacket.h"
#include "CharSelectPacket.h"
#include "GameJoinPacket.h"
#include "GameJoinPacket2.h"
#include "ReadyPacket.h"
#include "Enums.h"

#include "LobbyBackground.h"
#include "Lobby_ButtonOne.h"
#include "Lobby_ButtonTwo.h"
#include "Lobby_PortraitBg.h"
#include "TempLobbyRenderer.h"
#include "UI_TestMouse.h"

#include <GameEngine/GameEngineCollision.h>

#include "GameServer.h"
#include "GameClient.h"
#include "PlayerInfoManager.h"
#include "SetPlayerNumberPacket.h"

#include "LobbyUIController.h"

LobbyLevel::LobbyLevel()
	: playerCount_(0), myCharacterSelection_(-1), myStartPointSelection_(-1), myIsReady_(false)
{

}

LobbyLevel::~LobbyLevel()
{

}

void LobbyLevel::LevelStart()
{
	{
		GetMainCamera()->SetProjectionMode(ProjectionMode::Orthographic);
		//GetMainCameraActor()->GetCamera()->SetFov(60.f);
		GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.0f, 0.0f, -400.0f));
	}

	{
		state_.CreateState<LobbyLevel>("Idle", this, &LobbyLevel::StartIdle, &LobbyLevel::UpdateIdle, &LobbyLevel::EndIdle);
		// Idle : 아무 입력이 없는 대기실의 첫 상태입니다.
		// 호스트 개설 | 클라이언트로 입장이 확정된 후 캐릭터 선택 상태인 Select 로 넘어갑니다.

		state_.CreateState<LobbyLevel>("Select", this, &LobbyLevel::StartSelect, &LobbyLevel::UpdateSelect, &LobbyLevel::EndSelect);
		// Select : 게임 방에 정식으로 입장해 캐릭터와 특성, 시작 위치를 정할 수 있는 상태입니다.
		// 선택을 완료 후 좌하단의 "캐릭터 선택" 버튼을 누르면 Join 상태로 넘어갑니다.

		state_.CreateState<LobbyLevel>("Join", this, &LobbyLevel::StartJoin, &LobbyLevel::UpdateJoin, &LobbyLevel::EndJoin);
		// Join : 캐릭터 선택을 마무리하고 다른 플레이어를 기다리는 상태입니다.

		state_.ChangeState("Idle");

	}

	if (false == GameEngineInput::GetInst().IsKey("1"))
	{
		GameEngineInput::GetInst().CreateKey("1", '1');
		GameEngineInput::GetInst().CreateKey("2", '2');
		GameEngineInput::GetInst().CreateKey("3", '3');
		GameEngineInput::GetInst().CreateKey("4", '4');
		GameEngineInput::GetInst().CreateKey("5", '5');
		GameEngineInput::GetInst().CreateKey("6", '6');
		GameEngineInput::GetInst().CreateKey("Ready", VK_RETURN);
		GameEngineInput::GetInst().CreateKey("LBUTTON", VK_LBUTTON);
	}

}



void LobbyLevel::LevelUpdate(float _DeltaTime)
{
	state_.Update(_DeltaTime);

	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	int a = 0;
}

void LobbyLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{

}

void LobbyLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	if (tempLobbyRenderers_.size() == 0)
	{
		for (size_t i = 0; i < 6; i++)
		{
			TempLobbyRenderer* renderer_ = CreateActor<TempLobbyRenderer>();
			renderer_->SetPos({ (-500.f + 200.f * i), -280.f, 0.f });
			renderer_->SetScale({ 122.f, 155.f });
			renderer_->SetRender(false);
			tempLobbyRenderers_.emplace_back(renderer_);
		}
	}


	//{
	//	BackgroundRenderer = CreateActor<LobbyBackground>();
	//	ButtonLeft = CreateActor<Lobby_ButtonOne>();
	//	ButtonRight = CreateActor<Lobby_ButtonTwo>();
	//}

	//{
	//	for (int x = 0; x < (int)JobType::MAX; x++)
	//	{
	//		Lobby_PortraitBg* Portrait = CreateActor<Lobby_PortraitBg>();
	//		float4 BasicPosition = { -580.0f, 200.0f, -102.0f };
	//		
	//		int Vertical = (x/4);
	//		int Horizonal = x - (Vertical * 4);
	//		Portrait->GetTransform()->SetLocalPosition(BasicPosition + float4{ float(Horizonal) * 72.0f, (float(Vertical) * -115.0f), 0.0f});
	//		Portrait->SetChar(static_cast<JobType>(x));
	//
	//		PortraitBg.emplace_back(Portrait);
	//	}
	//}

	{
		UIController_ = CreateActor<LobbyUIController>();
	}

	{
		UI_TestMouse* Mouse = CreateActor<UI_TestMouse>();
	}

}



// FSM Method
void LobbyLevel::StartIdle()
{

}

void LobbyLevel::UpdateIdle(float _DeltaTime)
{
	GameServer* serverSocket_ = GameServer::GetInstance();
	GameClient* clientSocket_ = GameClient::GetInstance();

	// 서버가 쓰이는 업데이트는
	// 기본적으로 현 프로세스가 [호스트일 때 / 클라이언트일 때] 를 모두 상정하고, if 문으로 구분해서 코드를 짜셔야 합니다.
	// 
	if (true == GameEngineInput::Down("1"))
	{
		//  프로세스를 호스트로 지정 (= 방을 만들겠다)
		serverSocket_->Initialize();
		serverSocket_->OpenServer();
		serverSocket_->AddPacketHandler(ePacketID::PlayerNumberPacket, new PlayerNumberPacket);
		//serverSocket_->AddPacketHandler(ePacketID::GameJoinPacket, new GameJoinPacket);
		serverSocket_->AddPacketHandler(ePacketID::GameJoinPacket2, new GameJoinPacket2);
		serverSocket_->AddPacketHandler(ePacketID::SetPlayerNumberPacket, new SetPlayerNumberPacket);
		serverSocket_->AddPacketHandler(ePacketID::CharSelectPacket, new CharSelectPacket);
		serverSocket_->AddPacketHandler(ePacketID::ReadyPacket, new ReadyPacket);
		GameEngineDebug::OutPutDebugString("호스트로서 방을 만듭니다.");

		PlayerInfoManager::GetInstance()->AddNewPlayer({ 0, -1, -1, 0 });
		// AddNewPlayer() 의 파라미터는 곧 PlayerInfo 의 생성자 파라미터로,
		// 순서대로 {플레이어 번호, 캐릭터, 시작 지역, 준비 상태} 의 이니셜라이즈 값입니다.
	
		PlayerInfoManager::GetInstance()->SetPlayerNumber(0);
		// 해당 플레이어 번호는 현 프로세스의 멤버 변수로도 따로 저장합니다.

		state_.ChangeState("Select");
		return;
	}

	// 프로세스를 클라이언트로 지정 (= 방에 참여하겠다)
	if (true == GameEngineInput::Down("2"))
	{
		clientSocket_->Initialize();
		clientSocket_->Connect("127.0.0.1");
		clientSocket_->AddPacketHandler(ePacketID::PlayerNumberPacket, new PlayerNumberPacket);
		//clientSocket_->AddPacketHandler(ePacketID::GameJoinPacket, new GameJoinPacket);
		clientSocket_->AddPacketHandler(ePacketID::GameJoinPacket2, new GameJoinPacket2);
		clientSocket_->AddPacketHandler(ePacketID::SetPlayerNumberPacket, new SetPlayerNumberPacket);
		clientSocket_->AddPacketHandler(ePacketID::CharSelectPacket, new CharSelectPacket);
		clientSocket_->AddPacketHandler(ePacketID::ReadyPacket, new ReadyPacket);

		GameEngineDebug::OutPutDebugString("클라이언트로서 방에 참여합니다.");
		
		// 바로 플레이어 정보를 저장하는 호스트와는 달리, 클라이언트는 따로 요청 패킷을 보냅니다.
		// 패킷을 보내면 알맞은 플레이어 순번을 지정해 호스트가 다시 지정된 순번을 브로드캐스팅 해 줍니다.
		GameJoinPacket2 packet;
		packet.SetPlayerInfo({ -1, -1, -1, 0 });
		packet.SetListSize(PlayerInfoManager::GetInstance()->GetPlayerList().size());

		clientSocket_->Send(&packet);

		state_.ChangeState("Select");

		return;
	}
}

void LobbyLevel::EndIdle()
{


}

void LobbyLevel::StartSelect()
{
	if (true == GameClient::GetInstance()->IsConnected())
	{

	}

	if (true == GameServer::GetInstance()->IsOpened())
	{

		playerCount_ = PlayerInfoManager::GetInstance()->GetPlayerList().size();
	}
}

void LobbyLevel::UpdateSelect(float _DeltaTime)
{
	GameServer* serverSocket_ = GameServer::GetInstance();
	GameClient* clientSocket_ = GameClient::GetInstance();
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();


	// 호스트 서버일 시
	if (serverSocket_->IsOpened())
	{
		serverSocket_->ProcessPacket();

		// 캐릭터 선택시 패킷 보내는 코드블록
		for (int x = 0; x < (int)JobType::MAX; x++)
		{
			//PortraitBg[x]->GetCollision()->Collision(CollisionGroup::MousePointer)
			
			if (UIController_->GetPortraitVector(x)->GetCollision()->Collision(CollisionGroup::MousePointer))
			{
				if (GameEngineInput::GetInst().Down("LBUTTON"))
				{
					UIController_->GetPortraitVector(x)->SelectOn();
					CharSelectPacket packet;
					packet.SetTargetIndex(pm->GetMyNumber()); // 나의 플레이어 번호를 알려준다
					packet.SetCharacter(static_cast<int>(UIController_->GetPortraitVector(x)->GetChar())); // 나의 캐릭터 선택을 알려준다
					packet.SetStartPoint(static_cast<int>(Location::DOCK)); // 나의 스타팅 포인트 지역을 알려준다
					serverSocket_->Send(&packet);

					pm->GetPlayerList()[pm->GetMyNumber()].character_ = static_cast<int>(UIController_->GetPortraitVector(x)->GetChar());
					pm->GetPlayerList()[pm->GetMyNumber()].startPoint_ = static_cast<int>(Location::UPTOWN);

					for (int o = 0; o < (int)JobType::MAX; o++)
					{
						if (o == x)
						{
							continue;
						}
						else
						{
							UIController_->GetPortraitVector(o)->SelectOff();
						}

					}
				}
			}
		}

		if (true == GameEngineInput::Down("Ready"))
		{
			if (-1 == pm->GetPlayerList()[pm->GetMyNumber()].character_)
			{
				GameEngineDebug::OutPutDebugString("캐릭터를 선택해주세요!!\n");
				return;
			}

			pm->GetPlayerList()[pm->GetMyNumber()].isReady_ = true;

			ReadyPacket packet;
			packet.SetTargetIndex(pm->GetMyNumber());
			packet.SetReadyStatus(1);
			serverSocket_->Send(&packet);

			pm->GetPlayerList()[pm->GetMyNumber()].isReady_ = 1;
			state_.ChangeState("Join");
		}
	}
	else if (clientSocket_->IsConnected())	// 클라이언트일 시
	{
		clientSocket_->ProcessPacket();
		playerCount_ = static_cast<int>(pm->GetPlayerList().size());

		// 캐릭터 선택시 패킷 보내는 코드블록
		for (int x = 0; x < (int)JobType::MAX; x++)
		{
			if (UIController_->GetPortraitVector(x)->GetCollision()->Collision(CollisionGroup::MousePointer))
			{
				if (GameEngineInput::GetInst().Down("LBUTTON"))
				{
					UIController_->GetPortraitVector(x)->SelectOn();
					CharSelectPacket packet;
					packet.SetTargetIndex(pm->GetMyNumber()); // 나의 플레이어 번호를 알려준다
					packet.SetCharacter(static_cast<int>(UIController_->GetPortraitVector(x)->GetChar())); // 나의 캐릭터 선택을 알려준다
					packet.SetStartPoint(static_cast<int>(Location::DOCK)); // 나의 스타팅 포인트 지역을 알려준다
					clientSocket_->Send(&packet);

					for (int o = 0; o < (int)JobType::MAX; o++)
					{
						if (o == x)
						{
							continue;
						}
						else
						{
							UIController_->GetPortraitVector(o)->SelectOff();
						}

					}
				}
			}
		}

		



		if (true == GameEngineInput::Down("Ready"))
		{
			if (-1 == pm->GetPlayerList()[pm->GetMyNumber()].character_)
			{
				GameEngineDebug::OutPutDebugString("캐릭터를 선택해주세요!!\n");
				return;
			}

			pm->GetPlayerList()[pm->GetMyNumber()].isReady_ = true;

			ReadyPacket packet;
			packet.SetTargetIndex(pm->GetMyNumber());
			packet.SetReadyStatus(true);
			clientSocket_->Send(&packet);
			state_.ChangeState("Join");
		}

	}


	// 유저들의 달라진 캐릭터 선택을 인지해 렌더링을 바꿔 준다.
	for (int i = 0; i < pm->GetPlayerList().size(); i++)
	{
		tempLobbyRenderers_[i]->SetRender(true);
		switch (static_cast<JobType>(pm->GetPlayerList()[i].character_))
		{
		case JobType::NONE:
			break;
		case JobType::YUKI:
			break;
		case JobType::FIORA:
			break;
		case JobType::ZAHIR:
			break;
		case JobType::NADINE:
			break;
		case JobType::HYUNWOO:
			tempLobbyRenderers_[i]->SetImage("tempLobbyHyunwoo.png");
			break;
		case JobType::JACKIE:
			tempLobbyRenderers_[i]->SetImage("tempLobbyJackie.png");
			break;
		case JobType::RIO:
			tempLobbyRenderers_[i]->SetImage("tempLobbyRio.png");
			break;
		case JobType::AYA:
			tempLobbyRenderers_[i]->SetImage("tempLobbyAya.png");
			break;
		case JobType::MAX:
			break;
		default:
			break;
		}
	}
}

void LobbyLevel::EndSelect()
{
}

void LobbyLevel::StartJoin()
{
}

void LobbyLevel::UpdateJoin(float _DeltaTime)
{
	// 다른 상대의 준비를 기다리거나, 취소 버튼을 눌러 다시 캐릭터 선택으로 돌아가는 단계입니다.
	// 최종 결제권을 가진 호스트가 전체 레디 상황을 확인하고 게임을 시작합니다.

	GameServer* serverSocket_ = GameServer::GetInstance();
	GameClient* clientSocket_ = GameClient::GetInstance();
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	if (true == serverSocket_->IsOpened())
	{
		serverSocket_->ProcessPacket();
		int count = 0;

		for (int i = 0; i < pm->GetPlayerList().size(); i++)
		{
			if (true == pm->GetPlayerList()[i].isReady_)
			{
				count++;
			}
		}

		if (count == pm->GetPlayerList().size())
		{
			// 모두 캐릭터 선택을 마치면, 호스트 프로세스가 최종적으로 이 위치로 들어오게 됩니다.
			
			// 각 프로세스마다 맵과 몬스터 ,아이템을 배치하는 과정을 여기에서 명령해야 할 듯...


			GameEngineDebug::MsgBoxError("모두 준비 완료, 게임을 시작합니다.");
		}
	}
	else if (true == clientSocket_->IsConnected())
	{
		clientSocket_->ProcessPacket();
	}

	// 유저들의 달라진 캐릭터 선택을 인지해 렌더링을 바꿔 준다.
	for (int i = 0; i < pm->GetPlayerList().size(); i++)
	{
		tempLobbyRenderers_[i]->SetRender(true);
		switch (static_cast<JobType>(pm->GetPlayerList()[i].character_))
		{
		case JobType::NONE:
			break;
		case JobType::YUKI:
			break;
		case JobType::FIORA:
			break;
		case JobType::ZAHIR:
			break;
		case JobType::NADINE:
			break;
		case JobType::HYUNWOO:
			tempLobbyRenderers_[i]->SetImage("tempLobbyHyunwoo.png");
			break;
		case JobType::JACKIE:
			tempLobbyRenderers_[i]->SetImage("tempLobbyJackie.png");
			break;
		case JobType::RIO:
			tempLobbyRenderers_[i]->SetImage("tempLobbyRio.png");
			break;
		case JobType::AYA:
			tempLobbyRenderers_[i]->SetImage("tempLobbyAya.png");
			break;
		case JobType::MAX:
			break;
		default:
			break;
		}
	}
}

void LobbyLevel::EndJoin()
{
}

void LobbyLevel::Check_PortraitCollision()
{




}
