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
	// 로딩레벨로부터 수신하는 명령
	switch (_LoadingType)
	{
	case LoadingType::MAP:
	{
		// 별도 정보 필요없음
		// 맵생성 명령 패킷 전송후
		MapCreationPacketSend();

		// 생성 성공시 패킷 전송
		if (true == MapCreationCommand() && true == GameServer::GetInstance()->IsOpened())
		{
			GameEngineDebug::OutPutDebugString("서버(호스트)의 맵 생성을 완료했습니다!!!!\n");
		}

		break;
	}
	case LoadingType::MONSTER:
	{
		// 생성 성공시 패킷 전송
		if (true == MonsterCreationCommand() && true == GameServer::GetInstance()->IsOpened())
		{
			GameEngineDebug::OutPutDebugString("서버(호스트)의 몬스터 생성을 완료했습니다!!!!\n");

			// 별도 정보 필요
			// 몬스터 정보 생성완료 후 패킷 전송
			MonsterCreationPacketSend();
		}

		break;
	}
	case LoadingType::CHARACTER:
	{
		// 별도 정보 필요없음
		// 캐릭터생성 명령 패킷 전송후
		CharacterCreationPacketSend();

		// 생성 성공시 패킷 전송
		if (true == CharacterCreationCommand() && true == GameServer::GetInstance()->IsOpened())
		{
			GameEngineDebug::OutPutDebugString("서버(호스트)의 캐릭터 생성을 완료했습니다!!!!\n");
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
		GameEngineDebug::OutPutDebugString("클라이언트(게스트) Number: " + std::to_string(MyNumber) + "의 맵 생성을 완료했습니다!!!!\n");
	}
}

void LumiaLevel::ClientMonsterCreation()
{
	// 클라이언트(게스트)측에서 호출하는 함수로 
	// 수신한 몬스터 정보를 토대로 몬스터 액터를 생성하는 로직을 구현
	MonsterInfoManager* InfoManager = MonsterInfoManager::GetInstance();

	// 생성하려는 몬스터 정보 Get
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

		// 예외처리
		if (nullptr == NewMonster)
		{
			GameEngineDebug::OutPutDebugString(std::to_string(MonsterNum) + "번째 몬스터는 " + "등록되지않은 타입으로 생성하려고 시도했습니다!!!!\n");
			continue;
		}

		// NewMonster Setting
		NewMonster->SetMonsterIndex(MonsterNum);
		NewMonster->SetMonsterAreaType(AllMonster[MonsterNum].AreaType_);
		NewMonster->InitalizeSpawnPosition(AllMonster[MonsterNum].SpawnPosition_);

		// 99. 몬스터 관리목록에 추가
		MonsterActorList_.push_back(NewMonster);
	}

	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	int MyNumber = pm->GetMyNumber();
	GameEngineDebug::OutPutDebugString("클라이언트(게스트) Number: " + std::to_string(MyNumber) + "의 몬스터 생성을 완료했습니다!!!!\n");
}

void LumiaLevel::ClientCharacterCreation()
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	if (true == CharacterCreationCommand())
	{
		int MyNumber = pm->GetMyNumber();
		GameEngineDebug::OutPutDebugString("클라이언트(게스트) Number: " + std::to_string(MyNumber) + "의 캐릭터 생성을 완료했습니다!!!!\n");
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

	// 몬스터생성 패킷 데이터 셋팅
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

	// 패킷전송
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
	// 맵관련 리소스 로드
	MapResourceLoad();

	// 맵 생성
	CurMap_ = CreateActor<LumiaMap>();

	return true;
}

bool LumiaLevel::MonsterCreationCommand()
{
	// 패킷 전송을 위해 먼저데이터를 만들어내므로 생성된 데이터를 기준으로 액터생성

	// 0. 랜덤한 갯수 몬스터 생성
	GameEngineRandom CountRandom;
	//int CreateCount = CountRandom.RandomInt(1, 10);			// 임시주석
	int CreateCount = 1;										// 임시사용(1마리만 생성)
	for (int MonsterNum = 0; MonsterNum < CreateCount; ++MonsterNum)
	{
		Monsters* NewMonster = nullptr;

		// 1. 랜덤한 몬스터 타입으로 몬스터 생성
		GameEngineRandom TypeRandom;
		//MonsterType CurType = static_cast<MonsterType>(TypeRandom.RandomInt(0, static_cast<int>(MonsterType::MAX) - 1));			// 임시주석
		MonsterType CurType = MonsterType::WOLF;																					// 임시사용
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

		// 예외처리
		if (nullptr == NewMonster)
		{
			GameEngineDebug::OutPutDebugString(std::to_string(MonsterNum) + "번째 몬스터는 " + "등록되지않은 타입으로 생성하려고 시도했습니다!!!!\n");
			continue;
		}

		// 액터 생성 성공시 초기화 & 셋팅함수 호출
		NewMonster->SetMonsterIndex(MonsterNum);
		NewMonster->InitalizeSpawnAreaType();

		// 99. 몬스터 관리목록에 추가
		MonsterActorList_.push_back(NewMonster);
	}

	return true;
}

bool LumiaLevel::CharacterCreationCommand()
{
	// 캐릭터관련 리소스 로드
	CharacterResourceLoad();

	// Get 현재 게임에 진입한 플레이어 정보
	PlayerInfoManager* PlayerInfo = PlayerInfoManager::GetInstance();

	// 캐릭터 생성
	int PlayerCount = static_cast<int>(PlayerInfo->GetPlayerList().size());
	for (int PlayerNum = 0; PlayerNum < PlayerCount; ++PlayerNum)
	{
		// 캐릭터 타입별
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

		// 예외처리
		if (nullptr == NewCharacter)
		{
			GameEngineDebug::MsgBoxError("등록되지않은 캐릭터 타입으로 시작하려는 플레이어가 존재합니다!!!!\n");
			continue;
		}

		// 
		PlayerInfoManager::GetInstance()->GetPlayerList()[PlayerNum].stat_ = NewCharacter->GetStat();
		NewCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
		NewCharacter->SetIndex(PlayerNum);

		// 관리목록에 추가
		CharacterActorList_.emplace_back(NewCharacter);

		// 현재 호스트 or 게스트의 메인 캐릭터 포커싱
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

void LumiaLevel::CharacterStateUpdatePacketSend()
{
	GameServer* ServerSocket = GameServer::GetInstance();
	GameClient* ClientSocket = GameClient::GetInstance();

	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	// 캐릭터 이동갱신 패킷
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

	// 캐릭터 애니메이션갱신 패킷
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

void LumiaLevel::CharactersTransformUpdate()
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

#ifdef SERVER
	for (int i = 0; i < pm->GetPlayerList().size(); i++)
	{
		// 받은 패킷의 정보로 갱신된 playerinfo 값에 따라
		// 레벨의 캐릭터 액터들의 위치 회전 애니메이션 등을 변경해주는 코드블록입니다.
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
	// 기본 액터 생성
	CreateBasicActor();

	// Create LumiaLevel Input
	CreateLevelInput();

	// 네트워크 핸들러 등록
	AddSocketHandle();

	// IMGUI Window Find & Setting
	InitIMGUIWindow();

	// MainCamera Adjustment
	CameraAdjustment();
}
