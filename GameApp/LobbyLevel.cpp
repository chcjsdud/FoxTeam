#include "Precompile.h"
#include "LobbyLevel.h"
#include "ePacketID.h"
#include "PlayerNumberPacket.h"
#include "CharSelectPacket.h"
#include "GameJoinPacket.h"
#include "Enums.h"

#include "LobbyBackground.h"
#include "Lobby_ButtonOne.h"
#include "TempLobbyRenderer.h"
#include "UI_TestMouse.h"

GameEngineSocketServer LobbyLevel::serverSocket_;
GameEngineSocketClient LobbyLevel::clientSocket_;

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


	{
		LobbyBackground* BackgroundRenderer = CreateActor<LobbyBackground>();
		Lobby_ButtonOne* ButtonOne = CreateActor<Lobby_ButtonOne>();
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
	// ����� :: ȣ��Ʈ�μ� ���� �����Ѵ�
	if (true == GameEngineInput::Down("1"))
	{
		serverSocket_.Initialize();
		serverSocket_.OpenServer();
		serverSocket_.AddPacketHandler(ePacketID::PlayerNumberPacket, new PlayerNumberPacket);
		serverSocket_.AddPacketHandler(ePacketID::GameJoinPacket, new GameJoinPacket);
		serverSocket_.AddPacketHandler(ePacketID::CharSelectPacket, new CharSelectPacket);
		GameEngineDebug::OutPutDebugString("ȣ��Ʈ�μ� ���� ����ϴ�.");


		state_.ChangeState("Select");
		return;
	}


	// ����� :: Ŭ���̾�Ʈ�μ� �濡 �����Ѵ�.
	if (true == GameEngineInput::Down("2"))
	{
		clientSocket_.Initialize();
		clientSocket_.Connect("127.0.0.1");
		clientSocket_.AddPacketHandler(ePacketID::PlayerNumberPacket, new PlayerNumberPacket);
		clientSocket_.AddPacketHandler(ePacketID::GameJoinPacket, new GameJoinPacket);
		clientSocket_.AddPacketHandler(ePacketID::CharSelectPacket, new CharSelectPacket);

		GameEngineDebug::OutPutDebugString("Ŭ���̾�Ʈ�μ� �濡 �����մϴ�.");
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
	
		playerCount_ = serverSocket_.serverPlayerList_.size();
	}
}

void LobbyLevel::UpdateSelect(float _DeltaTime)
{
	// ȣ��Ʈ ������ ��
	if (serverSocket_.IsOpened())
	{
		serverSocket_.ProcessPacket();
		// Ŭ���̾�Ʈ�� ������ �� ������ ���� �����Ǿ�� �ϴ�,
		// ȣ��Ʈ�� �������� �� �������� ������ ���� ����.
		// �� �������� ��� Ŭ���̾�Ʈ�� ������ ���� üũ�ϰ� ������ �ο��Ͽ��� �Ѵ�.
		// ���� �ο��� ���ÿ� ��� ����/ĳ���� ����/�������� ���� �� �޾Ƽ� �������־�� �ϳ�?? �ƴϴ�.
		// Ŭ�󿡼� ���� ��Ŷ�� ��û�ϸ�, �װ͸� �޾Ƽ� ��ε�ĳ���� �� �־ �ȴ�!!!
		// �ϴ� ������ ���� Ŭ���̾�Ʈ�� "���� ���Լ�" �ϰ� ���� ��Ŷ�� ��û�� ��.


		if (playerCount_ != serverSocket_.serverPlayerList_.size())
		{
			// ���� ���� �ο����� ������ ���� �� �������� ��Ŷ�Դϴ�.
			// ����� ���� �ο� ����Ʈ�� ���� ���� ���� ���ؼ�
			GameJoinPacket packet;
			packet.SetOtherPlayers(serverSocket_.GetServerPlayerList());
			serverSocket_.Send(&packet);


			if (playerCount_ < serverSocket_.serverPlayerList_.size())
			{
				PlayerNumberPacket packet1;
				packet1.SetPlayerNumber(serverSocket_.GetClientSocketSize() + 1);
				packet1.SetOtherPlayers(serverSocket_.GetServerPlayerList());
				serverSocket_.Send(serverSocket_.GetSocketList()[serverSocket_.GetClientSocketSize()-1], &packet1);
			}

			playerCount_ = static_cast<int>(serverSocket_.serverPlayerList_.size());
		}

	
		
		//if (true == GameEngineInput::Down("4"))
		//{
		//	CharSelectPacket packet;
		//	packet.SetTargetIndex(1);
		//	packet.SetCharacter(static_cast<int>(JobType::HYUNWOO));
		//	packet.SetStartPoint(static_cast<int>(Location::SCHOOL));
		//
		//	serverSocket_.Send(&packet);
		//	//serverSocket_.serverPlayerList_[0].isReady_ = true;
		//	GameEngineDebug::OutPutDebugString("ȣ��Ʈ ������ ĳ���͸� �����߽��ϴ�.");
		//	state_.ChangeState("Join");
		//	return;
		//}


		// �������� �޶��� ĳ���� ������ ������ �������� �ٲ� �ش�.
		for (int i = 0; i < serverSocket_.serverPlayerList_.size(); i++)
		{

			tempLobbyRenderers_[i]->SetRender(true);
			switch (static_cast<JobType>(serverSocket_.serverPlayerList_[i].character_))
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
	else if (clientSocket_.IsConnected())	// Ŭ���̾�Ʈ�� ��
	{
		clientSocket_.ProcessPacket();
		playerCount_ = static_cast<int>(clientSocket_.serverPlayerList_.size());
		

		if (clientSocket_.myPlayerNumber_ == 3)
		{
			// �ߴ����� �ɾ 
			// ���� Ŭ���̾�Ʈ�� ������ �� ���� �������̽��� ������ ���ŵǴ����� Ȯ���ϴ� ������
			int a = 0;
		}
		if (clientSocket_.serverPlayerList_.size() == 3)
		{
			int a = 0;
		}

		// Ŀ���� ĳ���� �ʻ�ȭ�� �������� ���� ������Դϴ�.
		if (true == GameEngineInput::Down("4"))
		{
			CharSelectPacket packet;
			packet.SetTargetIndex(clientSocket_.myPlayerNumber_); // ���� �÷��̾� ��ȣ�� �˷��ش�
			packet.SetCharacter(static_cast<int>(JobType::JACKIE)); // ���� ĳ���� ������ �˷��ش�
			packet.SetStartPoint(static_cast<int>(Location::UPTOWN)); // ���� ��Ÿ�� ����Ʈ ������ �˷��ش�
			clientSocket_.Send(&packet);
			
			GameEngineDebug::OutPutDebugString("Ŭ���̾�Ʈ ���� " + std::to_string(clientSocket_.myPlayerNumber_) + " ��(��) ĳ���͸� �����߽��ϴ�.");
		}
		if (true == GameEngineInput::Down("5"))
		{
			CharSelectPacket packet;
			packet.SetTargetIndex(clientSocket_.myPlayerNumber_); // ���� �÷��̾� ��ȣ�� �˷��ش�
			packet.SetCharacter(static_cast<int>(JobType::HYUNWOO)); // ���� ĳ���� ������ �˷��ش�
			packet.SetStartPoint(static_cast<int>(Location::DOCK)); // ���� ��Ÿ�� ����Ʈ ������ �˷��ش�
			clientSocket_.Send(&packet);

			GameEngineDebug::OutPutDebugString("Ŭ���̾�Ʈ ���� " + std::to_string(clientSocket_.myPlayerNumber_) + " ��(��) ĳ���͸� �����߽��ϴ�.");
		}

	}


	// �������� �޶��� ĳ���� ������ ������ �������� �ٲ� �ش�.
	for (int i = 0; i < clientSocket_.serverPlayerList_.size(); i++)
	{
		tempLobbyRenderers_[i]->SetRender(true);
		switch (static_cast<JobType>(clientSocket_.serverPlayerList_[i].character_))
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

	if (true == serverSocket_.IsOpened())
	{
		int count = 0;

		for (int i = 0; i < serverSocket_.serverPlayerList_.size(); i++)
		{
			if (true == serverSocket_.serverPlayerList_[i].isReady_)
			{
				count++;
			}

		}

		if (count == serverSocket_.serverPlayerList_.size())
		{
			GameEngineDebug::MsgBoxError("��� �غ� �Ϸ�, ������ �����մϴ�.");
		}
	}
	else if (true == clientSocket_.IsConnected())
	{

	}

}

void LobbyLevel::EndJoin()
{
}
