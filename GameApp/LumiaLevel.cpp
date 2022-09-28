#include "Precompile.h"
#include <GameEngine/GameEngineRenderWindow.h>
#include <GameEngine/SKySphereActor.h>
#include <GameEngine/GameEngineLevelControlWindow.h>
#include <GameEngine/LightActor.h>
#include <GameEngine/GameEngineFBXMesh.h>
#include "PlayerInfoManager.h"
#include "LumiaLevel.h"
#include "LumiaMap.h"
#include "ItemBoxWindow.h"

#include "Rio.h"
#include "Hyunwoo.h"

#include "GameServer.h"
#include "GameClient.h"
#include "ePacketID.h"
#include "CharMovementPacket.h"
#include "CharAnimationPacket.h"
#include "CharStatPacket.h"
#include "Enums.h"

LumiaLevel::LumiaLevel()
	: itemBox_(nullptr)
	, map_(nullptr)
{
}

LumiaLevel::~LumiaLevel()
{
}

void LumiaLevel::LevelStart()
{
	GameEngineInput::GetInst().CreateKey("A", 'A');
	GameEngineInput::GetInst().CreateKey("D", 'D');
	GameEngineInput::GetInst().CreateKey("W", 'W');
	GameEngineInput::GetInst().CreateKey("S", 'S');
	GameEngineInput::GetInst().CreateKey("Q", 'Q');
	GameEngineInput::GetInst().CreateKey("E", 'E');
	GameEngineInput::GetInst().CreateKey("O", 'O');
	GameEngineInput::GetInst().CreateKey("LButton", VK_LBUTTON);
	GameEngineInput::GetInst().CreateKey("RButton", VK_RBUTTON);
	GameEngineInput::GetInst().CreateKey("Esc", VK_ESCAPE);

	// Test Key
	GameEngineInput::GetInst().CreateKey("1", '1');
	GameEngineInput::GetInst().CreateKey("2", '2');
	GameEngineInput::GetInst().CreateKey("3", '3');
}

void LumiaLevel::LevelUpdate(float _DeltaTime)
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	// 캐릭터의 실질적 액터를 관리 중인 건 현재 레벨(== 사실상 게임 컨트롤러 클래스의 역할도 할 수 있다 볼 수 있다)
	if (true == GameServer::GetInstance()->IsOpened())
	{
		GameServer* server = GameServer::GetInstance();
		server->ProcessPacket();

		{
			CharMovementPacket packet;
			packet.SetTargetIndex(pm->GetMyNumber());
			packet.SetPos(characterActorList_[pm->GetMyNumber()]->GetTransform()->GetLocalPosition());
			packet.SetDirection(characterActorList_[pm->GetMyNumber()]->GetTransform()->GetLocalRotation());
			server->Send(&packet);
		}

		{
			CharAnimationPacket packet;
			packet.SetTargetIndex(pm->GetMyNumber());
			packet.SetAnimation(characterActorList_[pm->GetMyNumber()]->GetCurAnimation());
			server->Send(&packet);
		}


	}
	else if (true == GameClient::GetInstance()->IsConnected())
	{
		GameClient* client = GameClient::GetInstance();
		client->ProcessPacket();

		{
			CharMovementPacket packet;
			packet.SetTargetIndex(pm->GetMyNumber());
			packet.SetPos(characterActorList_[pm->GetMyNumber()]->GetTransform()->GetLocalPosition());
			packet.SetDirection(characterActorList_[pm->GetMyNumber()]->GetTransform()->GetLocalRotation());
			client->Send(&packet);
		}

		{
			CharAnimationPacket packet;
			packet.SetTargetIndex(pm->GetMyNumber());
			packet.SetAnimation(characterActorList_[pm->GetMyNumber()]->GetCurAnimation());
			client->Send(&packet);

		}

		{
			// 자기 스테이터스를 계속 업데이트하는 패킷
			CharStatPacket packet;
			packet.SetTargetIndex(pm->GetMyNumber());
			packet.SetStat(*(pm->GetMyPlayer().stat_));
			client->Send(&packet);
		}

	}

#ifdef SERVER
	for (int i = 0; i < pm->GetPlayerList().size(); i++)
	{
		// 받은 패킷의 정보로 갱신된 playerinfo 값에 따라
		// 레벨의 캐릭터 액터들의 위치 회전 애니메이션 등을 변경해주는 코드블록입니다.

		if (i == pm->GetMyNumber())
		{
			continue;
		}

		characterActorList_[i]->GetTransform()->SetLocalPosition(pm->GetPlayerList()[i].curPos_);
		characterActorList_[i]->ChangeAnimation(pm->GetPlayerList()[i].curAnimation_);
		characterActorList_[i]->GetTransform()->SetWorldRotationDegree(pm->GetPlayerList()[i].curDir_);
	}
#endif

	if (GameEngineInput::GetInst().Down("O"))
	{
		GetMainCameraActor()->FreeCameraModeSwitch();
	}

	if (!GetMainCameraActor()->IsFreeCameraMode())
	{
		float4 playerPosition = characterActorList_[pm->GetMyNumber()]->GetTransform()->GetWorldPosition();

		GetMainCameraActor()->GetTransform()->SetWorldPosition(playerPosition + float4(400.f, 1280.f, -600.f));

		GetMainCameraActor()->GetTransform()->SetLocalRotationDegree({ 60.f, -35.f, 0.0f });
	}

	

	GameEngineLevelControlWindow* controlWindow = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");
	if (nullptr != controlWindow)
	{
		if (nullptr != MousePointer::InGameMouse)
		{
			float4 position = MousePointer::InGameMouse->GetIntersectionYAxisPlane(0, 50000.f);

			controlWindow->AddText("X : " + std::to_string(position.x));
			controlWindow->AddText("Z : " + std::to_string(position.z));

			float4 playerPosition = characterActorList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->GetTransform()->GetWorldPosition();
			float4 camPos = GetMainCameraActor()->GetTransform()->GetWorldPosition();
			float4 playerToCamera = camPos - playerPosition;
			float4 cameraRotation = GetMainCameraActor()->GetTransform()->GetLocalRotation();
			controlWindow->AddText("Player to Camera vector : " + std::to_string(playerToCamera.x) + ", " + std::to_string(playerToCamera.y) + ", " + std::to_string(playerToCamera.z));
			controlWindow->AddText("CameraRotation : " + std::to_string(cameraRotation.x) + ", " + std::to_string(cameraRotation.y) + ", " + std::to_string(cameraRotation.z));

		}
	}

}

void LumiaLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
	this->ClearAll();
	releaseRenderWindow();
	releaseResource();

	MainCameraActor_ = CreateActor<CameraActor>();
	UICameraActor_ = CreateActor<CameraActor>();
	UICameraActor_->GetCamera()->SetProjectionMode(ProjectionMode::Orthographic);
}

void LumiaLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	serverCheck();
	loadResource();
	initRenderWindow();
	createActor();
	adjustCamera();
	GenerateCharactor();

	ItemBoxWindow* Window = nullptr;

	if (nullptr == GameEngineGUI::GetInst()->FindGUIWindow("ItemBoxWindow"))
	{
		Window = GameEngineGUI::GetInst()->CreateGUIWindow<ItemBoxWindow>("ItemBoxWindow");
	}

	if (nullptr != Window)
	{
		Window->ItemBoxManager_ = itemBox_;
	}
}

void LumiaLevel::GenerateCharactor()
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

#ifdef SERVER
	for (size_t i = 0; i < pm->GetPlayerList().size(); i++)
	{
		pm->GetPlayerList()[i].curAnimation_ = "";

		Character* newCharacter = nullptr;

		switch (static_cast<JobType>(pm->GetPlayerList()[i].character_))
		{
		case JobType::NONE:
			GameEngineDebug::MsgBoxError("캐릭터 선택을 하지 않고 게임을 시작한 유저가 있습니다.\n" + std::to_string(i) + " 번 플레이어");
			break;
		case JobType::YUKI:
		{
			newCharacter = CreateActor<Rio>();
			break;
		}
		case JobType::FIORA:
		{
			newCharacter = CreateActor<Rio>();
			break;
		}
		case JobType::ZAHIR:
		{
			newCharacter = CreateActor<Rio>();
			break;
		}
		case JobType::NADINE:
		{
			newCharacter = CreateActor<Rio>();
			break;
		}
		case JobType::HYUNWOO:
		{
			newCharacter = CreateActor<Hyunwoo>();
			break;
		}
		case JobType::JACKIE:
		{
			newCharacter = CreateActor<Rio>();
			break;
		}
		case JobType::RIO:
		{
			newCharacter = CreateActor<Rio>();
			
			break;
		}
		case JobType::AYA:
		{
			newCharacter = CreateActor<Rio>();
			break;
		}
		case JobType::MAX:
			GameEngineDebug::MsgBoxError("캐릭터 선택을 하지 않고 게임을 시작한 유저가 있습니다.\n" + std::to_string(i) + " 번 플레이어");
			break;
		default:
			GameEngineDebug::MsgBoxError("캐릭터 선택을 하지 않고 게임을 시작한 유저가 있습니다.\n" + std::to_string(i) + " 번 플레이어");
			break;
		}

		PlayerInfoManager::GetInstance()->GetPlayerList()[i].stat_ = newCharacter->GetStat();
		newCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
		newCharacter->SetIndex(i);
		characterActorList_.emplace_back(newCharacter);
		
		if (i == pm->GetMyNumber())
		{
			characterActorList_[i]->Focus();
			pm->SetMainCharacter(characterActorList_[pm->GetMyNumber()]);
			pm->SetPlayerNumber(pm->GetMyNumber());
		}
	}
#else
	for (int i = 0; i < 2; i++)
	{

		Character* newCharacter = CreateActor<Rio>();
		newCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
		PlayerInfo newPlayer;
		newPlayer.playerNumber_ = i;
		newPlayer.startPoint_ = 0;
		newPlayer.character_ = 0;
		newPlayer.curAnimation_ = "";
		newPlayer.isReady_ = true;

		newPlayer.curDir_ = float4::ZERO;
		newPlayer.curPos_ = float4::ZERO;

		pm->AddNewPlayer(newPlayer);
		characterActorList_.push_back(newCharacter);
	}

	characterActorList_[0]->Focus();
	pm->SetMainCharacter(characterActorList_[0]);

	pm->SetPlayerNumber(0);
#endif
}

void LumiaLevel::loadResource()
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

void LumiaLevel::releaseResource()
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

	itemBox_ = nullptr;
}

void LumiaLevel::createActor()
{
	// 인게임 마우스 생성
	if (nullptr == MousePointer::InGameMouse)
	{
		MousePointer::InGameMouse = CreateActor<MousePointer>();
		MousePointer::InGameMouse->GetTransform()->SetLocalPosition(GameEngineInput::GetInst().GetMouse3DPos());
		MousePointer::InGameMouse;
	}

	{
		GameEngineDirectory tempDir;

		tempDir.MoveParent("FoxTeam");
		tempDir / "Resources" / "FBX" / "UserMesh" / "ItemBox" / "ItemBoxInfo";

		itemBox_ = CreateActor<ItemBoxManager>();
		itemBox_->UserAllLoad(tempDir);
		itemBox_->GetTransform()->SetLocalScaling(100.0f);
	}

	map_ = CreateActor<LumiaMap>();

	SKySphereActor* Actor = CreateActor<SKySphereActor>();


	{
		LightActor* Actor;

		Actor = CreateActor<LightActor>();
		Actor->GetLight()->SetDiffusePower(1.f);
		Actor->GetLight()->SetAmbientPower(10.f);
		Actor->GetLight()->SetSpacularLightPow(10.f);
	}

	
	
}

void LumiaLevel::adjustCamera()
{
	GetMainCameraActor()->GetCamera()->SetFov(50.f);
	GetMainCameraActor()->FreeCameraModeSwitch();
	GetMainCameraActor()->GetTransform()->SetWorldPosition({ 0.0f, 100.f, -200.f });
}

void LumiaLevel::serverCheck()
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

void LumiaLevel::initRenderWindow()
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
}

void LumiaLevel::releaseRenderWindow()
{
	if (nullptr != GameEngineGUI::GetInst()->FindGUIWindow("RenderWindow"))
	{
		GameEngineRenderWindow* Window = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
		Window->ClaerRenderTarget();
	}
}
