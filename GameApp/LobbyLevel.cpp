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
		// Idle : �ƹ� �Է��� ���� ������ ù �����Դϴ�.
		// ȣ��Ʈ ���� | Ŭ���̾�Ʈ�� ������ Ȯ���� �� ĳ���� ���� ������ Select �� �Ѿ�ϴ�.

		state_.CreateState<LobbyLevel>("Select", this, &LobbyLevel::StartSelect, &LobbyLevel::UpdateSelect, &LobbyLevel::EndSelect);
		// Select : ���� �濡 �������� ������ ĳ���Ϳ� Ư��, ���� ��ġ�� ���� �� �ִ� �����Դϴ�.
		// ������ �Ϸ� �� ���ϴ��� "ĳ���� ����" ��ư�� ������ Join ���·� �Ѿ�ϴ�.

		state_.CreateState<LobbyLevel>("Join", this, &LobbyLevel::StartJoin, &LobbyLevel::UpdateJoin, &LobbyLevel::EndJoin);
		// Join : ĳ���� ������ �������ϰ� �ٸ� �÷��̾ ��ٸ��� �����Դϴ�.

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
	// ����� :: ȣ��Ʈ�μ� ���� �����Ѵ�
	if (true == GameEngineInput::Down("1"))
	{
		serverSocket_.Initialize();
		serverSocket_.OpenServer();
		serverSocket_.AddPacketHandler(ePacketID::GameJoinPacket, new GameJoinPacket);
		serverSocket_.AddPacketHandler(ePacketID::CharSelectPacket, new CharSelectPacket);
		GameEngineDebug::OutPutDebugString("ȣ��Ʈ�μ� ���� ����ϴ�.");
		state_.ChangeState("Select");

		//GameJoinPacket packet;
		//packet.SetPlayerNumber(1);
		//serverSocket_.ProcessPacket();

		return;
	}


	// ����� :: Ŭ���̾�Ʈ�μ� �濡 �����Ѵ�.
	if (true == GameEngineInput::Down("2"))
	{
		clientSocket_.Initialize();
		clientSocket_.Connect("127.0.0.1");
		clientSocket_.AddPacketHandler(ePacketID::GameJoinPacket, new GameJoinPacket);
		clientSocket_.AddPacketHandler(ePacketID::CharSelectPacket, new CharSelectPacket);

		GameEngineDebug::OutPutDebugString("Ŭ���̾�Ʈ�μ� �濡 �����մϴ�.");
		state_.ChangeState("Select");

		
		GameJoinPacket packet;
		//packet.SetPlayerNumber(clientSocket_.)
		//clientSocket_.Send(&packet);
		//LobbyPlayerInfo hostPlayerInfo;
		//hostPlayerInfo.SetPlayerNumber(1);
		//playerList_.push_back(hostPlayerInfo);

		return;
	}
}

void LobbyLevel::EndIdle()
{
	// Ŭ���̾�Ʈ�� �������ڸ��� �α��� ���� ��Ŷ�� ����� ������ ������.

	// ������ �̸� �޾Ƶ鿩 ������ �����Ѵ�.

}

void LobbyLevel::StartSelect()


{

}

void LobbyLevel::UpdateSelect(float _DeltaTime)
{
	// Send �� ������ �� �����غ��� �Ѵ�...
	// ����� :: ĳ���͸� �����Ѵ�

	if (serverSocket_.IsOpened())
	{
		serverSocket_.ProcessPacket();
		// Ŭ���̾�Ʈ�� ������ �� ������ ���� �����Ǿ�� �ϴ�,
		// ȣ��Ʈ�� �������� �� �������� ������ ���� ����.
		// �� �������� ��� Ŭ���̾�Ʈ�� ������ ���� üũ�ϰ� ������ �ο��Ͽ��� �Ѵ�.
		// �ٵ� ���?

		if (true == GameEngineInput::Down("3"))
		{
			CharSelectPacket packet;

			GameEngineDebug::OutPutDebugString("ȣ��Ʈ ������ ĳ���͸� �����߽��ϴ�.");
			state_.ChangeState("Join");
		}

		// ���⼭ ��Ŷ�� ���� ������ üũ�ؼ� ���־��� �ٲ� �ִ� ���𰡰� �ʿ��ұ�?
		// ������ �� : ���־��� �ٲ� �ְ� �ٲ� ���� �ٸ� Ŭ���̾�Ʈ���� ��ε�ĳ���� �� �ش�.
		// ex) (�� ���־��� �ٲٰ�) 2�� 3�� Ŭ���̾�Ʈ�� ���� 4���� ���Դ�.


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
			GameEngineDebug::OutPutDebugString("Ŭ���̾�Ʈ ������ ĳ���͸� �����߽��ϴ�.");
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
	// ����� :: ������ �����ϰ� ��Ŷ�� �Է��� ������ ������
	if (serverSocket_.IsOpened())
	{
		if (true == GameEngineInput::Down("4"))
		{
			//GameJoinPacket packet;
			//packet.SetCharacter(1);
			//packet.SetStartPoint(2);
			GameEngineDebug::OutPutDebugString("ȣ��Ʈ ������ ĳ���͸� �����߽��ϴ�.");
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
			GameEngineDebug::OutPutDebugString("Ŭ���̾�Ʈ ������ ĳ���͸� �����߽��ϴ�.");
		}

		clientSocket_.ProcessPacket();
	}
}

void LobbyLevel::EndJoin()
{
}
