#include "Precompile.h"
#include "LobbyLevel.h"
#include "ePacketID.h"
#include "PlayerNumberPacket.h"
#include "CharSelectPacket.h"
#include "GameJoinPacket.h"
#include "Enums.h"

#include "LobbyBackground.h"
#include "TempLobbyRenderer.h"

GameEngineSocketServer LobbyLevel::serverSocket_;
GameEngineSocketClient LobbyLevel::clientSocket_;

LobbyLevel::LobbyLevel()
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
	}

}



void LobbyLevel::LevelUpdate(float _DeltaTime)
{
	state_.Update(_DeltaTime);


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
			renderer_->SetPos({ (-500.f + 200.f*i), -280.f, 0.f});
			renderer_->SetScale({ 122.f, 155.f });
			renderer_->SetRender(false);
			tempLobbyRenderers_.emplace_back(renderer_);
		}



	}


//	{
//		LobbyBackground* BackgroundRenderer = CreateActor<LobbyBackground>();
//	}


}



// FSM Method
void LobbyLevel::StartIdle()
{

}

void LobbyLevel::UpdateIdle(float _DeltaTime)
{
	// 디버깅 :: 호스트로서 방을 개설한다
	if (true == GameEngineInput::Down("1"))
	{
		serverSocket_.Initialize();
		serverSocket_.OpenServer();
		serverSocket_.AddPacketHandler(ePacketID::PlayerNumberPacket, new PlayerNumberPacket);
		serverSocket_.AddPacketHandler(ePacketID::GameJoinPacket, new GameJoinPacket);
		serverSocket_.AddPacketHandler(ePacketID::CharSelectPacket, new CharSelectPacket);
		GameEngineDebug::OutPutDebugString("호스트로서 방을 만듭니다.");


		state_.ChangeState("Select");
		return;
	}


	// 디버깅 :: 클라이언트로서 방에 입장한다.
	if (true == GameEngineInput::Down("2"))
	{
		clientSocket_.Initialize();
		clientSocket_.Connect("127.0.0.1");
		clientSocket_.AddPacketHandler(ePacketID::PlayerNumberPacket, new PlayerNumberPacket);
		clientSocket_.AddPacketHandler(ePacketID::GameJoinPacket, new GameJoinPacket);
		clientSocket_.AddPacketHandler(ePacketID::CharSelectPacket, new CharSelectPacket);

		GameEngineDebug::OutPutDebugString("클라이언트로서 방에 참여합니다.");
		state_.ChangeState("Select");

		return;
	}
}

void LobbyLevel::EndIdle()
{


}

void LobbyLevel::StartSelect()
{
	if (true == clientSocket_.IsConnected())
	{

	}

	if (true == serverSocket_.IsOpened())
	{
	
		playerList_ = serverSocket_.serverPlayerList_;
	}
}

void LobbyLevel::UpdateSelect(float _DeltaTime)
{
	// 호스트 서버일 시
	if (serverSocket_.IsOpened())
	{
		serverSocket_.ProcessPacket();
		// 클라이언트가 들어오기 전 서버가 먼저 개설되어야 하니,
		// 호스트는 누구보다 이 시점으로 빠르게 들어올 것임.
		// 이 시점부터 계속 클라이언트가 들어오는 것을 체크하고 순번을 부여하여야 한다.
		// 순번 부여와 동시에 계속 순번/캐릭터 선택/시작지점 선택 을 받아서 변경해주어야 하나?? 아니다.
		// 클라에서 변동 패킷을 요청하면, 그것만 받아서 브로드캐스팅 해 주어도 된다!!!
		// 일단 들어오는 순간 클라이언트는 "내가 들어왔소" 하고 변동 패킷을 요청할 것.


		if (playerList_.size() != serverSocket_.serverPlayerList_.size())
		{
			// 서버 참석 인원수에 변동이 있을 때 보내지는 패킷입니다.
			// 멤버로 가진 인원 리스트와 서버 소켓 수를 비교해서
			GameJoinPacket packet;
			packet.SetAllPlayerList(serverSocket_.serverPlayerList_);
			serverSocket_.Send(&packet);

			if (true)
			{
				PlayerNumberPacket packet1;
				packet1.SetPlayerNumber(serverSocket_.GetClientSocketSize() + 1);
				serverSocket_.Send(&packet1, serverSocket_.GetClientSocketSize());
			}
		}

		playerList_ = serverSocket_.serverPlayerList_;
		
		if (true == GameEngineInput::Down("3"))
		{
			CharSelectPacket packet;
			packet.SetCharacter(static_cast<int>(JobType::HYUNWOO));
			packet.SetStartPoint(static_cast<int>(Location::SCHOOL));
			packet.SetIsReady(true);
			serverSocket_.Send(&packet);
			serverSocket_.serverPlayerList_[0] = true;
			GameEngineDebug::OutPutDebugString("호스트 유저가 캐릭터를 선택했습니다.");
			state_.ChangeState("Join");
		}

		// 여기서 패킷이 들어온 값들을 체크해서 비주얼을 바꿔 주는 무언가가 필요할까?
		// 서버일 때 : 비주얼을 바꿔 주고 바뀐 값을 다른 클라이언트에게 브로드캐스팅 해 준다.
		// ex) (내 비주얼을 바꾸고) 2번 3번 클라이언트야 신입 4번이 들어왔다.


	}
	else if (clientSocket_.IsConnected())	// 클라이언트일 시
	{
		clientSocket_.ProcessPacket();
		playerList_ = clientSocket_.serverPlayerList_;
		









		if (clientSocket_.playerNumber_ == 3)
		{
			// 중단점을 걸어서 
			// 만약 클라이언트가 들어왔을 시 서버 인터페이스의 정보가 갱신되는지를 확인하는 스코프
			int a = 0;
		}

		//if (true == GameEngineInput::Down("3"))
		//{
		//	CharSelectPacket packet;
		//	packet.SetCharacter(static_cast<int>(JobType::JACKIE));
		//	packet.SetStartPoint(static_cast<int>(Location::UPTOWN));
		//	packet.SetIsReady(true);
		//	clientSocket_.Send(&packet);
		//	clientSocket_.serverPlayerList_[clientSocket_.playerNumber_] = true;
		//	GameEngineDebug::OutPutDebugString("클라이언트 유저가 캐릭터를 선택했습니다.");
		//	state_.ChangeState("Join");
		//}

	}

	for (int i = 0; i < playerList_.size(); i++)
	{
		tempLobbyRenderers_[i]->SetRender(true);
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

	if (true == serverSocket_.IsOpened())
	{

	}
	else if (true == clientSocket_.IsConnected())
	{

	}

}

void LobbyLevel::EndJoin()
{
}
