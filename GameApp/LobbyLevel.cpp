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
#include "TempLobbyRenderer.h"
#include "UI_TestMouse.h"

#include "GameServer.h"
#include "GameClient.h"
#include "PlayerInfoManager.h"
#include "SetPlayerNumberPacket.h"

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
		GameEngineInput::GetInst().CreateKey("Ready", VK_RETURN);
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


	{
		LobbyBackground* BackgroundRenderer = CreateActor<LobbyBackground>();
		Lobby_ButtonOne* ButtonOne = CreateActor<Lobby_ButtonOne>();
		Lobby_ButtonTwo* ButtonTwo = CreateActor<Lobby_ButtonTwo>();
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

	// ������ ���̴� ������Ʈ��
	// �⺻������ �� ���μ����� [ȣ��Ʈ�� �� / Ŭ���̾�Ʈ�� ��] �� ��� �����ϰ�, if ������ �����ؼ� �ڵ带 ¥�ž� �մϴ�.
	// 
	if (true == GameEngineInput::Down("1"))
	{
		//  ���μ����� ȣ��Ʈ�� ���� (= ���� ����ڴ�)
		serverSocket_->Initialize();
		serverSocket_->OpenServer();
		serverSocket_->AddPacketHandler(ePacketID::PlayerNumberPacket, new PlayerNumberPacket);
		serverSocket_->AddPacketHandler(ePacketID::GameJoinPacket, new GameJoinPacket);
		serverSocket_->AddPacketHandler(ePacketID::GameJoinPacket2, new GameJoinPacket2);
		serverSocket_->AddPacketHandler(ePacketID::SetPlayerNumberPacket, new SetPlayerNumberPacket);
		serverSocket_->AddPacketHandler(ePacketID::CharSelectPacket, new CharSelectPacket);
		serverSocket_->AddPacketHandler(ePacketID::ReadyPacket, new ReadyPacket);
		GameEngineDebug::OutPutDebugString("ȣ��Ʈ�μ� ���� ����ϴ�.");

		PlayerInfoManager::GetInstance()->AddNewPlayer({ 0, -1, -1, 0 });
		// AddNewPlayer() �� �Ķ���ʹ� �� PlayerInfo �� ������ �Ķ���ͷ�,
		// ������� {�÷��̾� ��ȣ, ĳ����, ���� ����, �غ� ����} �� �̴ϼȶ����� ���Դϴ�.
	
		PlayerInfoManager::GetInstance()->SetPlayerNumber(0);
		// �ش� �÷��̾� ��ȣ�� �� ���μ����� ��� �����ε� ���� �����մϴ�.

		state_.ChangeState("Select");
		return;
	}

	// ���μ����� Ŭ���̾�Ʈ�� ���� (= �濡 �����ϰڴ�)
	if (true == GameEngineInput::Down("2"))
	{
		clientSocket_->Initialize();
		clientSocket_->Connect("10.0.4.126");
		clientSocket_->AddPacketHandler(ePacketID::PlayerNumberPacket, new PlayerNumberPacket);
		clientSocket_->AddPacketHandler(ePacketID::GameJoinPacket, new GameJoinPacket);
		clientSocket_->AddPacketHandler(ePacketID::GameJoinPacket2, new GameJoinPacket2);
		clientSocket_->AddPacketHandler(ePacketID::SetPlayerNumberPacket, new SetPlayerNumberPacket);
		clientSocket_->AddPacketHandler(ePacketID::CharSelectPacket, new CharSelectPacket);
		clientSocket_->AddPacketHandler(ePacketID::ReadyPacket, new ReadyPacket);

		GameEngineDebug::OutPutDebugString("Ŭ���̾�Ʈ�μ� �濡 �����մϴ�.");
		
		// �ٷ� �÷��̾� ������ �����ϴ� ȣ��Ʈ�ʹ� �޸�, Ŭ���̾�Ʈ�� ���� ��û ��Ŷ�� �����ϴ�.
		// ��Ŷ�� ������ �˸��� �÷��̾� ������ ������ ȣ��Ʈ�� �ٽ� ������ ������ ��ε�ĳ���� �� �ݴϴ�.
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


	// ȣ��Ʈ ������ ��
	if (serverSocket_->IsOpened())
	{
		serverSocket_->ProcessPacket();

		// Ŀ���� ĳ���� �ʻ�ȭ�� �������� ���� ������Դϴ�.
		if (true == GameEngineInput::Down("4"))
		{
			CharSelectPacket packet;
			packet.SetTargetIndex(pm->GetMyNumber()); // ���� �÷��̾� ��ȣ�� �˷��ش�
			packet.SetCharacter(static_cast<int>(JobType::JACKIE)); // ���� ĳ���� ������ �˷��ش�
			packet.SetStartPoint(static_cast<int>(Location::UPTOWN)); // ���� ��Ÿ�� ����Ʈ ������ �˷��ش�
			serverSocket_->Send(&packet);

			pm->GetPlayerList()[pm->GetMyNumber()].character_ = static_cast<int>(JobType::JACKIE);
			pm->GetPlayerList()[pm->GetMyNumber()].startPoint_ = static_cast<int>(Location::UPTOWN);

			GameEngineDebug::OutPutDebugString("ȣ��Ʈ ������ ĳ���͸� �����߽��ϴ�\n");
		}
		if (true == GameEngineInput::Down("5"))
		{
			CharSelectPacket packet;
			packet.SetTargetIndex(pm->GetMyNumber()); // ���� �÷��̾� ��ȣ�� �˷��ش�
			packet.SetCharacter(static_cast<int>(JobType::HYUNWOO)); // ���� ĳ���� ������ �˷��ش�
			packet.SetStartPoint(static_cast<int>(Location::DOCK)); // ���� ��Ÿ�� ����Ʈ ������ �˷��ش�
			serverSocket_->Send(&packet);

			pm->GetPlayerList()[pm->GetMyNumber()].character_ = static_cast<int>(JobType::HYUNWOO);
			pm->GetPlayerList()[pm->GetMyNumber()].startPoint_ = static_cast<int>(Location::DOCK);

			GameEngineDebug::OutPutDebugString("ȣ��Ʈ ������ ĳ���͸� �����߽��ϴ�.\n");
		}


		if (true == GameEngineInput::Down("Ready"))
		{
			if (-1 == pm->GetPlayerList()[pm->GetMyNumber()].character_)
			{
				GameEngineDebug::OutPutDebugString("ĳ���͸� �������ּ���!!\n");
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
	else if (clientSocket_->IsConnected())	// Ŭ���̾�Ʈ�� ��
	{
		clientSocket_->ProcessPacket();
		playerCount_ = static_cast<int>(pm->GetPlayerList().size());

		// Ŀ���� ĳ���� �ʻ�ȭ�� �������� ���� ������Դϴ�.
		if (true == GameEngineInput::Down("4"))
		{
			CharSelectPacket packet;
			packet.SetTargetIndex(pm->GetMyNumber()); // ���� �÷��̾� ��ȣ�� �˷��ش�
			packet.SetCharacter(static_cast<int>(JobType::JACKIE)); // ���� ĳ���� ������ �˷��ش�
			packet.SetStartPoint(static_cast<int>(Location::UPTOWN)); // ���� ��Ÿ�� ����Ʈ ������ �˷��ش�
			clientSocket_->Send(&packet);

			GameEngineDebug::OutPutDebugString("Ŭ���̾�Ʈ ���� " + std::to_string(pm->GetMyNumber()) + " ��(��) ĳ���͸� �����߽��ϴ�.\n");
		}
		if (true == GameEngineInput::Down("5"))
		{
			CharSelectPacket packet;
			packet.SetTargetIndex(pm->GetMyNumber()); // ���� �÷��̾� ��ȣ�� �˷��ش�
			packet.SetCharacter(static_cast<int>(JobType::HYUNWOO)); // ���� ĳ���� ������ �˷��ش�
			packet.SetStartPoint(static_cast<int>(Location::DOCK)); // ���� ��Ÿ�� ����Ʈ ������ �˷��ش�
			clientSocket_->Send(&packet);

			GameEngineDebug::OutPutDebugString("Ŭ���̾�Ʈ ���� " + std::to_string(pm->GetMyNumber()) + " ��(��) ĳ���͸� �����߽��ϴ�.\n");
		}


		if (true == GameEngineInput::Down("Ready"))
		{
			if (-1 == pm->GetPlayerList()[pm->GetMyNumber()].character_)
			{
				GameEngineDebug::OutPutDebugString("ĳ���͸� �������ּ���!!\n");
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


	// �������� �޶��� ĳ���� ������ ������ �������� �ٲ� �ش�.
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
	// �ٸ� ����� �غ� ��ٸ��ų�, ��� ��ư�� ���� �ٽ� ĳ���� �������� ���ư��� �ܰ��Դϴ�.
	// ���� �������� ���� ȣ��Ʈ�� ��ü ���� ��Ȳ�� Ȯ���ϰ� ������ �����մϴ�.

	GameServer* serverSocket_ = GameServer::GetInstance();
	GameClient* clientSocket_ = GameClient::GetInstance();
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	if (true == serverSocket_->IsOpened())
	{
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
			GameEngineDebug::MsgBoxError("��� �غ� �Ϸ�, ������ �����մϴ�.");
		}
	}
	else if (true == clientSocket_->IsConnected())
	{

	}

}

void LobbyLevel::EndJoin()
{
}
