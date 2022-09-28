#include "Precompile.h"
#include "LoadingLevel.h"

#include "ePacketID.h"
#include "UserGame.h"
#include "LumiaLevel.h"
#include "PlayerInfoManager.h"
#include "GameServer.h"
#include "GameClient.h"

#include "MapCreationPacket.h"
#include "MonsterCreationPacket.h"
#include "CharacterCreationPacket.h"
#include "LoadingEndPacket.h"
#include "LevelChangePacket.h"

#include "MousePointer.h"
#include "LoadingLevel_BackDrop.h"
#include "LoadingLevel_LoadPercent.h"

bool LoadingLevel::ResourceLoadEndCheck = false;

bool LoadingLevel::ReadyMapCreationCommand = false;
bool LoadingLevel::ReadyMonsterCreationCommand = true;
bool LoadingLevel::ReadyCharacterCreationCommnd = true;

void LoadingLevel::LoadingLevelInitalize()
{
	// �ε�ȭ�� �⺻ ���� ����

	// �ΰ��� ���콺 ����
	if (nullptr == MousePointer::InGameMouse)
	{
		MousePointer::InGameMouse = CreateActor<MousePointer>();
		MousePointer::InGameMouse->GetTransform()->SetLocalPosition(GameEngineInput::GetInst().GetMouse3DPos());
	}

	// BackDrop
	BackDrop_ = CreateActor<LoadingLevel_BackDrop>();

	// Connection Player Loading Percent
	// -> ���� ���ӿ� ������ �÷��̾��� ���� ���� �ε����� ����ϸ�,
	//    ��� �÷��̾ �ε��� �Ϸ�Ǹ� 100%�� ǥ��
	LoadPercent_ = CreateActor<LoadingLevel_LoadPercent>();
	LoadPercent_->SetConnectPlayerCount();

	// LoadingLevel Basic Actor Creation End
	ResourceLoadEndCheck = true;
}

void LoadingLevel::InGameCreationCommand()
{
	// ĳ���ͻ��� �� ���������Ŷ �۽�
	if (false == ReadyCharacterCreationCommnd)
	{
		CharacterCreationCommand();
		ReadyCharacterCreationCommnd = true;
	}

	// ���ͻ��� �� ���������Ŷ �۽�
	if (false == ReadyMonsterCreationCommand)
	{
		MonsterCreationCommand();
		ReadyMonsterCreationCommand = true;
		ReadyCharacterCreationCommnd = false;
	}

	// �ʻ��� �� ���������Ŷ �۽�
	if (false == ReadyMapCreationCommand)
	{
		MapCreationCommand();
		ReadyMapCreationCommand = true;
		ReadyMonsterCreationCommand = false;
	}

	// ��� ���ҽ� �� ���� �����Ϸ�
	if (true == ReadyMapCreationCommand &&
		true == ReadyMonsterCreationCommand &&
		true == ReadyCharacterCreationCommnd)
	{
		// ������ �ε��Ϸ� Flag On
		PlayerInfoManager* InfoManager = PlayerInfoManager::GetInstance();
		InfoManager->GetPlayerList()[InfoManager->GetMyNumber()].IsLoading_ = 1;

		// Update Loading ProgressBar
		LoadingLevel_LoadPercent::Percent->CheckLoadingPercent();

		// Server LoadingEnd Packet Send
		LoadingEndPacket EndPacket;
		EndPacket.SetUniqueID(InfoManager->GetMyNumber());
		EndPacket.SetLoadingFlag(InfoManager->GetPlayerList()[InfoManager->GetMyNumber()].IsLoading_);
		GameServer::GetInstance()->Send(&EndPacket);

		// ����� ��� Ŭ���̾�Ʈ(�Խ�Ʈ)�� �ε��� ���� ������ ����
		// ���� ������ ��̾Ʒ����̾ƴϰ�, ������ ������ ��� Ŭ���̾�Ʈ�� �ε��Ϸ�� �ε�Ϸ᷹��ü���� ��Ŷ �۽��� ����ü����
		LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));
		if (true == InfoManager->AllPlayerLoadingStateCheck() && PlayerLevel != UserGame::CurrentLevel())
		{
			// ��ȯ���� true �̶�� ��� �÷��̾�(ȣ��Ʈ ����) �ε��Ϸ� �Ǿ��ٴ� �ǹ��̹Ƿ� ���� ü���� ��Ŷ �۽�
			LevelChangePacket Packet;
			Packet.SetChangeLevelName("LumiaLevel");
			GameServer::GetInstance()->Send(&Packet);

			// ��Ŷ�۽��� �������Ƿ� ���� ������ ü����
			UserGame::LevelChange("LumiaLevel");
		}
	}
}

void LoadingLevel::MapCreationCommand()
{
	// �÷��̷����� ���� ���� ���� ���
	LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));
	PlayerLevel->OtherLevelCommand(LoadingType::MAP);
}

void LoadingLevel::MonsterCreationCommand()
{
	// �÷��̷����� ���� ���� ���� ���
	LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));
	PlayerLevel->OtherLevelCommand(LoadingType::MONSTER);
}

void LoadingLevel::CharacterCreationCommand()
{
	// �÷��̷����� ���� ���� ���� ���
	LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));
	PlayerLevel->OtherLevelCommand(LoadingType::CHARACTER);
}

void LoadingLevel::LevelStart()
{
	// Camera Setting
	GetMainCamera()->SetProjectionMode(ProjectionMode::Orthographic);
	GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.0f, 0.0f, -400.0f));
}

void LoadingLevel::LevelUpdate(float _DeltaTime)
{
	// ���ҽ� �ε��Ϸ� �� ���ͻ��� ��� ��Ŷ �۽�
	if (true == ResourceLoadEndCheck && true == GameServer::GetInstance()->IsOpened())
	{
		InGameCreationCommand();
	}

	// ���ҽ� �ε����� üũ �� �⺻ ���� ����
	if (0 >= UserGame::LoadingFolder && false == ResourceLoadEndCheck)
	{
		LoadingLevelInitalize();
	}

	// ������ ��Ŷ ����ó��
	if (true == GameServer::GetInstance()->IsOpened())
	{
		GameServer::GetInstance()->ProcessPacket();
	}

	// Ŭ���̾�Ʈ�� ��Ŷ ����ó��
	if (true == GameClient::GetInstance()->IsConnected())
	{
		GameClient::GetInstance()->ProcessPacket();
	}
}

void LoadingLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
}

void LoadingLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	// Add Socket Handle
	GameServer* server = GameServer::GetInstance();
	GameClient* client = GameClient::GetInstance();

	if (true == server->IsOpened())
	{
		server->AddPacketHandler(ePacketID::MapCreatPacket, new MapCreationPacket);
		server->AddPacketHandler(ePacketID::MonsterCreatePacket, new MonsterCreationPacket);
		server->AddPacketHandler(ePacketID::CharacterCreatePacket, new CharacterCreationPacket);
		server->AddPacketHandler(ePacketID::LoadEndPacket, new LoadingEndPacket);
		server->AddPacketHandler(ePacketID::LvChangePacket, new LevelChangePacket);
	}

	if (true == client->IsConnected())
	{
		client->AddPacketHandler(ePacketID::MapCreatPacket, new MapCreationPacket);
		client->AddPacketHandler(ePacketID::MonsterCreatePacket, new MonsterCreationPacket);
		client->AddPacketHandler(ePacketID::CharacterCreatePacket, new CharacterCreationPacket);
		client->AddPacketHandler(ePacketID::LoadEndPacket, new LoadingEndPacket);
		client->AddPacketHandler(ePacketID::LvChangePacket, new LevelChangePacket);
	}
}

LoadingLevel::LoadingLevel()
	: BackDrop_(nullptr)
	, LoadPercent_(nullptr)
{
}

LoadingLevel::~LoadingLevel()
{
}
