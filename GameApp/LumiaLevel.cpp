#include "Precompile.h"
#include "LumiaLevel.h"
#include <GameEngine/GameEngineRenderWindow.h>
#include <GameEngine/SKySphereActor.h>
#include <GameEngine/LightActor.h>
#include <GameEngine/GameEngineLevelControlWindow.h>



LumiaLevel::LumiaLevel()
	: mouse_(nullptr)
	, lumiaMap(nullptr)
	, ItemBox_(nullptr)
	, player_(nullptr)
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
}

void LumiaLevel::LevelUpdate(float _DeltaTime)
{
	if (GameEngineInput::GetInst().Down("O"))
	{
		GetMainCameraActor()->FreeCameraModeSwitch();
	}

	if (!GetMainCameraActor()->IsFreeCameraMode())
	{
		GetMainCameraActor()->GetTransform()->SetWorldPosition(player_->GetTransform()->GetWorldPosition() + float4(-200.0f, 800.f, -200.f));
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
}

void LumiaLevel::loadResource()
{
	{
		GameEngineDirectory tempDir;

		tempDir.MoveParent("FoxTeam");
		tempDir.MoveChild("Resources");
		tempDir.MoveChild("FBX");
		tempDir.MoveChild("YSJ");

		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(tempDir.PathToPlusFileName("Bg_NaviMesh.fbx"));
		Mesh->CreateRenderingBuffer();

		Mesh = GameEngineFBXMeshManager::GetInst().Load(tempDir.PathToPlusFileName("NaviCol.fbx"));
		Mesh->CreateRenderingBuffer();
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
	}

	{
		GameEngineDirectory tempDir;
		tempDir.MoveParent("FoxTeam");
		tempDir / "Resources" / "FBX" / "UserMesh" / "ItemBox";

		std::vector<GameEngineFile> vecFile = tempDir.GetAllFile(".UserMesh");
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
		GameEngineDirectory tempDir;
		tempDir.MoveParent("FoxTeam");
		tempDir / "Resources" / "FBX" / "UserMesh" / "ItemBox" / "ItemBoxInfo";

		//ItemBox_ = CreateActor<ItemBoxManager>();

		//ItemBox_->UserAllLoad(tempDir);
	}

	{
		GameEngineDirectory dir;

		dir.MoveParent("FoxTeam");
		dir / "Resources" / "FBX" / "GH";

		std::string meshName = "Rio_Run.fbx";


		GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Bg_NaviMesh_Cobalt.fbx"));
		mesh->CreateRenderingBuffer();

		mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Rio_Run.fbx"));
		mesh->CreateRenderingBuffer();

		GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Rio_Run.fbx"));
		GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Rio_Wait.fbx"));

		//GameEngineFBXMeshManager::GetInst().LoadUser(dir.PathToPlusFileName("Rio_Wait.UserMesh"));
		//GameEngineFBXAnimationManager::GetInst().LoadUser(dir.PathToPlusFileName("Rio_Run.UserAnimation"));
		//GameEngineFBXAnimationManager::GetInst().LoadUser(dir.PathToPlusFileName("Rio_Wait.UserAnimation"));
	}
}

void LumiaLevel::releaseResource()
{
	GameEngineFBXMeshManager::GetInst().Delete("Bg_NaviMesh.fbx");
	GameEngineFBXMeshManager::GetInst().Delete("NaviCol.fbx");

	{
		GameEngineDirectory tempDir;
		tempDir.MoveParent("FoxTeam");
		tempDir / "Resources" / "FBX" / "UserMesh" / "Map";

		std::vector<GameEngineFile> vecFile = tempDir.GetAllFile(".UserMesh");

		for (size_t i = 0; i < vecFile.size(); i++)
		{
			GameEngineFBXMeshManager::GetInst().Delete(vecFile[i].FileName());
		}
	}

	{
		GameEngineDirectory tempDir;
		tempDir.MoveParent("FoxTeam");
		tempDir / "Resources" / "FBX" / "UserMesh" / "ItemBox";

		std::vector<GameEngineFile> vecFile = tempDir.GetAllFile(".UserMesh");
		vecFile = tempDir.GetAllFile(".UserMesh");

		for (size_t i = 0; i < vecFile.size(); i++)
		{
			GameEngineFBXMeshManager::GetInst().Delete(vecFile[i].FileName());
		}
	}

	GameEngineFBXMeshManager::GetInst().Delete("Bg_NaviMesh_Cobalt.fbx");
	GameEngineFBXMeshManager::GetInst().Delete("Rio_Run.fbx");

	GameEngineFBXAnimationManager::GetInst().Delete("Rio_Run.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("Rio_Wait.fbx");

	mouse_ = nullptr;
	lumiaMap = nullptr;
	ItemBox_ = nullptr;
}

void LumiaLevel::createActor()
{
	mouse_ = CreateActor<MousePointer>();
	lumiaMap = CreateActor<YSJ_LumiaMap>();

	player_ = CreateActor<GHRio>();

	SKySphereActor* Actor = CreateActor<SKySphereActor>();


	{
		LightActor* Actor;


		Actor = CreateActor<LightActor>();
		Actor->GetLight()->SetDiffusePower(1.f);
		Actor->GetLight()->SetAmbientPower(10.f);
		Actor->GetLight()->SetSpacularLightPow(10.f);

		//Actor = CreateActor<LightActor>();
		//Actor->GetLight()->SetDiffusePower(0.3f);
		//Actor->GetLight()->SetSpacularLightPow(50.0f);
		////Actor->GetTransform()->SetLocalRotationDegree({ 45.0f, 0.0f, 0.0f });

		//Actor = CreateActor<LightActor>();
		//Actor->GetLight()->SetDiffusePower(0.3f);
		//Actor->GetLight()->SetSpacularLightPow(50.0f);
		////Actor->GetTransform()->SetLocalRotationDegree({ 0.0f, 90.0f, 0.0f });
	}
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
		float4 Size = { 128, 72 };
		Window->PushRenderTarget("메인 카메라 타겟", GetMainCamera()->GetCameraRenderTarget(), Size * 3);
		Window->PushRenderTarget("UI 카메라 타겟", GetUICamera()->GetCameraRenderTarget(), Size * 3);
		Window->PushRenderTarget("메인 카메라 G-Buffer", GetMainCamera()->GetCameraDeferredGBufferTarget(), Size * 3);
		Window->PushRenderTarget("메인 카메라 디퍼드 라이트", GetMainCamera()->GetCameraDeferredLightTarget(), Size * 3);
		Window->PushRenderTarget("메인 카메라 디퍼드 타겟", GetMainCamera()->GetCameraDeferredTarget(), Size * 3);
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
