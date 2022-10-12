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
		"DOCK",				// 항구
		"POND",				// 연못
		"BEACH",			// 모래사장
		"UPTOWN",			// 고급 주택가
		"ALLEY",			// 골목길
		"HOTEL",			// 호텔
		"AVENUE",			// 번화가
		"HOSPITAL",			// 병원
		"TEMPLE",			// 절
		"ARCHERY",			// 양궁장
		"CEMETERY",			// 묘지
		"FOREST",			// 숲
		"FACTORY",			// 공장
		"CHAPEL",			// 성당
		"SCHOOL",			// 학교
		"RESERCH_CENTRE",	// 연구소
		"ESCAPE_DOCK",		// 오래된 선창
};

void LumiaLevel::HostCreateCommand()
{
	// 서버(호스트) 스레드 작업할당
	GameEngineCore::ThreadQueue.JobPost([&]
		{
			// 서버(호스트) 생성 명령전 생성하려는 몬스터 정보 생성
			CreateMonsterInfo();

			// 서버(호스트)의 리소스 로딩 및 액터생성 시작
			HostAllCreationCommand();

			// Loading Thread End Flag On
			LoadingLevel::ThreadLoadingEnd = true;
		}
	);
}

void LumiaLevel::GuestCreateCommand()
{
	int Count = GameEngineCore::ThreadQueue.GetWorkingCount();

	// 클라이언트(게스트)의 리소스 로딩 및 액터생성 시작
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
	// 예외처리
	if (MonsterType::NONE == _MonsterType || MonsterType::MAX == _MonsterType)
	{
		GameEngineDebug::MsgBoxError("잘못된 타입의 몬스터를 등장시키려고 시도했습니다!!!!!");
		return;
	}

	// 해당 몬스터타입의 몬스터는 일괄적으로 첫등장상태로 전환
	int MonsterCount = static_cast<int>(MonsterActorByTypeList_[static_cast<int>(_MonsterType)].size());
	for (int MonsterNum = 0; MonsterNum < MonsterCount; ++MonsterNum)
	{
		MonsterActorByTypeList_[static_cast<int>(_MonsterType)][MonsterNum]->On();
		MonsterActorByTypeList_[static_cast<int>(_MonsterType)][MonsterNum]->ChangeAnimationAndState(MonsterStateType::APPEAR);
	}
}

void LumiaLevel::GuestMonsterFirstAppear(MonsterType _MonsterType)
{
	// 예외처리
	if (MonsterType::NONE == _MonsterType || MonsterType::MAX == _MonsterType)
	{
		GameEngineDebug::MsgBoxError("잘못된 타입의 몬스터를 등장시키려고 시도했습니다!!!!!");
		return;
	}

	// 해당 몬스터타입의 몬스터는 일괄적으로 첫등장상태로 전환
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
		GameEngineDebug::OutPutDebugString("서버(호스트)에서 현재맵에 배치하려는 몬스터 정보 생성을 완료했습니다!!!!");
	}
}

void LumiaLevel::HostAllCreationCommand()
{
	// Host(서버) 생성관련 명령
	MapCreationCommand();
	MonsterCreationCommand();
	CharacterCreationCommand();
}

void LumiaLevel::GuestAllCreationCommand()
{
	// Guest(클라이언트) 생성관련 명령
	MapCreationCommand();
	MonsterCreationCommand();
	CharacterCreationCommand();
}

void LumiaLevel::MapCreationCommand()
{
	// 맵관련 리소스 로드
	MapResourceLoad();

	// 맵 생성
	CurMap_ = CreateActor<LumiaMap>();

//#ifdef _DEBUG
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	GameEngineDebug::OutPutDebugString(std::to_string(pm->GetMyNumber()) + "번째 플레이어의 맵생성을 완료했습니다\n");
//#endif // _DEBUG
}

void LumiaLevel::MonsterCreationCommand()
{
	// 몬스터정보목록을 참고하여 몬스터 생성시작
	GameTimeController* gm = GameTimeController::GetInstance();
	MonsterInfoManager* mm = MonsterInfoManager::GetInstance();

	int AllMonsterCount = mm->GetCurMonsterListSize();
	for (int MonsterNum = 0; MonsterNum < AllMonsterCount; ++MonsterNum)
	{
		Monsters* NewMonster = nullptr;

		// 현재 몬스터 생성정보 Get
		MonsterInfo CurMonsterInfo = mm->GetAllMonsterListRef()[MonsterNum];

		// 현재 몬스터의 타입체크
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

		// 예외처리
		if (nullptr == NewMonster)
		{
			//GameEngineDebug::OutPutDebugString("잘못된 타입의 몬스터생성을 시도했습니다!!!! 몬스터타입: " + std::to_string(static_cast<int>(CurMonsterType)) + " \n");
			continue;
		}

		// 생성된 몬스터 공통정보 셋팅
		NewMonster->SetMonsterIndex(CurMonsterInfo.Index_);
		NewMonster->SetMonsterAreaType(CurMonsterInfo.RegionType_);

		// 트랜스폼데이터 셋팅
		NewMonster->InitalizeSpawnPosition(CurMonsterInfo.SpawnPosition_);

		// GameController에 의하여 첫등장전까지 Off 상태로 생성
		// -> GameController가 첫등장명령을 내리면 On상태로 전환되며 Appear State를 실행
		NewMonster->Off();

		// 관리목록 추가
		MonsterActorList_.push_back(NewMonster);

		// 일괄처리용 몬스터목록에 추가
		MonsterActorByTypeList_[static_cast<int>(CurMonsterType)].push_back(NewMonster);
	}

//#ifdef _DEBUG
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	GameEngineDebug::OutPutDebugString(std::to_string(pm->GetMyNumber()) + "번째 플레이어의 몬스터생성을 완료했습니다\n");
//#endif // _DEBUG
}

void LumiaLevel::CharacterCreationCommand()
{
	// 캐릭터관련 리소스 로드
	CharacterResourceLoad();

	// Get 현재 게임에 진입한 플레이어 정보
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	// 캐릭터 생성
	int PlayerCount = static_cast<int>(pm->GetPlayerList().size());
	for (int PlayerNum = 0; PlayerNum < PlayerCount; ++PlayerNum)
	{
		// 캐릭터 타입별
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

		// 예외처리
		if (nullptr == NewCharacter)
		{
			GameEngineDebug::MsgBoxError("등록되지않은 캐릭터 타입으로 시작하려는 플레이어가 존재합니다!!!!\n");
			continue;
		}

		// 
		PlayerInfoManager::GetInstance()->GetPlayerList()[PlayerNum].stat_ = NewCharacter->GetStat();
		PlayerInfoManager::GetInstance()->GetPlayerList()[PlayerNum].curPos_ = float4(-2500.f, 0.0f, 10000.f);
		NewCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
		NewCharacter->SetIndex(PlayerNum);
		// 관리목록에 추가
		CharacterActorList_.emplace_back(NewCharacter);

		// 현재 호스트 or 게스트의 메인 캐릭터 포커싱
		if (PlayerNum == pm->GetMyNumber())
		{
			CharacterActorList_[PlayerNum]->Focus();
			PlayerInfoManager::GetInstance()->SetMainCharacter(NewCharacter);
		}
	}

//#ifdef _DEBUG
	GameEngineDebug::OutPutDebugString(std::to_string(pm->GetMyNumber()) + "번째 플레이어의 캐릭터생성을 완료했습니다\n");
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
	// 인게임 마우스 생성
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
		Window->PushRenderTarget("메인 카메라 타겟", GetMainCamera()->GetCameraRenderTarget(), Size);
		Window->PushRenderTarget("UI 카메라 타겟", GetUICamera()->GetCameraRenderTarget(), Size);
		Window->PushRenderTarget("메인 카메라 G-Buffer", GetMainCamera()->GetCameraDeferredGBufferTarget(), Size);
		Window->PushRenderTarget("메인 카메라 디퍼드 라이트", GetMainCamera()->GetCameraDeferredLightTarget(), Size);
		Window->PushRenderTarget("메인 카메라 디퍼드 타겟", GetMainCamera()->GetCameraDeferredTarget(), Size);
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
	// 게임진행시간 동기화를 위한 패킷전송(서버전용)
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
		// 캐릭터 이동갱신 패킷
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

		// 캐릭터 애니메이션갱신 패킷
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

		// 캐릭터 상태갱신 패킷
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
			// 받은 패킷의 정보로 갱신된 playerinfo 값에 따라
			// 레벨의 캐릭터 액터들의 위치 회전 애니메이션 등을 변경해주는 코드블록입니다.
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
		
		// 현재일차 낮/밤
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

	// 서버 패킷 처리
	if (true == GameServer::GetInstance()->IsOpened())
	{
		GameServer::GetInstance()->ProcessPacket();
	}

	// 클라이언트 패킷 처리
	if (true == GameClient::GetInstance()->IsConnected())
	{
		GameClient::GetInstance()->ProcessPacket();
	}

	// 게임진행컨트롤러(레벨업, 낮/밤전환, 몬스터첫등장, ... )
	// 단, 서버일때만 처리하며 서버(호스트)가 현재게임을 제어(한곳에서 제어하기위해)
	if (true == GameServer::GetInstance()->IsOpened())
	{
		// 레벨업, 낮/밤전환, 몬스터첫등장을 위한 타임갱신
		GameTimeController::GetInstance()->HostUpdate(_DeltaTime);

		// 게임시간 실시간 패킷(서버-클라 동기화)
		GameTimeUpdatePacketSend();
	}

	// 캐릭터 상태 업데이트 패킷 전송처리
	CharacterStateUpdatePacketSend();

	// 캐릭터 목록의 트랜스폼 데이터 갱신
	CharactersTransformUpdate();

	// 몬스터 상태 업데이트 패킷 전송처리
	MonsterStateUpdatePacketSend();

	// 몬스터 목록의 트랜스폼 데이터 갱신
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

	//// 221011 SJH TEST : 몬스터 확인을위해 몬스터 포커싱
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
	// 220929 ADD SJH : 테스트용(추후삭제예정)
	// 서버를 생성을 안했거나 클라이언트로 서버에 연결되지않은상태에서 LevelControlWindow에의해 강제로 레벨이동한 경우
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

	// 220929 ADD SJH : 테스트용(추후삭제예정)
	// 서버를 생성을 안했거나 클라이언트로 서버에 연결되지않은상태에서 LevelControlWindow에의해 강제로 레벨이동한 경우
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
		// 기본 액터 생성
		CreateBasicActor();

		// 네트워크 핸들러 등록
		AddSocketHandle();

		// IMGUI Window Find & Setting
		InitIMGUIWindow();

		// MainCamera Adjustment
		CameraAdjustment();
	}
}

#pragma region 테스트용함수(추후삭제예정)
// LevelControlWindow에의해 강제 레벨이동한경우 사용하는 함수
// 서버 생성이 없이 해당 레벨로 체인지한경우

void LumiaLevel::Test_loadResource()
{
	// 맵, 네비게이션 메쉬, 아이템박스
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

	// 캐릭터
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
		Window->PushRenderTarget("메인 카메라 타겟", GetMainCamera()->GetCameraRenderTarget(), Size);
		Window->PushRenderTarget("UI 카메라 타겟", GetUICamera()->GetCameraRenderTarget(), Size);
		Window->PushRenderTarget("메인 카메라 G-Buffer", GetMainCamera()->GetCameraDeferredGBufferTarget(), Size);
		Window->PushRenderTarget("메인 카메라 디퍼드 라이트", GetMainCamera()->GetCameraDeferredLightTarget(), Size);
		Window->PushRenderTarget("메인 카메라 디퍼드 타겟", GetMainCamera()->GetCameraDeferredTarget(), Size);
	}

	DebugAndControlWindow_ = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");
}

void LumiaLevel::Test_createActor()
{
	// 인게임 마우스 생성
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
	// loadResource에서 로드한 리소스 삭제

	// 맵, 네비게이션 메쉬, 아이템박스
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

	// 캐릭터
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
