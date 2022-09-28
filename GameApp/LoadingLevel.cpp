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
	// 로딩화면 기본 액터 생성

	// 인게임 마우스 생성
	if (nullptr == MousePointer::InGameMouse)
	{
		MousePointer::InGameMouse = CreateActor<MousePointer>();
		MousePointer::InGameMouse->GetTransform()->SetLocalPosition(GameEngineInput::GetInst().GetMouse3DPos());
	}

	// BackDrop
	BackDrop_ = CreateActor<LoadingLevel_BackDrop>();

	// Connection Player Loading Percent
	// -> 현재 게임에 참여한 플레이어의 수에 따라 로딩율을 계산하며,
	//    모든 플레이어가 로딩이 완료되면 100%로 표시
	LoadPercent_ = CreateActor<LoadingLevel_LoadPercent>();
	LoadPercent_->SetConnectPlayerCount();

	// LoadingLevel Basic Actor Creation End
	ResourceLoadEndCheck = true;
}

void LoadingLevel::InGameCreationCommand()
{
	// 캐릭터생성 및 생성명령패킷 송신
	if (false == ReadyCharacterCreationCommnd)
	{
		CharacterCreationCommand();
		ReadyCharacterCreationCommnd = true;
	}

	// 몬스터생성 및 생성명령패킷 송신
	if (false == ReadyMonsterCreationCommand)
	{
		MonsterCreationCommand();
		ReadyMonsterCreationCommand = true;
		ReadyCharacterCreationCommnd = false;
	}

	// 맵생성 및 생성명령패킷 송신
	if (false == ReadyMapCreationCommand)
	{
		MapCreationCommand();
		ReadyMapCreationCommand = true;
		ReadyMonsterCreationCommand = false;
	}

	// 모든 리소스 및 액터 생성완료
	if (true == ReadyMapCreationCommand &&
		true == ReadyMonsterCreationCommand &&
		true == ReadyCharacterCreationCommnd)
	{
		// 서버의 로딩완료 Flag On
		PlayerInfoManager* InfoManager = PlayerInfoManager::GetInstance();
		InfoManager->GetPlayerList()[InfoManager->GetMyNumber()].IsLoading_ = 1;

		// Update Loading ProgressBar
		LoadingLevel_LoadPercent::Percent->CheckLoadingPercent();

		// Server LoadingEnd Packet Send
		LoadingEndPacket EndPacket;
		EndPacket.SetUniqueID(InfoManager->GetMyNumber());
		EndPacket.SetLoadingFlag(InfoManager->GetPlayerList()[InfoManager->GetMyNumber()].IsLoading_);
		GameServer::GetInstance()->Send(&EndPacket);

		// 연결된 모든 클라이언트(게스트)가 로딩이 먼저 끝남을 방지
		// 현재 레벨이 루미아레벨이아니고, 서버를 포함한 모든 클라이언트가 로딩완료시 로드완료레벨체인지 패킷 송신후 레벨체인지
		LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));
		if (true == InfoManager->AllPlayerLoadingStateCheck() && PlayerLevel != UserGame::CurrentLevel())
		{
			// 반환값이 true 이라면 모든 플레이어(호스트 포함) 로딩완료 되었다는 의미이므로 레벨 체인지 패킷 송신
			LevelChangePacket Packet;
			Packet.SetChangeLevelName("LumiaLevel");
			GameServer::GetInstance()->Send(&Packet);

			// 패킷송신이 끝났으므로 나의 레벨도 체인지
			UserGame::LevelChange("LumiaLevel");
		}
	}
}

void LoadingLevel::MapCreationCommand()
{
	// 플레이레벨에 강제 액터 생성 명령
	LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));
	PlayerLevel->OtherLevelCommand(LoadingType::MAP);
}

void LoadingLevel::MonsterCreationCommand()
{
	// 플레이레벨에 강제 액터 생성 명령
	LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));
	PlayerLevel->OtherLevelCommand(LoadingType::MONSTER);
}

void LoadingLevel::CharacterCreationCommand()
{
	// 플레이레벨에 강제 액터 생성 명령
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
	// 리소스 로딩완료 후 액터생성 명령 패킷 송신
	if (true == ResourceLoadEndCheck && true == GameServer::GetInstance()->IsOpened())
	{
		InGameCreationCommand();
	}

	// 리소스 로딩상태 체크 후 기본 액터 생성
	if (0 >= UserGame::LoadingFolder && false == ResourceLoadEndCheck)
	{
		LoadingLevelInitalize();
	}

	// 서버측 패킷 수신처리
	if (true == GameServer::GetInstance()->IsOpened())
	{
		GameServer::GetInstance()->ProcessPacket();
	}

	// 클라이언트측 패킷 수신처리
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
