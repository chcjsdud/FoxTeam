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
#include "CharCrowdControlPacket.h"
#include "CreationCommandPacket.h"
#include "LoadingEndPacket.h"
#include "PacketCreateProjectile.h"
#include "PacketSoundPlay.h"
#include "MonsterStateChangePacket.h"
#include "GameTimeSyncPacket.h"

#include "LoadingLevel.h"

//======================== Controller
#include "GameTimeController.h"

//======================== Map
#include "LumiaMap.h"
#include "ItemBoxManager.h"

//======================== Characters
#include "Character.h"
#include "Dummy.h"
#include "Rio.h"
#include "Hyunwoo.h"

//======================== Monsters
#include "Monsters.h"
#include "Wolf.h"

const char* enum_str[static_cast<int>(Location::MAX)] = {
		"DOCK",				// �ױ�
		"POND",				// ����
		"BEACH",			// �𷡻���
		"UPTOWN",			// ��� ���ð�
		"ALLEY",			// ����
		"HOTEL",			// ȣ��
		"AVENUE",			// ��ȭ��
		"HOSPITAL",			// ����
		"TEMPLE",			// ��
		"ARCHERY",			// �����
		"CEMETERY",			// ����
		"FOREST",			// ��
		"FACTORY",			// ����
		"CHAPEL",			// ����
		"SCHOOL",			// �б�
		"RESERCH_CENTRE",	// ������
		"ESCAPE_DOCK",		// ������ ��â
};

void LumiaLevel::HostCreateCommand()
{
	// ����(ȣ��Ʈ) ������ �۾��Ҵ�
	GameEngineCore::ThreadQueue.JobPost([&]
		{
			// ����(ȣ��Ʈ) ���� ����� �����Ϸ��� ���� ���� ����
			CreateMonsterInfo();

			// ����(ȣ��Ʈ)�� ���ҽ� �ε� �� ���ͻ��� ����
			HostAllCreationCommand();

			// Loading Thread End Flag On
			LoadingLevel::ThreadLoadingEnd = true;
		}
	);
}

void LumiaLevel::GuestCreateCommand()
{
	int Count = GameEngineCore::ThreadQueue.GetWorkingCount();

	// Ŭ���̾�Ʈ(�Խ�Ʈ)�� ���ҽ� �ε� �� ���ͻ��� ����
	GameEngineCore::ThreadQueue.JobPost([&]
		{
			GuestAllCreationCommand();

			// Loading Thread End Flag On
			LoadingLevel::ThreadLoadingEnd = true;
		}
	);
}

void LumiaLevel::HostMonsterFirstAppear(MonsterType _MonsterType)
{
	// ����ó��
	if (MonsterType::NONE == _MonsterType || MonsterType::MAX == _MonsterType)
	{
		GameEngineDebug::MsgBoxError("�߸��� Ÿ���� ���͸� �����Ű���� �õ��߽��ϴ�!!!!!");
		return;
	}

	// �ش� ����Ÿ���� ���ʹ� �ϰ������� ù������·� ��ȯ
	int MonsterCount = static_cast<int>(MonsterActorByTypeList_[static_cast<int>(_MonsterType)].size());
	for (int MonsterNum = 0; MonsterNum < MonsterCount; ++MonsterNum)
	{
		MonsterActorByTypeList_[static_cast<int>(_MonsterType)][MonsterNum]->On();
		MonsterActorByTypeList_[static_cast<int>(_MonsterType)][MonsterNum]->ChangeAnimationAndState(MonsterStateType::APPEAR);
	}
}

void LumiaLevel::GuestMonsterFirstAppear(MonsterType _MonsterType)
{
	// ����ó��
	if (MonsterType::NONE == _MonsterType || MonsterType::MAX == _MonsterType)
	{
		GameEngineDebug::MsgBoxError("�߸��� Ÿ���� ���͸� �����Ű���� �õ��߽��ϴ�!!!!!");
		return;
	}

	// �ش� ����Ÿ���� ���ʹ� �ϰ������� ù������·� ��ȯ
	int MonsterCount = static_cast<int>(MonsterActorByTypeList_[static_cast<int>(_MonsterType)].size());
	for (int MonsterNum = 0; MonsterNum < MonsterCount; ++MonsterNum)
	{
		MonsterActorByTypeList_[static_cast<int>(_MonsterType)][MonsterNum]->On();
		MonsterActorByTypeList_[static_cast<int>(_MonsterType)][MonsterNum]->ChangeAnimationAndState(MonsterStateType::APPEAR);
	}
}

void LumiaLevel::CreateMonsterInfo()
{
	if (true == MonsterInfoManager::GetInstance()->CreatMonsterInfomation())
	{
		GameEngineDebug::OutPutDebugString("����(ȣ��Ʈ)���� ����ʿ� ��ġ�Ϸ��� ���� ���� ������ �Ϸ��߽��ϴ�!!!!");
	}
}

void LumiaLevel::HostAllCreationCommand()
{
	// Host(����) �������� ���
	MapCreationCommand();
	MonsterCreationCommand();
	CharacterCreationCommand();
}

void LumiaLevel::GuestAllCreationCommand()
{
	// Guest(Ŭ���̾�Ʈ) �������� ���
	MapCreationCommand();
	MonsterCreationCommand();
	CharacterCreationCommand();
}

void LumiaLevel::MapCreationCommand()
{
	// �ʰ��� ���ҽ� �ε�
	MapResourceLoad();

	// �� ����
	CurMap_ = CreateActor<LumiaMap>();

//#ifdef _DEBUG
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	GameEngineDebug::OutPutDebugString(std::to_string(pm->GetMyNumber()) + "��° �÷��̾��� �ʻ����� �Ϸ��߽��ϴ�\n");
//#endif // _DEBUG
}

void LumiaLevel::MonsterCreationCommand()
{
	// ������������� �����Ͽ� ���� ��������
	GameTimeController* gm = GameTimeController::GetInstance();
	MonsterInfoManager* mm = MonsterInfoManager::GetInstance();

	int AllMonsterCount = mm->GetCurMonsterListSize();
	for (int MonsterNum = 0; MonsterNum < AllMonsterCount; ++MonsterNum)
	{
		Monsters* NewMonster = nullptr;

		// ���� ���� �������� Get
		MonsterInfo CurMonsterInfo = mm->GetAllMonsterListRef()[MonsterNum];

		// ���� ������ Ÿ��üũ
		MonsterType CurMonsterType = CurMonsterInfo.MonsterType_;
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
			case MonsterType::DOG:
			{
				//NewMonster = CreateActor<Dog>();
				break;
			}
			case MonsterType::CHICKEN:
			{
				//NewMonster = CreateActor<Chicken>();
				break;
			}
			case MonsterType::BOAR:
			{
				//NewMonster = CreateActor<Boar>();
				break;
			}
		}

		// ����ó��
		if (nullptr == NewMonster)
		{
			//GameEngineDebug::OutPutDebugString("�߸��� Ÿ���� ���ͻ����� �õ��߽��ϴ�!!!! ����Ÿ��: " + std::to_string(static_cast<int>(CurMonsterType)) + " \n");
			continue;
		}

		// ������ ���� �������� ����
		NewMonster->SetMonsterIndex(CurMonsterInfo.Index_);
		NewMonster->SetMonsterAreaType(CurMonsterInfo.RegionType_);

		// Ʈ������������ ����
		NewMonster->InitalizeSpawnPosition(CurMonsterInfo.SpawnPosition_);

		// GameController�� ���Ͽ� ù���������� Off ���·� ����
		// -> GameController�� ù�������� ������ On���·� ��ȯ�Ǹ� Appear State�� ����
		NewMonster->Off();

		// ������� �߰�
		MonsterActorList_.push_back(NewMonster);

		// �ϰ�ó���� ���͸�Ͽ� �߰�
		MonsterActorByTypeList_[static_cast<int>(CurMonsterType)].push_back(NewMonster);
	}

//#ifdef _DEBUG
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	GameEngineDebug::OutPutDebugString(std::to_string(pm->GetMyNumber()) + "��° �÷��̾��� ���ͻ����� �Ϸ��߽��ϴ�\n");
//#endif // _DEBUG
}

void LumiaLevel::CharacterCreationCommand()
{
	// ĳ���Ͱ��� ���ҽ� �ε�
	CharacterResourceLoad();

	// Get ���� ���ӿ� ������ �÷��̾� ����
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	// ĳ���� ����
	int PlayerCount = static_cast<int>(pm->GetPlayerList().size());
	for (int PlayerNum = 0; PlayerNum < PlayerCount; ++PlayerNum)
	{
		// ĳ���� Ÿ�Ժ�
		JobType CurCharacterType = static_cast<JobType>(pm->GetPlayerList()[PlayerNum].character_);

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
		case JobType::DUMMY:
		{
			NewCharacter = CreateActor<Rio>();
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
		PlayerInfoManager::GetInstance()->GetPlayerList()[PlayerNum].curPos_ = float4(-2500.f, 0.0f, 10000.f);
		NewCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
		NewCharacter->SetIndex(PlayerNum);
		// ������Ͽ� �߰�
		CharacterActorList_.emplace_back(NewCharacter);

		// ���� ȣ��Ʈ or �Խ�Ʈ�� ���� ĳ���� ��Ŀ��
		if (PlayerNum == pm->GetMyNumber())
		{
			CharacterActorList_[PlayerNum]->Focus();
			PlayerInfoManager::GetInstance()->SetMainCharacter(NewCharacter);
		}
	}

//#ifdef _DEBUG
	GameEngineDebug::OutPutDebugString(std::to_string(pm->GetMyNumber()) + "��° �÷��̾��� ĳ���ͻ����� �Ϸ��߽��ϴ�\n");
//#endif // _DEBUG
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

	GameEngineDirectory MeshDir;
	MeshDir.MoveParent("FoxTeam");
	MeshDir / "Resources" / "FBX" / "YSJ";

	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(MeshDir.PathToPlusFileName("CharacterSpawnPoints.fbx")))
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(MeshDir.PathToPlusFileName("CharacterSpawnPoints.fbx"));
		std::vector<FbxNodeData> nodeDatas = Mesh->GetAllNodeData();
		for (const auto& data : nodeDatas)
		{
			std::vector<float4> vecTrans;

			std::string UpperName = GameEngineString::toupper(data.name);
			for (const auto& areaName : enum_str)
			{

				const auto& iter = characterSpawnPoints_.find(areaName);

				if (characterSpawnPoints_.end() != iter)
				{
					(*iter).second;
				}

				if (std::string::npos != UpperName.find(areaName))
				{
					characterSpawnPoints_.insert(std::pair(areaName, vecTrans));
				}
			}
			
		}
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
		server->AddPacketHandler(ePacketID::CharCrowdControlPacket, new CharCrowdControlPacket);
		server->AddPacketHandler(ePacketID::PacketCreateProjectile, new PacketCreateProjectile);
		server->AddPacketHandler(ePacketID::PacketSoundPlay, new PacketSoundPlay);
		server->AddPacketHandler(ePacketID::MonsterStatePacket, new MonsterStateChangePacket);
		server->AddPacketHandler(ePacketID::TimeSyncPacket, new GameTimeSyncPacket);
	}

	if (true == client->IsConnected())
	{
		client->AddPacketHandler(ePacketID::CharMovementPacket, new CharMovementPacket);
		client->AddPacketHandler(ePacketID::CharAnimationPacket, new CharAnimationPacket);
		client->AddPacketHandler(ePacketID::CharStatPacket, new CharStatPacket);
		client->AddPacketHandler(ePacketID::CharCrowdControlPacket, new CharCrowdControlPacket);
		client->AddPacketHandler(ePacketID::PacketCreateProjectile, new PacketCreateProjectile);
		client->AddPacketHandler(ePacketID::PacketSoundPlay, new PacketSoundPlay);
		client->AddPacketHandler(ePacketID::MonsterStatePacket, new MonsterStateChangePacket);
		client->AddPacketHandler(ePacketID::TimeSyncPacket, new GameTimeSyncPacket);
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

void LumiaLevel::GameTimeUpdatePacketSend()
{
	// ��������ð� ����ȭ�� ���� ��Ŷ����(��������)
	GameTimeSyncPacket SendPacket;
	SendPacket.SetCurrentDay(GameTimeController::GetInstance()->GetCurrentDay());
	SendPacket.SetDayAndNightType(GameTimeController::GetInstance()->GetCurrentDayType());
	SendPacket.SetGameTime(GameTimeController::GetInstance()->GetCurrentGameTimeToSec());
	GameServer::GetInstance()->Send(&SendPacket);
}

void LumiaLevel::CharacterStateUpdatePacketSend()
{
	GameServer* ServerSocket = GameServer::GetInstance();
	GameClient* ClientSocket = GameClient::GetInstance();

	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	if (-1 != pm->GetMyNumber() && 0 < static_cast<int>(CharacterActorList_.size()))
	{
		Character* currentCharacter = CharacterActorList_[pm->GetMyNumber()];
		// ĳ���� �̵����� ��Ŷ
		CharMovementPacket MovePacket;
		MovePacket.SetTargetIndex(pm->GetMyNumber());
		MovePacket.SetPos(currentCharacter->GetTransform()->GetLocalPosition());
		MovePacket.SetDirection(currentCharacter->GetTransform()->GetLocalRotation());

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
		AnimPacket.SetAnimationName(currentCharacter->GetCurAnimation());
		AnimPacket.SetOverrideAnimation(currentCharacter->GetOverrideAnimationName(), currentCharacter->GetOverrideAnimationBoneName());

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
}

void LumiaLevel::CharactersTransformUpdate()
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	if (0 < static_cast<int>(CharacterActorList_.size()))
	{
		for (int i = 0; i < pm->GetPlayerList().size(); i++)
		{
			// ���� ��Ŷ�� ������ ���ŵ� playerinfo ���� ����
			// ������ ĳ���� ���͵��� ��ġ ȸ�� �ִϸ��̼� ���� �������ִ� �ڵ����Դϴ�.
			if (i == pm->GetMyNumber())
			{
				continue;
			}

			PlayerInfo& currentInfo = pm->GetPlayerList()[i];

			CharacterActorList_[i]->GetTransform()->SetLocalPosition(currentInfo.curPos_);
			CharacterActorList_[i]->ChangeAnimation(currentInfo.curAnimation_);
			CharacterActorList_[i]->ChangeOverrideAnimation(currentInfo.overrideAnimationName_, currentInfo.overrideAnimationBoneName_);
			CharacterActorList_[i]->GetTransform()->SetWorldRotationDegree(pm->GetPlayerList()[i].curRotation_);
		}
	}
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
	if (nullptr != DebugAndControlWindow_ && nullptr != MousePointer::InGameMouse && 0 < static_cast<int>(CharacterActorList_.size()))
	{
		GameTimeController* gm = GameTimeController::GetInstance();
		PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

		// Game Time Debug Value
		
		// �������� ��/��
		int CurDay = gm->GetCurrentDay();
		DayAndNightType CurType = gm->GetCurrentDayType();
		if (DayAndNightType::DAY == CurType)
		{
			DebugAndControlWindow_->AddText(std::to_string(CurDay) + " DAY");
		}
		else if (DayAndNightType::NIGHT == CurType)
		{
			DebugAndControlWindow_->AddText(std::to_string(CurDay) + " NIGHT");
		}

		float CurrentTimeSec = gm->GetCurrentGameTimeToSec();
		tm CurrentTimeMin = gm->GetCurrentGameTimeToMinute();
		DebugAndControlWindow_->AddText(std::to_string(CurrentTimeMin.tm_min) + " MIN " + std::to_string(CurrentTimeMin.tm_sec) + " SEC");
		DebugAndControlWindow_->AddText(std::to_string(CurrentTimeSec) + " SEC");

		// InGameMouse Debug Value
		float4 position = MousePointer::InGameMouse->GetIntersectionYAxisPlane(0, 50000.f);
		DebugAndControlWindow_->AddText("X : " + std::to_string(position.x));
		DebugAndControlWindow_->AddText("Z : " + std::to_string(position.z));

		// Player Debug Value
		float4 playerPosition = CharacterActorList_[pm->GetMyNumber()]->GetTransform()->GetWorldPosition();
		float4 camPos = GetMainCameraActor()->GetTransform()->GetWorldPosition();
		float4 playerToCamera = camPos - playerPosition;
		float4 cameraRotation = GetMainCameraActor()->GetTransform()->GetLocalRotation();
		DebugAndControlWindow_->AddText("Player to Camera vector : " + std::to_string(playerToCamera.x) + ", " + std::to_string(playerToCamera.y) + ", " + std::to_string(playerToCamera.z));
		DebugAndControlWindow_->AddText("CameraRotation : " + std::to_string(cameraRotation.x) + ", " + std::to_string(cameraRotation.y) + ", " + std::to_string(cameraRotation.z));

		for (int i = 0; i < CharacterActorList_.size(); i++)
		{
			DebugAndControlWindow_->AddText("Player " + std::to_string(i) + "curHP(Local) : " + std::to_string(CharacterActorList_[i]->GetStat()->HP));
			DebugAndControlWindow_->AddText("Player " + std::to_string(i) + "curHP(Server) : " + std::to_string(pm->GetPlayerList()[i].stat_->HP));
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
	// Create LumiaLevel Input
	CreateLevelInput();
}

void LumiaLevel::LevelUpdate(float _DeltaTime)
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	MonsterInfoManager* mm = MonsterInfoManager::GetInstance();

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

	// ����������Ʈ�ѷ�(������, ��/����ȯ, ����ù����, ... )
	// ��, �����϶��� ó���ϸ� ����(ȣ��Ʈ)�� ��������� ����(�Ѱ����� �����ϱ�����)
	if (true == GameServer::GetInstance()->IsOpened())
	{
		// ������, ��/����ȯ, ����ù������ ���� Ÿ�Ӱ���
		GameTimeController::GetInstance()->HostUpdate(_DeltaTime);

		// ���ӽð� �ǽð� ��Ŷ(����-Ŭ�� ����ȭ)
		GameTimeUpdatePacketSend();
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
	if (false == GetMainCameraActor()->IsFreeCameraMode() && -1 != pm->GetMyNumber())
	{
		float4 playerPosition = CharacterActorList_[pm->GetMyNumber()]->GetTransform()->GetWorldPosition();
		GetMainCameraActor()->GetTransform()->SetWorldPosition(playerPosition + float4(400.f, 1280.f, -600.f));
		GetMainCameraActor()->GetTransform()->SetLocalRotationDegree({ 60.f, -35.f, 0.0f });
	}

	//// 221011 SJH TEST : ���� Ȯ�������� ���� ��Ŀ��
	//if (false == GetMainCameraActor()->IsFreeCameraMode() && 0 != static_cast<int>(MonsterActorList_.size()))
	//{
	//	float4 MonsterPosition = MonsterActorList_[0]->GetTransform()->GetWorldPosition();
	//	GetMainCameraActor()->GetTransform()->SetWorldPosition(MonsterPosition + float4(400.f, 1280.f, -600.f));
	//	GetMainCameraActor()->GetTransform()->SetLocalRotationDegree({ 60.f, -35.f, 0.0f });
	//}

	// Debug Window Update
	DebugWindowUpdate();
}

void LumiaLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
	// 220929 ADD SJH : �׽�Ʈ��(���Ļ�������)
	// ������ ������ ���߰ų� Ŭ���̾�Ʈ�� ������ ��������������¿��� LevelControlWindow������ ������ �����̵��� ���
	if (false == GameServer::GetInstance()->IsOpened() && false == GameClient::GetInstance()->IsConnected())
	{
		this->ClearAll();
		Test_releaseRenderWindow();
		Test_releaseResource();

		MainCameraActor_ = CreateActor<CameraActor>();
		UICameraActor_ = CreateActor<CameraActor>();
		UICameraActor_->GetCamera()->SetProjectionMode(ProjectionMode::Orthographic);
	}
}

void LumiaLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	// GameController Initalize
	if (true == GameServer::GetInstance()->IsOpened())
	{
		GameTimeController::GetInstance()->HostInitialize();
	}

	// 220929 ADD SJH : �׽�Ʈ��(���Ļ�������)
	// ������ ������ ���߰ų� Ŭ���̾�Ʈ�� ������ ��������������¿��� LevelControlWindow������ ������ �����̵��� ���
	if (false == GameServer::GetInstance()->IsOpened() && false == GameClient::GetInstance()->IsConnected())
	{
		Test_loadResource();
		Test_initRenderWindow();
		Test_createActor();
		Test_adjustCamera();
		Test_serverCheck();

		ItemBoxWindow* Window = nullptr;

		if (nullptr == GameEngineGUI::GetInst()->FindGUIWindow("ItemBoxWindow"))
		{
			Window = GameEngineGUI::GetInst()->CreateGUIWindow<ItemBoxWindow>("ItemBoxWindow");
		}

		if (nullptr != Window)
		{
			Window->ItemBoxManager_ = ItemBoxManager_;
		}

		// 
	}
	else
	{
		// �⺻ ���� ����
		CreateBasicActor();

		// ��Ʈ��ũ �ڵ鷯 ���
		AddSocketHandle();

		// IMGUI Window Find & Setting
		InitIMGUIWindow();

		// MainCamera Adjustment
		CameraAdjustment();
	}
}

#pragma region �׽�Ʈ���Լ�(���Ļ�������)
// LevelControlWindow������ ���� �����̵��Ѱ�� ����ϴ� �Լ�
// ���� ������ ���� �ش� ������ ü�����Ѱ��

void LumiaLevel::Test_loadResource()
{
	// ��, �׺���̼� �޽�, �����۹ڽ�
	{
		GameEngineDirectory tempDir;

		tempDir.MoveParent("FoxTeam");
		tempDir / "Resources" / "FBX" / "Map";

		if (nullptr == GameEngineFBXMeshManager::GetInst().Find(tempDir.PathToPlusFileName("Bg_NaviMesh.fbx")))
		{
			GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(tempDir.PathToPlusFileName("Bg_NaviMesh.fbx"));
			Mesh->CreateRenderingBuffer();
		}

		tempDir.MoveParent("FBX");
		tempDir / "UserMesh" / "Map";

		std::vector<GameEngineFile> vecFile = tempDir.GetAllFile(".UserMesh");

		for (size_t i = 0; i < vecFile.size(); i++)
		{
			if (nullptr == GameEngineFBXMeshManager::GetInst().Find(vecFile[i].GetFullPath()))
			{
				GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().LoadUser(vecFile[i].GetFullPath());
			}
		}

		tempDir.MoveParent("UserMesh");
		tempDir.MoveChild("ItemBox");

		vecFile = tempDir.GetAllFile(".UserMesh");

		for (size_t i = 0; i < vecFile.size(); i++)
		{
			if (nullptr == GameEngineFBXMeshManager::GetInst().Find(vecFile[i].GetFullPath()))
			{
				GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().LoadUser(vecFile[i].GetFullPath());
			}
		}
	}

	// ĳ����
	Rio::LoadResource();
	Hyunwoo::LoadResource();
}

void LumiaLevel::Test_initRenderWindow()
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
}

void LumiaLevel::Test_createActor()
{
	// �ΰ��� ���콺 ����
	if (nullptr == MousePointer::InGameMouse)
	{
		MousePointer::InGameMouse = CreateActor<MousePointer>();
		MousePointer::InGameMouse->GetTransform()->SetLocalPosition(GameEngineInput::GetInst().GetMouse3DPos());
	}

	{
		GameEngineDirectory tempDir;

		tempDir.MoveParent("FoxTeam");
		tempDir / "Resources" / "FBX" / "UserMesh" / "ItemBox" / "ItemBoxInfo";

		ItemBoxManager_ = CreateActor<ItemBoxManager>();
		ItemBoxManager_->UserAllLoad(tempDir);
		ItemBoxManager_->GetTransform()->SetLocalScaling(100.0f);
	}

	CurMap_ = CreateActor<LumiaMap>();

	SKySphereActor* Actor = CreateActor<SKySphereActor>();


	{
		LightActor* Actor;

		Actor = CreateActor<LightActor>();
		Actor->GetLight()->SetDiffusePower(1.f);
		Actor->GetLight()->SetAmbientPower(10.f);
		Actor->GetLight()->SetSpacularLightPow(10.f);
	}

	Test_GenerateCharactor();
}

void LumiaLevel::Test_adjustCamera()
{
	GetMainCameraActor()->GetCamera()->SetFov(50.f);
	GetMainCameraActor()->FreeCameraModeSwitch();
	GetMainCameraActor()->GetTransform()->SetWorldPosition({ 0.0f, 100.f, -200.f });
}

void LumiaLevel::Test_serverCheck()
{
	if (true == GameServer::GetInstance()->IsOpened())
	{
		GameServer* server = GameServer::GetInstance();
		server->AddPacketHandler(ePacketID::CharMovementPacket, new CharMovementPacket);
		server->AddPacketHandler(ePacketID::CharAnimationPacket, new CharAnimationPacket);
		server->AddPacketHandler(ePacketID::CharStatPacket, new CharStatPacket);
	}
	else if (true == GameClient::GetInstance()->IsConnected())
	{
		GameClient* client = GameClient::GetInstance();
		client->AddPacketHandler(ePacketID::CharMovementPacket, new CharMovementPacket);
		client->AddPacketHandler(ePacketID::CharAnimationPacket, new CharAnimationPacket);
		client->AddPacketHandler(ePacketID::CharStatPacket, new CharStatPacket);
	}
}

void LumiaLevel::Test_releaseRenderWindow()
{
	if (nullptr != GameEngineGUI::GetInst()->FindGUIWindow("RenderWindow"))
	{
		GameEngineRenderWindow* Window = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
		Window->ClaerRenderTarget();
	}
}

void LumiaLevel::Test_releaseResource()
{
	// loadResource���� �ε��� ���ҽ� ����

	// ��, �׺���̼� �޽�, �����۹ڽ�
	GameEngineFBXMeshManager::GetInst().Delete("Bg_NaviMesh.fbx");
	{
		GameEngineDirectory tempDir;
		tempDir.MoveParent("FoxTeam");
		tempDir / "Resources" / "FBX" / "UserMesh" / "Map";

		std::vector<GameEngineFile> vecFile = tempDir.GetAllFile(".UserMesh");

		for (size_t i = 0; i < vecFile.size(); i++)
		{
			if (nullptr == GameEngineFBXMeshManager::GetInst().Find(vecFile[i].GetFullPath()))
			{
				GameEngineFBXMeshManager::GetInst().Delete(vecFile[i].FileName());
			}
		}

		tempDir.MoveParent("UserMesh");
		tempDir.MoveChild("ItemBox");

		vecFile = tempDir.GetAllFile(".UserMesh");

		for (size_t i = 0; i < vecFile.size(); i++)
		{
			if (nullptr == GameEngineFBXMeshManager::GetInst().Find(vecFile[i].GetFullPath()))
			{
				GameEngineFBXMeshManager::GetInst().Delete(vecFile[i].FileName());
			}
		}
	}

	// ĳ����
	Rio::ReleaseResource();
	Hyunwoo::ReleaseResource();

	ItemBoxManager_ = nullptr;
}

void LumiaLevel::Test_GenerateCharactor()
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	for (int i = 0; i < 2; i++)
	{
		PlayerInfo newPlayer;
		newPlayer.playerNumber_ = i;
		newPlayer.startPoint_ = 0;
		newPlayer.character_ = 6;
		newPlayer.curAnimation_ = "";
		newPlayer.isReady_ = true;

		newPlayer.curRotation_ = float4::ZERO;
		newPlayer.curPos_ = { -2500.f, 0.0f, 10000.f };

		pm->AddNewPlayer(newPlayer);

		Character* newCharacter = CreateActor<Rio>();
		PlayerInfoManager::GetInstance()->GetPlayerList()[i].stat_ = newCharacter->GetStat();
		newCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
		newCharacter->SetIndex(i);
		CharacterActorList_.push_back(newCharacter);
	}

	CharacterActorList_[0]->Focus();
	pm->SetPlayerNumber(0);
	pm->SetMainCharacter(CharacterActorList_[0]);
}

#pragma endregion
