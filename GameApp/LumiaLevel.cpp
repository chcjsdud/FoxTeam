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
#include "CreationCommandPacket.h"
#include "LoadingEndPacket.h"

#include "LoadingLevel.h"

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

void LumiaLevel::HostCreateCommand()
{
	// 서버(호스트) 생성 명령전 생성하려는 몬스터 정보 생성
	CreateMonsterInfo();

	// 서버(호스트)의 리소스 로딩 및 액터생성 시작
	GameEngineCore::ThreadQueue.JobPost([&]
		{
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

void LumiaLevel::CreateMonsterInfo()
{
	MonsterInfoManager* mm = MonsterInfoManager::GetInstance();

	// 몬스터 생성정보 작성 시작
	//int Index_;									// 몬스터 생성 인덱스(탐색용)
	//Location AreaType_;							// 몬스터 스폰 지역 타입(탐색용)
	//MonsterType MonsterType_;						// 몬스터 타입
	//float4 SpawnPosition_;						// 스폰 위치
	// -> 위의 정보 모두 해당 함수에서 결정

	// 현재맵의 몬스터타입-출현지역-갯수 기준으로 몬스터 생성
	//int LocationCnt = static_cast<int>(Location::MAX);
	//for (int LocationNum = 0; LocationNum < LocationCnt; ++LocationNum)
	//{



	//}

	// 생성 패킷 전송
	CreationCommandPacket CommandPacket;
	CommandPacket.SetMonsterInfos(mm->GetAllMonsterListValue());
	GameServer::GetInstance()->Send(&CommandPacket);
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

#ifdef _DEBUG
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	GameEngineDebug::OutPutDebugString(std::to_string(pm->GetMyNumber()) + "번째 플레이어의 맵생성을 완료했습니다\n");
#endif // _DEBUG
}

void LumiaLevel::MonsterCreationCommand()
{
	// 몬스터정보목록을 참고하여 몬스터 생성시작
	MonsterInfoManager* mm = MonsterInfoManager::GetInstance();

	int AllMonsterCount = mm->GetCurMonsterListSize();
	for (int MonsterNum = 0; MonsterNum < AllMonsterCount; ++MonsterNum)
	{








	}

#ifdef _DEBUG
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	GameEngineDebug::OutPutDebugString(std::to_string(pm->GetMyNumber()) + "번째 플레이어의 몬스터생성을 완료했습니다\n");
#endif // _DEBUG
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
		}
	}

#ifdef _DEBUG
	GameEngineDebug::OutPutDebugString(std::to_string(pm->GetMyNumber()) + "번째 플레이어의 캐릭터생성을 완료했습니다\n");
#endif // _DEBUG
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
	if (-1 != pm->GetMyNumber() && 0 < static_cast<int>(CharacterActorList_.size()))
	{
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

			CharacterActorList_[i]->GetTransform()->SetLocalPosition(pm->GetPlayerList()[i].curPos_);
			CharacterActorList_[i]->ChangeAnimation(pm->GetPlayerList()[i].curAnimation_);
			CharacterActorList_[i]->GetTransform()->SetWorldRotationDegree(pm->GetPlayerList()[i].curDir_);
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

		PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
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
	if (false == GetMainCameraActor()->IsFreeCameraMode() && -1 != pm->GetMyNumber())
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

		newPlayer.curDir_ = float4::ZERO;
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
