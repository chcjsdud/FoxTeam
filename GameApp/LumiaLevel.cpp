#include "Precompile.h"
#include "LumiaLevel.h"

#include <GameEngine/GameEngineRenderWindow.h>
#include <GameEngine/SKySphereActor.h>
#include <GameEngine/GameEngineLevelControlWindow.h>
#include <GameEngine/LightActor.h>
#include <GameEngine/GameEngineFBXMesh.h>

#include "ItemBoxWindow.h"

#include "Enums.h"
#include "ePacketID.h"
#include "PlayerInfoManager.h"
#include "MonsterInfoManager.h"
#include "UserGame.h"
#include "GameServer.h"
#include "GameClient.h"

#include "CharMovementPacket.h"
#include "CharAnimationPacket.h"
#include "CharStatPacket.h"
#include "MapCreationPacket.h"
#include "MonsterCreationPacket.h"
#include "CharacterCreationPacket.h"

//======================== Map
#include "LumiaMap.h"

//======================== Characters
#include "Character.h"
#include "Dummy.h"
#include "Rio.h"
#include "Hyunwoo.h"

//======================== Monsters
#include "Monsters.h"
#include "Wolf.h"


void LumiaLevel::OtherLevelCommand(LoadingType _LoadingType)
{
	// �ε������κ��� �����ϴ� ���
	switch (_LoadingType)
	{
	case LoadingType::MAP:
	{
		// ���� ���� �ʿ����
		// �ʻ��� ��� ��Ŷ ������
		MapCreationPacketSend();

		// ���� ������ ��Ŷ ����
		if (true == MapCreationCommand() && true == GameServer::GetInstance()->IsOpened())
		{
			GameEngineDebug::OutPutDebugString("����(ȣ��Ʈ)�� �� ������ �Ϸ��߽��ϴ�!!!!\n");
		}

		break;
	}
	case LoadingType::MONSTER:
	{
		// ���� ������ ��Ŷ ����
		if (true == MonsterCreationCommand() && true == GameServer::GetInstance()->IsOpened())
		{
			GameEngineDebug::OutPutDebugString("����(ȣ��Ʈ)�� ���� ������ �Ϸ��߽��ϴ�!!!!\n");

			// ���� ���� �ʿ�
			// ���� ���� �����Ϸ� �� ��Ŷ ����
			MonsterCreationPacketSend();
		}

		break;
	}
	case LoadingType::CHARACTER:
	{
		// ���� ���� �ʿ����
		// ĳ���ͻ��� ��� ��Ŷ ������
		CharacterCreationPacketSend();

		// ���� ������ ��Ŷ ����
		if (true == CharacterCreationCommand() && true == GameServer::GetInstance()->IsOpened())
		{
			GameEngineDebug::OutPutDebugString("����(ȣ��Ʈ)�� ĳ���� ������ �Ϸ��߽��ϴ�!!!!\n");
		}

		break;
	}
	}
}

void LumiaLevel::ClientMapCreation()
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	if (true == MapCreationCommand())
	{
		int MyNumber = pm->GetMyNumber();
		GameEngineDebug::OutPutDebugString("Ŭ���̾�Ʈ(�Խ�Ʈ) Number: " + std::to_string(MyNumber) + "�� �� ������ �Ϸ��߽��ϴ�!!!!\n");
	}
}

void LumiaLevel::ClientMonsterCreation()
{
	// Ŭ���̾�Ʈ(�Խ�Ʈ)������ ȣ���ϴ� �Լ��� 
	// ������ ���� ������ ���� ���� ���͸� �����ϴ� ������ ����
	MonsterInfoManager* InfoManager = MonsterInfoManager::GetInstance();

	// �����Ϸ��� ���� ���� Get
	std::vector<MonsterInfo>& AllMonster = InfoManager->GetAllMonsterListRef();

	int MonsterCount = InfoManager->GetCurMonsterListSize();
	for (int MonsterNum = 0; MonsterNum < MonsterCount; ++MonsterNum)
	{
		Monsters* NewMonster = nullptr;

		MonsterType CurMonsterType = AllMonster[MonsterNum].MonsterType_;
		switch (CurMonsterType)
		{
		case MonsterType::WOLF:
		{
			NewMonster = CreateActor<Wolf>();
			break;
		}
		case MonsterType::BEAR:
		{
			//NewMonster = CreateActor<Bear>();
			break;
		}
		case MonsterType::BAT:
		{
			//NewMonster = CreateActor<Bat>();
			break;
		}
		}

		// ����ó��
		if (nullptr == NewMonster)
		{
			GameEngineDebug::OutPutDebugString(std::to_string(MonsterNum) + "��° ���ʹ� " + "��ϵ������� Ÿ������ �����Ϸ��� �õ��߽��ϴ�!!!!\n");
			continue;
		}

		// NewMonster Setting
		NewMonster->SetMonsterIndex(MonsterNum);
		NewMonster->SetMonsterAreaType(AllMonster[MonsterNum].AreaType_);
		NewMonster->InitalizeSpawnPosition(AllMonster[MonsterNum].SpawnPosition_);

		// 99. ���� ������Ͽ� �߰�
		MonsterActorList_.push_back(NewMonster);
	}

	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	int MyNumber = pm->GetMyNumber();
	GameEngineDebug::OutPutDebugString("Ŭ���̾�Ʈ(�Խ�Ʈ) Number: " + std::to_string(MyNumber) + "�� ���� ������ �Ϸ��߽��ϴ�!!!!\n");
}

void LumiaLevel::ClientCharacterCreation()
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	if (true == CharacterCreationCommand())
	{
		int MyNumber = pm->GetMyNumber();
		GameEngineDebug::OutPutDebugString("Ŭ���̾�Ʈ(�Խ�Ʈ) Number: " + std::to_string(MyNumber) + "�� ĳ���� ������ �Ϸ��߽��ϴ�!!!!\n");
	}
}

void LumiaLevel::MapCreationPacketSend()
{
	MapCreationPacket MapPacket;
	GameServer::GetInstance()->Send(&MapPacket);
}

void LumiaLevel::MonsterCreationPacketSend()
{
	MonsterInfoManager* InfoManager = MonsterInfoManager::GetInstance();

	// ���ͻ��� ��Ŷ ������ ����
	int MonsterCount = static_cast<int>(MonsterActorList_.size());
	for (int MonsterNum = 0; MonsterNum < MonsterCount; ++MonsterNum)
	{
		MonsterInfo NewMonsterInfo = {};

		NewMonsterInfo.Index_ = MonsterNum;
		NewMonsterInfo.AreaType_ = MonsterActorList_[MonsterNum]->GetMonsterAreaType();
		NewMonsterInfo.MonsterType_ = MonsterActorList_[MonsterNum]->GetMonsterType();
		NewMonsterInfo.SpawnPosition_ = MonsterActorList_[MonsterNum]->GetMonsterStateInfo().NestPosition_;

		InfoManager->AddMonsterInfo(NewMonsterInfo);
	}

	// ��Ŷ����
	MonsterCreationPacket MonsterPacket;
	MonsterPacket.SetMonsterInfos(InfoManager->GetAllMonsterListValue());
	GameServer::GetInstance()->Send(&MonsterPacket);
}

void LumiaLevel::CharacterCreationPacketSend()
{
	CharacterCreationPacket CharacterPacket;
	GameServer::GetInstance()->Send(&CharacterPacket);
}

bool LumiaLevel::MapCreationCommand()
{
	// �ʰ��� ���ҽ� �ε�
	MapResourceLoad();

	// �� ����
	CurMap_ = CreateActor<LumiaMap>();

	return true;
}

bool LumiaLevel::MonsterCreationCommand()
{
	// ��Ŷ ������ ���� ���������͸� �����Ƿ� ������ �����͸� �������� ���ͻ���

	// 0. ������ ���� ���� ����
	GameEngineRandom CountRandom;
	//int CreateCount = CountRandom.RandomInt(1, 10);			// �ӽ��ּ�
	int CreateCount = 1;										// �ӽû��(1������ ����)
	for (int MonsterNum = 0; MonsterNum < CreateCount; ++MonsterNum)
	{
		Monsters* NewMonster = nullptr;

		// 1. ������ ���� Ÿ������ ���� ����
		GameEngineRandom TypeRandom;
		//MonsterType CurType = static_cast<MonsterType>(TypeRandom.RandomInt(0, static_cast<int>(MonsterType::MAX) - 1));			// �ӽ��ּ�
		MonsterType CurType = MonsterType::WOLF;																					// �ӽû��
		switch (CurType)
		{
		case MonsterType::WOLF:
		{
			NewMonster = CreateActor<Wolf>();
			break;
		}
		case MonsterType::BEAR:
		{
			//NewMonster = CreateActor<Bear>();
			break;
		}
		case MonsterType::BAT:
		{
			//NewMonster = CreateActor<Bat>();
			break;
		}
		}

		// ����ó��
		if (nullptr == NewMonster)
		{
			GameEngineDebug::OutPutDebugString(std::to_string(MonsterNum) + "��° ���ʹ� " + "��ϵ������� Ÿ������ �����Ϸ��� �õ��߽��ϴ�!!!!\n");
			continue;
		}

		// ���� ���� ������ �ʱ�ȭ & �����Լ� ȣ��
		NewMonster->SetMonsterIndex(MonsterNum);
		NewMonster->InitalizeSpawnAreaType();

		// 99. ���� ������Ͽ� �߰�
		MonsterActorList_.push_back(NewMonster);
	}

	return true;
}

bool LumiaLevel::CharacterCreationCommand()
{
	// ĳ���Ͱ��� ���ҽ� �ε�
	CharacterResourceLoad();

	// Get ���� ���ӿ� ������ �÷��̾� ����
	PlayerInfoManager* PlayerInfo = PlayerInfoManager::GetInstance();

	// ĳ���� ����
	int PlayerCount = static_cast<int>(PlayerInfo->GetPlayerList().size());
	for (int PlayerNum = 0; PlayerNum < PlayerCount; ++PlayerNum)
	{
		// ĳ���� Ÿ�Ժ�
		JobType CurCharacterType = static_cast<JobType>(PlayerInfo->GetPlayerList()[PlayerNum].character_);

		Character* NewCharacter = nullptr;
		switch (CurCharacterType)
		{
		case JobType::YUKI:
		{
			//NewCharacter = CreateActor<Yuki>();
			break;
		}
		case JobType::FIORA:
		{
			//NewCharacter = CreateActor<Fiora>();
			break;
		}
		case JobType::HYUNWOO:
		{
			NewCharacter = CreateActor<Hyunwoo>();
			break;
		}
		case JobType::AYA:
		{
			//NewCharacter = CreateActor<Aya>();
			break;
		}
		case JobType::RIO:
		{
			NewCharacter = CreateActor<Rio>();
			break;
		}
		case JobType::JACKIE:
		{
			//NewCharacter = CreateActor<Jackie>();
			break;
		}
		}

		// ����ó��
		if (nullptr == NewCharacter)
		{
			GameEngineDebug::MsgBoxError("��ϵ������� ĳ���� Ÿ������ �����Ϸ��� �÷��̾ �����մϴ�!!!!\n");
			continue;
		}

		// 
		PlayerInfoManager::GetInstance()->GetPlayerList()[PlayerNum].stat_ = NewCharacter->GetStat();
		NewCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
		NewCharacter->SetIndex(PlayerNum);

		// ������Ͽ� �߰�
		CharacterActorList_.emplace_back(NewCharacter);

		// ���� ȣ��Ʈ or �Խ�Ʈ�� ���� ĳ���� ��Ŀ��
		if (PlayerNum == PlayerInfo->GetMyNumber())
		{
			CharacterActorList_[PlayerNum]->Focus();
		}
	}

	return true;
}

void LumiaLevel::MapResourceLoad()
{
	//========================================== FBX & UserMesh Files
	GameEngineDirectory NaviMeshDir;
	NaviMeshDir.MoveParent("FoxTeam");
	NaviMeshDir / "Resources" / "FBX" / "Map";
	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(NaviMeshDir.PathToPlusFileName("Bg_NaviMesh.fbx")))
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(NaviMeshDir.PathToPlusFileName("Bg_NaviMesh.fbx"));
		Mesh->CreateRenderingBuffer();
	}

	GameEngineDirectory UserMapMeshDir;
	UserMapMeshDir.MoveParent("FoxTeam");
	UserMapMeshDir / "Resources" / "FBX" / "UserMesh" / "Map";
	std::vector<GameEngineFile> vecMapFile = UserMapMeshDir.GetAllFile(".UserMesh");
	for (size_t i = 0; i < vecMapFile.size(); i++)
	{
		if (nullptr == GameEngineFBXMeshManager::GetInst().Find(vecMapFile[i].GetFullPath()))
		{
			GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().LoadUser(vecMapFile[i].GetFullPath());
		}
	}

	//==================== ItemBox
	GameEngineDirectory UserItemBoxMeshDir;
	UserItemBoxMeshDir.MoveParent("FoxTeam");
	UserItemBoxMeshDir / "Resources" / "FBX" / "UserMesh" / "ItemBox";
	std::vector<GameEngineFile> vecItemBoxFile = UserItemBoxMeshDir.GetAllFile(".UserMesh");
	for (size_t i = 0; i < vecItemBoxFile.size(); i++)
	{
		if (nullptr == GameEngineFBXMeshManager::GetInst().Find(vecItemBoxFile[i].GetFullPath()))
		{
			GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().LoadUser(vecItemBoxFile[i].GetFullPath());
		}
	}

	GameEngineDirectory ItemBoxInfoDir;
	ItemBoxInfoDir.MoveParent("FoxTeam");
	ItemBoxInfoDir / "Resources" / "FBX" / "UserMesh" / "ItemBox" / "ItemBoxInfo";

	ItemBoxManager_ = CreateActor<ItemBoxManager>();
	ItemBoxManager_->UserAllLoad(ItemBoxInfoDir);
	ItemBoxManager_->GetTransform()->SetLocalScaling(100.0f);
}

void LumiaLevel::CharacterResourceLoad()
{
	Hyunwoo::LoadResource();
	Rio::LoadResource();
}

void LumiaLevel::CreateBasicActor()
{
	// �ΰ��� ���콺 ����
	if (nullptr == MousePointer::InGameMouse)
	{
		MousePointer::InGameMouse = CreateActor<MousePointer>();
		MousePointer::InGameMouse->GetTransform()->SetLocalPosition(GameEngineInput::GetInst().GetMouse3DPos());
	}

	// Create SkysphereActor
	SKySphereActor* Actor = CreateActor<SKySphereActor>();

	// Create Default Light
	LightActor* DefaultLight = CreateActor<LightActor>();
	DefaultLight->GetLight()->SetDiffusePower(1.f);
	DefaultLight->GetLight()->SetAmbientPower(10.f);
	DefaultLight->GetLight()->SetSpacularLightPow(10.f);
}

void LumiaLevel::CreateLevelInput()
{
	if (false == GameEngineInput::GetInst().IsKey("A"))
	{
		GameEngineInput::GetInst().CreateKey("A", 'A');
	}

	if (false == GameEngineInput::GetInst().IsKey("D"))
	{
		GameEngineInput::GetInst().CreateKey("D", 'D');
	}

	if (false == GameEngineInput::GetInst().IsKey("W"))
	{
		GameEngineInput::GetInst().CreateKey("W", 'W');
	}

	if (false == GameEngineInput::GetInst().IsKey("S"))
	{
		GameEngineInput::GetInst().CreateKey("S", 'S');
	}

	if (false == GameEngineInput::GetInst().IsKey("Q"))
	{
		GameEngineInput::GetInst().CreateKey("Q", 'Q');
	}

	if (false == GameEngineInput::GetInst().IsKey("E"))
	{
		GameEngineInput::GetInst().CreateKey("E", 'E');
	}

	if (false == GameEngineInput::GetInst().IsKey("O"))
	{
		GameEngineInput::GetInst().CreateKey("O", 'O');
	}

	if (false == GameEngineInput::GetInst().IsKey("LButton"))
	{
		GameEngineInput::GetInst().CreateKey("LButton", VK_LBUTTON);
	}

	if (false == GameEngineInput::GetInst().IsKey("RButton"))
	{
		GameEngineInput::GetInst().CreateKey("RButton", VK_RBUTTON);
	}

	if (false == GameEngineInput::GetInst().IsKey("Esc"))
	{
		GameEngineInput::GetInst().CreateKey("Esc", VK_ESCAPE);
	}

	// Test Key
	if (false == GameEngineInput::GetInst().IsKey("1"))
	{
		GameEngineInput::GetInst().CreateKey("1", '1');
	}

	if (false == GameEngineInput::GetInst().IsKey("2"))
	{
		GameEngineInput::GetInst().CreateKey("2", '2');
	}

	if (false == GameEngineInput::GetInst().IsKey("3"))
	{
		GameEngineInput::GetInst().CreateKey("3", '3');
	}
}

void LumiaLevel::AddSocketHandle()
{
	GameServer* server = GameServer::GetInstance();
	GameClient* client = GameClient::GetInstance();

	if (true == server->IsOpened())
	{
		server->AddPacketHandler(ePacketID::CharMovementPacket, new CharMovementPacket);
		server->AddPacketHandler(ePacketID::CharAnimationPacket, new CharAnimationPacket);
		server->AddPacketHandler(ePacketID::CharStatPacket, new CharStatPacket);
	}

	if (true == client->IsConnected())
	{
		client->AddPacketHandler(ePacketID::CharMovementPacket, new CharMovementPacket);
		client->AddPacketHandler(ePacketID::CharAnimationPacket, new CharAnimationPacket);
		client->AddPacketHandler(ePacketID::CharStatPacket, new CharStatPacket);
	}
}

void LumiaLevel::InitIMGUIWindow()
{
	if (nullptr != GameEngineGUI::GetInst()->FindGUIWindow("RenderWindow"))
	{
		GameEngineRenderWindow* Window = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
		Window->ClaerRenderTarget();
		float4 Size = { 128, 72 };
		Window->PushRenderTarget("���� ī�޶� Ÿ��", GetMainCamera()->GetCameraRenderTarget(), Size);
		Window->PushRenderTarget("UI ī�޶� Ÿ��", GetUICamera()->GetCameraRenderTarget(), Size);
		Window->PushRenderTarget("���� ī�޶� G-Buffer", GetMainCamera()->GetCameraDeferredGBufferTarget(), Size);
		Window->PushRenderTarget("���� ī�޶� ���۵� ����Ʈ", GetMainCamera()->GetCameraDeferredLightTarget(), Size);
		Window->PushRenderTarget("���� ī�޶� ���۵� Ÿ��", GetMainCamera()->GetCameraDeferredTarget(), Size);
	}

	DebugAndControlWindow_ = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");


	ItemBoxWindow* Window = nullptr;
	if (nullptr == GameEngineGUI::GetInst()->FindGUIWindow("ItemBoxWindow"))
	{
		Window = GameEngineGUI::GetInst()->CreateGUIWindow<ItemBoxWindow>("ItemBoxWindow");
	}

	if (nullptr != Window && nullptr != ItemBoxManager_)
	{
		Window->ItemBoxManager_ = ItemBoxManager_;
	}
}

void LumiaLevel::CameraAdjustment()
{
	GetMainCameraActor()->GetCamera()->SetFov(50.f);
	GetMainCameraActor()->FreeCameraModeSwitch();
	GetMainCameraActor()->GetTransform()->SetWorldPosition({ 0.0f, 100.f, -200.f });
}

void LumiaLevel::CharacterStateUpdatePacketSend()
{
	GameServer* ServerSocket = GameServer::GetInstance();
	GameClient* ClientSocket = GameClient::GetInstance();

	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	// ĳ���� �̵����� ��Ŷ
	CharMovementPacket MovePacket;
	MovePacket.SetTargetIndex(pm->GetMyNumber());
	MovePacket.SetPos(CharacterActorList_[pm->GetMyNumber()]->GetTransform()->GetLocalPosition());
	MovePacket.SetDirection(CharacterActorList_[pm->GetMyNumber()]->GetTransform()->GetLocalRotation());

	if (true == ServerSocket->IsOpened())
	{
		ServerSocket->Send(&MovePacket);
	}
	else if (true == ClientSocket->IsConnected())
	{
		ClientSocket->Send(&MovePacket);
	}

	// ĳ���� �ִϸ��̼ǰ��� ��Ŷ
	CharAnimationPacket AnimPacket;
	AnimPacket.SetTargetIndex(pm->GetMyNumber());
	AnimPacket.SetAnimation(CharacterActorList_[pm->GetMyNumber()]->GetCurAnimation());

	if (true == ServerSocket->IsOpened())
	{
		ServerSocket->Send(&AnimPacket);
	}
	else if (true == ClientSocket->IsConnected())
	{
		ClientSocket->Send(&AnimPacket);
	}

	// ĳ���� ���°��� ��Ŷ
	CharStatPacket StatePacket;
	StatePacket.SetTargetIndex(pm->GetMyNumber());
	StatePacket.SetStat(*(pm->GetMyPlayer().stat_));

	if (true == ServerSocket->IsOpened())
	{
		ServerSocket->Send(&StatePacket);
	}
	else if (true == ClientSocket->IsConnected())
	{
		ClientSocket->Send(&StatePacket);
	}

	// ...
}

void LumiaLevel::CharactersTransformUpdate()
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

#ifdef SERVER
	for (int i = 0; i < pm->GetPlayerList().size(); i++)
	{
		// ���� ��Ŷ�� ������ ���ŵ� playerinfo ���� ����
		// ������ ĳ���� ���͵��� ��ġ ȸ�� �ִϸ��̼� ���� �������ִ� �ڵ����Դϴ�.
		if (i == pm->GetMyNumber())
		{
			continue;
		}

		CharacterActorList_[i]->GetTransform()->SetLocalPosition(pm->GetPlayerList()[i].curPos_);
		CharacterActorList_[i]->ChangeAnimation(pm->GetPlayerList()[i].curAnimation_);
		CharacterActorList_[i]->GetTransform()->SetWorldRotationDegree(pm->GetPlayerList()[i].curDir_);
	}
#endif
}

void LumiaLevel::MonsterStateUpdatePacketSend()
{
	GameServer* ServerSocket = GameServer::GetInstance();
	GameClient* ClientSocket = GameClient::GetInstance();

	// ... 

}

void LumiaLevel::MonstersTransformUpdate()
{


	// ...


}

void LumiaLevel::DebugWindowUpdate()
{
	if (nullptr != DebugAndControlWindow_ && nullptr != MousePointer::InGameMouse)
	{
		// InGameMouse Debug Value
		float4 position = MousePointer::InGameMouse->GetIntersectionYAxisPlane(0, 50000.f);
		DebugAndControlWindow_->AddText("X : " + std::to_string(position.x));
		DebugAndControlWindow_->AddText("Z : " + std::to_string(position.z));

		// Player Debug Value
		float4 playerPosition = CharacterActorList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->GetTransform()->GetWorldPosition();
		float4 camPos = GetMainCameraActor()->GetTransform()->GetWorldPosition();
		float4 playerToCamera = camPos - playerPosition;
		float4 cameraRotation = GetMainCameraActor()->GetTransform()->GetLocalRotation();
		DebugAndControlWindow_->AddText("Player to Camera vector : " + std::to_string(playerToCamera.x) + ", " + std::to_string(playerToCamera.y) + ", " + std::to_string(playerToCamera.z));
		DebugAndControlWindow_->AddText("CameraRotation : " + std::to_string(cameraRotation.x) + ", " + std::to_string(cameraRotation.y) + ", " + std::to_string(cameraRotation.z));

		for (int i = 0; i < CharacterActorList_.size(); i++)
		{
			DebugAndControlWindow_->AddText("Player " + std::to_string(i) + "curHP(Local) : " + std::to_string(CharacterActorList_[i]->GetStat()->HP));
		}

		// Monster Debug Value



	}
}

LumiaLevel::LumiaLevel()
	: CurMap_(nullptr)
	, ItemBoxManager_(nullptr)
	, DebugAndControlWindow_(nullptr)
{
}

LumiaLevel::~LumiaLevel()
{
}

void LumiaLevel::LevelStart()
{
}

void LumiaLevel::LevelUpdate(float _DeltaTime)
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	// ���� ��Ŷ ó��
	if (true == GameServer::GetInstance()->IsOpened())
	{
		GameServer::GetInstance()->ProcessPacket();
	}

	// Ŭ���̾�Ʈ ��Ŷ ó��
	if (true == GameClient::GetInstance()->IsConnected())
	{
		GameClient::GetInstance()->ProcessPacket();
	}

	// ĳ���� ���� ������Ʈ ��Ŷ ����ó��
	CharacterStateUpdatePacketSend();

	// ĳ���� ����� Ʈ������ ������ ����
	CharactersTransformUpdate();

	// ���� ���� ������Ʈ ��Ŷ ����ó��
	MonsterStateUpdatePacketSend();

	// ���� ����� Ʈ������ ������ ����
	MonstersTransformUpdate();

	// Switching FreeCamMode
	if (true == GameEngineInput::GetInst().Down("O"))
	{
		GetMainCameraActor()->FreeCameraModeSwitch();
	}

	// MainCamera Adjustment
	if (false == GetMainCameraActor()->IsFreeCameraMode())
	{
		float4 playerPosition = CharacterActorList_[pm->GetMyNumber()]->GetTransform()->GetWorldPosition();
		GetMainCameraActor()->GetTransform()->SetWorldPosition(playerPosition + float4(400.f, 1280.f, -600.f));
		GetMainCameraActor()->GetTransform()->SetLocalRotationDegree({ 60.f, -35.f, 0.0f });
	}

	// Debug Window Update
	DebugWindowUpdate();
}

void LumiaLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
}

void LumiaLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	// �⺻ ���� ����
	CreateBasicActor();

	// Create LumiaLevel Input
	CreateLevelInput();

	// ��Ʈ��ũ �ڵ鷯 ���
	AddSocketHandle();

	// IMGUI Window Find & Setting
	InitIMGUIWindow();

	// MainCamera Adjustment
	CameraAdjustment();
}
