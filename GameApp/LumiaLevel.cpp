#include "Precompile.h"
#include <GameEngine/GameEngineRenderWindow.h>
#include <GameEngine/SKySphereActor.h>
#include <GameEngine/LightActor.h>
#include <GameEngine/GameEngineLevelControlWindow.h>

#include "PlayerInfoManager.h"
#include "LumiaLevel.h"
#include "LumiaMap.h"

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
	if (GameEngineInput::GetInst().Down("O"))
	{
		GetMainCameraActor()->FreeCameraModeSwitch();
	}

	if (!GetMainCameraActor()->IsFreeCameraMode())
	{
		GetMainCameraActor()->GetTransform()->SetWorldPosition(characterActorList_[PlayerInfoManager::GetInstance()->GetMyNumber()]->GetTransform()->GetLocalPosition() + float4(-200.0f, 800.f, -200.f));
		GetMainCameraActor()->GetTransform()->SetLocalRotationDegree({ 70.0f, 45.0f, 0.0f });
	}


	GameEngineLevelControlWindow* controlWindow = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");
	if (nullptr != controlWindow)
	{
		if (nullptr != mouse_)
		{
			float4 position = mouse_->GetIntersectionYAxisPlane(0, 50000.f);

			controlWindow->AddText("X : " + std::to_string(position.x));
			controlWindow->AddText("Z : " + std::to_string(position.z));
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
			GameEngineDebug::MsgBoxError("ĳ���� ������ ���� �ʰ� ������ ������ ������ �ֽ��ϴ�.\n" + std::to_string(i) + " �� �÷��̾�");
			break;
		case JobType::YUKI:
		{
			Rio* newCharacter = CreateActor<Rio>();
			newCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
			characterActorList_.emplace_back(newCharacter);
			break;
		}
		case JobType::FIORA:
		{
			Rio* newCharacter = CreateActor<Rio>();
			newCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
			characterActorList_.emplace_back(newCharacter);
			break;
		}
		case JobType::ZAHIR:
		{
			Rio* newCharacter = CreateActor<Rio>();
			newCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
			characterActorList_.emplace_back(newCharacter);
			break;
		}
		case JobType::NADINE:
		{
			Rio* newCharacter = CreateActor<Rio>();
			newCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
			characterActorList_.emplace_back(newCharacter);
			break;
		}
		case JobType::HYUNWOO:
		{
			Rio* newCharacter = CreateActor<Rio>();
			newCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
			characterActorList_.emplace_back(newCharacter);
			break;
		}
		case JobType::JACKIE:
		{
			Rio* newCharacter = CreateActor<Rio>();
			newCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
			characterActorList_.emplace_back(newCharacter);
			break;
		}
		case JobType::RIO:
		{
			Rio* newCharacter = CreateActor<Rio>();
			newCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
			characterActorList_.emplace_back(newCharacter);
			break;
		}
		case JobType::AYA:
		{
			Rio* newCharacter = CreateActor<Rio>();
			newCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
			characterActorList_.emplace_back(newCharacter);
			break;
		}
		case JobType::MAX:
			GameEngineDebug::MsgBoxError("ĳ���� ������ ���� �ʰ� ������ ������ ������ �ֽ��ϴ�.\n" + std::to_string(i) + " �� �÷��̾�");
			break;
		default:
			GameEngineDebug::MsgBoxError("ĳ���� ������ ���� �ʰ� ������ ������ ������ �ֽ��ϴ�.\n" + std::to_string(i) + " �� �÷��̾�");
			break;
		}

		// ĳ���� ���͸� ����� �ְ�(���� ĳ���͸� �� �������� ���� �ڵ尡 ����� �����丵 �� ��)

		if (i == pm->GetMyNumber())
		{
			characterActorList_[i]->TempFlag = false;
			// TempFlag �� ���� ���� ������Ʈ �� ��Ʈ�� �κ��� ���ų� �ǳʶ� �� �ְ� ���� �ӽ� �÷��װ��Դϴ�.
			// ������ �Էºΰ� ���� ��Ʈ�ѷ� Ŭ������ ��ȭ�Ǹ� ������ �����Դϴ�.
			// player_ = newCharacter;
			// pm->SetMainCharacter(player_);
			// ������ �� ĳ���� �ε��� ������ �ְ�...
			// characterActorList[�� �ε���] �� �� ĳ���� ���� ã�ư���...
		}
		else
		{
			characterActorList_[i]->TempFlag = true;
		}
	}
#else
	for (size_t i = 0; i < 2; i++)
	{
		Rio* newCharacter = CreateActor<Rio>();
		newCharacter->InitSpawnPoint({ -2500.f, 0.0f, 10000.f });
		PlayerInfo newPlayer;
		newPlayer.playerNumber_ = i;
		newPlayer.startPoint_ = 0;
		newPlayer.character_ = 0;
		newPlayer.isReady_ = true;
		pm->AddNewPlayer(newPlayer);
		characterActorList_.push_back(newCharacter);

		if (i == 0)
		{
			characterActorList_[0]->TempFlag = true;
			pm->SetMainCharacter(characterActorList_[0]);
		}
	}

	pm->SetPlayerNumber(0);
#endif
}

void LumiaLevel::loadResource()
{
	{
		GameEngineDirectory tempDir;

		tempDir.MoveParent("FoxTeam");
		tempDir / "Resources" / "FBX" / "Map";

		//{
		//	GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(tempDir.PathToPlusFileName("Downtown.fbx"));
		//	Mesh->CreateRenderingBuffer();
		//}

		{
			GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(tempDir.PathToPlusFileName("DowntownNavMesh.fbx"));
			Mesh->CreateRenderingBuffer();
		}

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

	{
		GameEngineDirectory dir;

		dir.MoveParent("FoxTeam");
		dir / "Resources" / "FBX" / "GH";

		std::string meshName = "Rio_Run.fbx";

		GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Rio_Run.fbx"));
		mesh->CreateRenderingBuffer();

		GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Rio_Run.fbx"));
		GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Rio_Wait.fbx"));
	}
}

void LumiaLevel::releaseResource()
{
	// loadResource���� �ε��� ���ҽ� ����

	GameEngineFBXMeshManager::GetInst().Delete("Bg_NaviMesh_Cobalt.fbx");
	GameEngineFBXMeshManager::GetInst().Delete("Bg_NaviMesh.fbx");
	GameEngineFBXMeshManager::GetInst().Delete("Rio_Run.fbx");

	GameEngineFBXAnimationManager::GetInst().Delete("Rio_Run.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("Rio_Wait.fbx");

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
	GetMainCameraActor()->GetCamera()->SetFov(60.f);
	GetMainCameraActor()->FreeCameraModeSwitch();
	GetMainCameraActor()->GetTransform()->SetWorldPosition({ 0.0f, 100.f, -200.f });
}

void LumiaLevel::initRenderWindow()
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
}

void LumiaLevel::releaseRenderWindow()
{
	if (nullptr != GameEngineGUI::GetInst()->FindGUIWindow("RenderWindow"))
	{
		GameEngineRenderWindow* Window = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
		Window->ClaerRenderTarget();
	}
}
