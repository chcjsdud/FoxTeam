#include "Precompile.h"
#include "LobbyLevel.h"
#include "ePacketID.h"
#include "CharSelectPacket.h"
#include "GameJoinPacket.h"
#include "LobbyPlayerInfo.h"

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
		serverSocket_.AddPacketHandler(ePacketID::GameJoinPacket, new GameJoinPacket);
		serverSocket_.AddPacketHandler(ePacketID::CharSelectPacket, new CharSelectPacket);
		GameEngineDebug::OutPutDebugString("호스트로서 방을 만듭니다.");
		state_.ChangeState("Select");
		
		// 이 시점에 GameEngineSocketServer 에 p1 값이 플레이어 리스트에 들어감
		// 이걸 받아내 내 프로그램 내 리스트에 입력하고(사실상의 ID임) 
		// 점차적으로 플레이어 리스트에 들어오는 클라이언트의 P2, P3... 값을 역시 내 리스트에 입력하고
		// 패킷으로 브로드캐스팅 해 다른 클라이언트들도 자기들의 리스트에 입력하게 해야 전체 등록이 된다.
		//LobbyPlayerInfo info;
		//info.SetPlayerNumber(static_cast<int>(serverSocket_.GetServerPlayerListSize()));
		//info.SetCharacter(1);
		//info.SetStartPoint(1);
		//
		//playerList_.emplace_back(info);
		
		return;
	}


	// 디버깅 :: 클라이언트로서 방에 입장한다.
	if (true == GameEngineInput::Down("2"))
	{
		clientSocket_.Initialize();
		clientSocket_.Connect("127.0.0.1");
		clientSocket_.AddPacketHandler(ePacketID::GameJoinPacket, new GameJoinPacket);
		clientSocket_.AddPacketHandler(ePacketID::CharSelectPacket, new CharSelectPacket);

		GameEngineDebug::OutPutDebugString("클라이언트로서 방에 참여합니다.");
		state_.ChangeState("Select");

		return;
	}
}

void LobbyLevel::EndIdle()
{
	// 클라이언트는 입장하자마자 로그인 순번 패킷을 만들어 서버에 보낸다.

	// 서버는 이를 받아들여 순번을 배정한다.

	if (true == serverSocket_.IsOpened())
	{
		GameJoinPacket packet;
	//	packet.SetAllPlayerList(playerList_);
		serverSocket_.Send(&packet);
	}
	if (true == clientSocket_.IsConnected())
	{
		// 이미 서버는 이 부분 이전부터 클라가 들어온 걸 재고 있었다.
		clientSocket_.ProcessPacket();
		



	}


}

void LobbyLevel::StartSelect()
{
	if (true == clientSocket_.IsConnected())
	{
		// 내가 클라이언트로서 게임 방에 입장했으면
		// 패킷을 보내 순번을 요청할 것.

		// 근데 시벌 그 순번을 클라이언트가 어캐 아냐고...
		// 클라이언트가 아닌 서버가 알고 있을 거임 최초에는
		// 그걸 먼저 배분해야 함.

		// 1. 들어온 순서대로 정렬시켜서 순번을 가져오는 것 까진 가능
		// 2. 


	}
}

void LobbyLevel::UpdateSelect(float _DeltaTime)
{
	// Send 의 시점을 잘 선택해봐야 한다...
	// 디버깅 :: 캐릭터를 선택한다

	if (serverSocket_.IsOpened())
	{
		serverSocket_.ProcessPacket();
		// 클라이언트가 들어오기 전 서버가 먼저 개설되어야 하니,
		// 호스트는 누구보다 이 시점으로 빠르게 들어올 것임.
		// 이 시점부터 계속 클라이언트가 들어오는 것을 체크하고 순번을 부여하여야 한다.
		// 순번 부여와 동시에 계속 순번/캐릭터 선택/시작지점 선택 을 받아서 변경해주어야 하나?? 아니다.
		// 클라에서 변동 패킷을 요청하면, 그것만 받아서 브로드캐스팅 해 주어도 된다!!!
		// 일단 들어오는 순간 클라이언트는 "내가 들어왔소" 하고 변동 패킷을 요청할 것.
		

		if (true == GameEngineInput::Down("3"))
		{
			CharSelectPacket packet;

			GameEngineDebug::OutPutDebugString("호스트 유저가 캐릭터를 선택했습니다.");
			state_.ChangeState("Join");
		}

		// 여기서 패킷이 들어온 값들을 체크해서 비주얼을 바꿔 주는 무언가가 필요할까?
		// 서버일 때 : 비주얼을 바꿔 주고 바뀐 값을 다른 클라이언트에게 브로드캐스팅 해 준다.
		// ex) (내 비주얼을 바꾸고) 2번 3번 클라이언트야 신입 4번이 들어왔다.


	}
	else if(clientSocket_.IsConnected())
	{
		clientSocket_.ProcessPacket();
	
		if (true == GameEngineInput::Down("3"))
		{
			//GameJoinPacket packet;
			//packet.SetCharacter(3);
			//packet.SetStartPoint(4);
			//clientSocket_.Send(&packet);
			GameEngineDebug::OutPutDebugString("클라이언트 유저가 캐릭터를 선택했습니다.");
			state_.ChangeState("Join");
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
	// 디버깅 :: 게임을 시작하고 패킷을 입력해 서버에 보낸다
	if (serverSocket_.IsOpened())
	{
		if (true == GameEngineInput::Down("4"))
		{
			//GameJoinPacket packet;
			//packet.SetCharacter(1);
			//packet.SetStartPoint(2);
			GameEngineDebug::OutPutDebugString("호스트 유저가 캐릭터를 선택했습니다.");
		}

		serverSocket_.ProcessPacket();
	}
	else if (clientSocket_.IsConnected())
	{
		if (true == GameEngineInput::Down("4"))
		{
			//GameJoinPacket packet;
			//packet.SetCharacter(3);
			//packet.SetStartPoint(4);
			//clientSocket_.Send(&packet);
			GameEngineDebug::OutPutDebugString("클라이언트 유저가 캐릭터를 선택했습니다.");
		}

		clientSocket_.ProcessPacket();
	}
}

void LobbyLevel::EndJoin()
{
}
