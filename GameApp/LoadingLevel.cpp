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
	// �ε�ȭ�� �⺻ ���� ����
	CreateBasicActor();
}

void LoadingLevel::CreateBasicActor()
{
	//// �ΰ��� ���콺 ����
	//if (nullptr == MousePointer::InGameMouse)
	//{
	//	MousePointer::InGameMouse = CreateActor<MousePointer>();
	//	MousePointer::InGameMouse->GetTransform()->SetLocalPosition(GameEngineInput::GetInst().GetMouse3DPos());
	//}

	//// BackDrop
	//BackDrop_ = CreateActor<LoadingLevel_BackDrop>();

	//// Connection Player Loading Percent
	//// -> ���� ���ӿ� ������ �÷��̾��� ���� ���� �ε����� ����ϸ�,
	////    ��� �÷��̾ �ε��� �Ϸ�Ǹ� 100%�� ǥ��
	//LoadPercent_ = CreateActor<LoadingLevel_LoadPercent>();
	//LoadPercent_->SetConnectPlayerCount();
}

void LoadingLevel::CreateGameInfomation()
{
	//// �ʻ��� �� ���������Ŷ �۽�
	//if (false == ReadyMapCreationCommand)
	//{
	//	MapCreationCommand();
	//	ReadyMapCreationCommand = true;
	//	ReadyMonsterCreationCommand = false;
	//}

	//// ���ͻ��� �� ���������Ŷ �۽�
	//if (false == ReadyMonsterCreationCommand)
	//{
	//	MonsterCreationCommand();
	//	ReadyMonsterCreationCommand = true;
	//	ReadyCharacterCreationCommnd = false;
	//}

	//// ĳ���ͻ��� �� ���������Ŷ �۽�
	//if (false == ReadyCharacterCreationCommnd)
	//{
	//	CharacterCreationCommand();
	//	ReadyCharacterCreationCommnd = true;
	//}

	//// ��� ���ҽ� �� ���� �����Ϸ�
	//if (true == ReadyMapCreationCommand &&
	//	true == ReadyMonsterCreationCommand &&
	//	true == ReadyCharacterCreationCommnd)
	//{
	//	// ������ �ε��Ϸ� Flag On
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
	//// �÷��̷����� ���� ���� ���� ���
	//LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("Lumia"));
	//PlayerLevel->OtherLevelCommand(LoadingType::MAP);
}

void LoadingLevel::MonsterCreationCommand()
{
	//// �÷��̷����� ���� ���� ���� ���
	//LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("Lumia"));
	//PlayerLevel->OtherLevelCommand(LoadingType::MONSTER);
}

void LoadingLevel::CharacterCreationCommand()
{
	//// �÷��̷����� ���� ���� ���� ���
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
	//// ���ҽ� �ε��Ϸ� �� ���ͻ��� ��� ��Ŷ �۽�
	//if (true == ResourceLoadEndCheck && true == UserGameServer::GetInstance()->IsOpened())
	//{
	//	CreateGameInfomation();
	//}

	//// ���ҽ� �ε����� üũ �� �⺻ ���� ����
	//if (0 >= UserGame::LoadingFolder &&
	//	false == ResourceLoadEndCheck)
	//{
	//	LoadingLevelInitalize();
	//	ResourceLoadEndCheck = true;
	//}

	//// ������ ��Ŷ ����ó��
	//if (true == UserGameServer::GetInstance()->IsOpened())
	//{
	//	UserGameServer::GetInstance()->ProcessPacket();
	//}

	//// Ŭ���̾�Ʈ�� ��Ŷ ����ó��
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
