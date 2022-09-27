#include "Precompile.h"
#include "LoadingLevel.h"

//#include "SynchronizationInfoManager.h"
#include "PlayerInfoManager.h"
#include "UserGame.h"
#include "LumiaLevel.h"
#include "GameServer.h"
#include "GameClient.h"

//#include "LoadingEndPacket.h"

#include "MousePointer.h"
//#include "LoadingLevel_BackDrop.h"
//#include "LoadingLevel_LoadPercent.h"

bool LoadingLevel::ResourceLoadEndCheck = false;

bool LoadingLevel::ReadyMapCreationCommand = false;
bool LoadingLevel::ReadyMonsterCreationCommand = true;
bool LoadingLevel::ReadyCharacterCreationCommnd = true;

void LoadingLevel::LoadingLevelInitalize()
{
	// 로딩화면 기본 액터 생성
	CreateBasicActor();
}

void LoadingLevel::CreateBasicActor()
{
	//// 인게임 마우스 생성
	//if (nullptr == MousePointer::InGameMouse)
	//{
	//	MousePointer::InGameMouse = CreateActor<MousePointer>();
	//	MousePointer::InGameMouse->GetTransform()->SetLocalPosition(GameEngineInput::GetInst().GetMouse3DPos());
	//}

	//// BackDrop
	//BackDrop_ = CreateActor<LoadingLevel_BackDrop>();

	//// Connection Player Loading Percent
	//// -> 현재 게임에 참여한 플레이어의 수에 따라 로딩율을 계산하며,
	////    모든 플레이어가 로딩이 완료되면 100%로 표시
	//LoadPercent_ = CreateActor<LoadingLevel_LoadPercent>();
	//LoadPercent_->SetConnectPlayerCount();
}

void LoadingLevel::CreateGameInfomation()
{
	//// 맵생성 및 생성명령패킷 송신
	//if (false == ReadyMapCreationCommand)
	//{
	//	MapCreationCommand();
	//	ReadyMapCreationCommand = true;
	//	ReadyMonsterCreationCommand = false;
	//}

	//// 몬스터생성 및 생성명령패킷 송신
	//if (false == ReadyMonsterCreationCommand)
	//{
	//	MonsterCreationCommand();
	//	ReadyMonsterCreationCommand = true;
	//	ReadyCharacterCreationCommnd = false;
	//}

	//// 캐릭터생성 및 생성명령패킷 송신
	//if (false == ReadyCharacterCreationCommnd)
	//{
	//	CharacterCreationCommand();
	//	ReadyCharacterCreationCommnd = true;
	//}

	//// 모든 리소스 및 액터 생성완료
	//if (true == ReadyMapCreationCommand &&
	//	true == ReadyMonsterCreationCommand &&
	//	true == ReadyCharacterCreationCommnd)
	//{
	//	// 서버의 로딩완료 Flag On
	//	PlayerInfoManager* InfoManager = PlayerInfoManager::GetInstance();
	//	InfoManager->GetAllPlayerList()[InfoManager->GetMyID()].IsLoading_ = 1;

	//	// Update Loading ProgressBar
	//	LoadingLevel_LoadPercent::Percent->CheckLoadingPercent();

	//	// Server LoadingEnd Packet Send
	//	LoadingEndPacket EndPacket;
	//	EndPacket.SetUniqueID(InfoManager->GetMyID());
	//	EndPacket.SetLoadingFlag(InfoManager->GetAllPlayerList()[InfoManager->GetMyID()].IsLoading_);
	//	UserGameServer::GetInstance()->Send(&EndPacket);
	//}
}

void LoadingLevel::MapCreationCommand()
{
	//// 플레이레벨에 강제 액터 생성 명령
	//LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("Lumia"));
	//PlayerLevel->OtherLevelCommand(LoadingType::MAP);
}

void LoadingLevel::MonsterCreationCommand()
{
	//// 플레이레벨에 강제 액터 생성 명령
	//LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("Lumia"));
	//PlayerLevel->OtherLevelCommand(LoadingType::MONSTER);
}

void LoadingLevel::CharacterCreationCommand()
{
	//// 플레이레벨에 강제 액터 생성 명령
	//LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("Lumia"));
	//PlayerLevel->OtherLevelCommand(LoadingType::CHARACTER);
}

void LoadingLevel::LevelStart()
{
	// Camera Setting
	GetMainCamera()->SetProjectionMode(ProjectionMode::Orthographic);
	GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.0f, 0.0f, -400.0f));
}

void LoadingLevel::LevelUpdate(float _DeltaTime)
{
	//// 리소스 로딩완료 후 액터생성 명령 패킷 송신
	//if (true == ResourceLoadEndCheck && true == UserGameServer::GetInstance()->IsOpened())
	//{
	//	CreateGameInfomation();
	//}

	//// 리소스 로딩상태 체크 후 기본 액터 생성
	//if (0 >= UserGame::LoadingFolder &&
	//	false == ResourceLoadEndCheck)
	//{
	//	LoadingLevelInitalize();
	//	ResourceLoadEndCheck = true;
	//}

	//// 서버측 패킷 수신처리
	//if (true == UserGameServer::GetInstance()->IsOpened())
	//{
	//	UserGameServer::GetInstance()->ProcessPacket();
	//}

	//// 클라이언트측 패킷 수신처리
	//if (true == UserGameClient::GetInstance()->IsConnected())
	//{
	//	UserGameClient::GetInstance()->ProcessPacket();
	//}
}

void LoadingLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
}

void LoadingLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
}

LoadingLevel::LoadingLevel()
	: BackDrop_(nullptr)
	, LoadPercent_(nullptr)
{
}

LoadingLevel::~LoadingLevel()
{
}
