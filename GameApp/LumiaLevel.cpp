#include "Precompile.h"
#include <GameEngine/GameEngineRenderWindow.h>
#include <GameEngine/SKySphereActor.h>
#include <GameEngine/LightActor.h>
#include <GameEngine/GameEngineLevelControlWindow.h>
#include <GameEngine/GameEngineFBXMesh.h>
#include "PlayerInfoManager.h"
#include "LumiaLevel.h"
#include "LumiaMap.h"

#include "Rio.h"
#include "Hyunwoo.h"

#include "GameServer.h"
#include "GameClient.h"
#include "ePacketID.h"
#include "CharMovementPacket.h"
//#include "LumiaUIController.h"

LumiaLevel::LumiaLevel()
	: mouse_(nullptr)
	, itemBox_(nullptr)
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
}

void LumiaLevel::LevelUpdate(float _DeltaTime)
{
	// 캐릭터의 실질적 액터를 관리 중인 건 현재 레벨(== 사실상 게임 컨트롤러 클래스의 역할도 할 수 있다 볼 수 있다)
	if (true == GameServer::GetInstance()->IsOpened())
	{
		GameServer* server = GameServer::GetInstance();
		server->ProcessPacket();

	}
	else if (true == GameClient::GetInstance()->IsConnected())
	{
		GameClient* client = GameClient::GetInstance();
		client->ProcessPacket();
	}

	if (GameEngineInput::GetInst().Down("O"))
	{
		GetMainCameraActor()->FreeCameraModeSwitch();
	}

	if (!GetMainCameraActor()->IsFreeCameraMode())
	{
		float4 playerPosition = characterActorList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->GetTransform()->GetWorldPosition();

		GetMainCameraActor()->GetTransform()->SetWorldPosition(playerPosition + float4(400.f, 1280.f, -600.f));

		GetMainCameraActor()->GetTransform()->SetLocalRotationDegree({ 60.f, -35.f, 0.0f });
	}

	

	GameEngineLevelControlWindow* controlWindow = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");
	if (nullptr != controlWindow)
	{
		if (nullptr != mouse_)
		{
			float4 position = mouse_->GetIntersectionYAxisPlane(0, 50000.f);

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


	{
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

}

void LumiaLevel::GenerateCharactor()
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

#ifdef SERVER
	for (size_t i = 0; i < pm->GetPlayerList().size(); i++)
	{
		switch (static_cast<JobType>(pm->GetPlayerList()[i].character_))
		{
		case JobType::NONE:
			GameEngineDebug::MsgBoxError("캐릭터 선택을 하지 않고 게임을 시작한 유저가 있습니다.\n" + std::to_string(i) + " 번 플레이어");
			break;
		case JobType::YUKI:
		{
			Character* newCharacter = CreateActor<Rio>();
			newCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
			characterActorList_.emplace_back(newCharacter);
			break;
		}
		case JobType::FIORA:
		{
			Character* newCharacter = CreateActor<Rio>();
			newCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
			characterActorList_.emplace_back(newCharacter);
			break;
		}
		case JobType::ZAHIR:
		{
			Character* newCharacter = CreateActor<Rio>();
			newCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
			characterActorList_.emplace_back(newCharacter);
			break;
		}
		case JobType::NADINE:
		{
			Character* newCharacter = CreateActor<Rio>();
			newCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
			characterActorList_.emplace_back(newCharacter);
			break;
		}
		case JobType::HYUNWOO:
		{
			Character* newCharacter = CreateActor<Rio>();
			newCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
			characterActorList_.emplace_back(newCharacter);
			break;
		}
		case JobType::JACKIE:
		{
			Character* newCharacter = CreateActor<Rio>();
			newCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
			characterActorList_.emplace_back(newCharacter);
			break;
		}
		case JobType::RIO:
		{
			Character* newCharacter = CreateActor<Rio>();
			newCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
			characterActorList_.emplace_back(newCharacter);
			break;
		}
		case JobType::AYA:
		{
			Character* newCharacter = CreateActor<Rio>();
			newCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
			characterActorList_.emplace_back(newCharacter);
			break;
		}
		case JobType::MAX:
			GameEngineDebug::MsgBoxError("캐릭터 선택을 하지 않고 게임을 시작한 유저가 있습니다.\n" + std::to_string(i) + " 번 플레이어");
			break;
		default:
			GameEngineDebug::MsgBoxError("캐릭터 선택을 하지 않고 게임을 시작한 유저가 있습니다.\n" + std::to_string(i) + " 번 플레이어");
			break;
		}

		// 캐릭터 액터를 만들어 주고(무슨 캐릭터를 고를 것인지에 대한 코드가 여기로 리팩토링 될 것)

		if (i == pm->GetMyNumber())
		{
			characterActorList_[i]->TempFlag = false;
			// TempFlag 는 액터 안의 업데이트 중 컨트롤 부분을 쓰거나 건너뛸 수 있게 만든 임시 플래그값입니다.
			// 액터의 입력부가 게임 컨트롤러 클래스로 분화되면 지워질 예정입니다.
			// player_ = newCharacter;
			// pm->SetMainCharacter(player_);
			// 어차피 내 캐릭터 인덱스 가지고 있고...
			// characterActorList[내 인덱스] 면 내 캐릭터 액터 찾아간다...
		}
		else
		{
			characterActorList_[i]->TempFlag = true;
		}
	}
#else
	for (int i = 0; i < 2; i++)
	{
		Character* newCharacter = CreateActor<Hyunwoo>();
		newCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
		PlayerInfo newPlayer;
		newPlayer.playerNumber_ = i;
		newPlayer.startPoint_ = 0;
		newPlayer.character_ = 0;
		newPlayer.isReady_ = true;
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
	{
		GameEngineDirectory tempDir;

		tempDir.MoveParent("FoxTeam");
		tempDir / "Resources" / "FBX" / "Map";

		if (nullptr == GameEngineFBXMeshManager::GetInst().Find(tempDir.PathToPlusFileName("Bg_NaviMesh.fbx")))
		{
			GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(tempDir.PathToPlusFileName("Bg_NaviMesh.fbx"));
			Mesh->CreateRenderingBuffer();
		}
	}

	{
		GameEngineDirectory tempDir;
		tempDir.MoveParent("FoxTeam");
		tempDir / "Resources" / "FBX" / "UserMesh" / "Map";

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


	{
		GameEngineDirectory dir;

		dir.MoveParent("FoxTeam");
		dir / "Resources" / "FBX" / "GH";

		GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Rio_Run.fbx"));
		mesh->CreateRenderingBuffer();

		GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Rio_Run.fbx"));
		GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Rio_Wait.fbx"));
	}

	{
		GameEngineDirectory dir;

		dir.MoveParent("FoxTeam");
		dir / "Resources" / "FBX" / "PJW";

		GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_run.fbx"));
		mesh->CreateRenderingBuffer();

		GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_run.fbx"));
		GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_wait.fbx"));
	}
}

void LumiaLevel::releaseResource()
{
	// loadResource에서 로드한 리소스 삭제

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

	{
		GameEngineFBXMeshManager::GetInst().Delete("Rio_Run.fbx");

		GameEngineFBXAnimationManager::GetInst().Delete("Rio_Run.fbx");
		GameEngineFBXAnimationManager::GetInst().Delete("Rio_Wait.fbx");
	}

	{
		GameEngineFBXMeshManager::GetInst().Delete("hyunwoo_run.fbx");

		GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_run.fbx");
		GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_wait.fbx");
	}

	mouse_ = nullptr;
	itemBox_ = nullptr;
}

void LumiaLevel::createActor()
{
	mouse_ = CreateActor<MousePointer>();
	map_ = CreateActor<LumiaMap>();

	{
		GameEngineDirectory tempDir;

		tempDir.MoveParent("FoxTeam");
		tempDir / "Resources" / "FBX" / "UserMesh" / "ItemBox" / "ItemBoxInfo";

		itemBox_ = CreateActor<ItemBoxManager>();
		itemBox_->UserAllLoad(tempDir);
		itemBox_->GetTransform()->SetLocalScaling(100.0f);
	}

	SKySphereActor* Actor = CreateActor<SKySphereActor>();


	{
		LightActor* Actor;

		Actor = CreateActor<LightActor>();
		Actor->GetLight()->SetDiffusePower(1.f);
		Actor->GetLight()->SetAmbientPower(10.f);
		Actor->GetLight()->SetSpacularLightPow(10.f);
	}

	
	//LumiaUIController* UIController = CreateActor<LumiaUIController>();
	
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

	}
	else if (true == GameClient::GetInstance()->IsConnected())
	{
		GameClient* client = GameClient::GetInstance();
		client->AddPacketHandler(ePacketID::CharMovementPacket, new CharMovementPacket);
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
