#include "PreCompile.h"
#include "YSJ_PlayLevel.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngine/LightActor.h>
#include "UserGame.h"
#include <GameEngine/SKySphereActor.h>
#include <GameEngine/GameEngineRenderWindow.h>

#include "YSJ_LumiaMap.h"
#include "YSJ_Player.h"
#include "YSJ_Char.h"
#include "YSJ_Mouse.h"
#include "NaviMesh.h"
#include "NaviMeshWindow.h"

YSJ_PlayLevel::YSJ_PlayLevel()
	: NaviMesh_(nullptr)
	, Player_(nullptr)
	, NaviWindow_(nullptr)
{

}

YSJ_PlayLevel::~YSJ_PlayLevel()
{

}

void YSJ_PlayLevel::LevelStart()
{
	GetMainCamera()->SetProjectionMode(ProjectionMode::Perspective);
	GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.0f, 300.0f, -300.0f));
	GetMainCameraActor()->GetTransform()->AddLocalRotationDegreeX(30.0f);

	GameEngineInput::GetInst().CreateKey("MoveLeft", 'A');
	GameEngineInput::GetInst().CreateKey("MoveRight", 'D');
	GameEngineInput::GetInst().CreateKey("MoveForward", 'W');
	GameEngineInput::GetInst().CreateKey("MoveBack", 'S');
	GameEngineInput::GetInst().CreateKey("MoveUp", 'Q');
	GameEngineInput::GetInst().CreateKey("MoveDown", 'E');
	GameEngineInput::GetInst().CreateKey("FreeCameraOn", 'o');
	GameEngineInput::GetInst().CreateKey("LBUTTON", VK_LBUTTON);

	Player_ = CreateActor<YSJ_Player>();
	Player_->GetTransform()->SetWorldMove({ 10.0f, 0.0f, 0.0f });

	GameEngineDirectory tempDir;

	tempDir.MoveParent("FoxTeam");
	tempDir.MoveChild("Resources");
	tempDir.MoveChild("FBX");
	tempDir.MoveChild("YSJ");

	//std::vector<GameEngineFile> vecFile = tempDir.GetAllFile("FBX");

	//for (size_t i = 0; i < vecFile.size(); i++)
	//{
	//	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(vecFile[i].GetFullPath()))
	//	{
	//		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(vecFile[i].GetFullPath());
	//		Mesh->CreateRenderingBuffer();
	//	}
	//}

	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(tempDir.PathToPlusFileName("Bg_NaviMesh.fbx")))
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(tempDir.PathToPlusFileName("Bg_NaviMesh.fbx"));
		Mesh->CreateRenderingBuffer();
	}

	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(tempDir.PathToPlusFileName("NaviCol.fbx")))
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(tempDir.PathToPlusFileName("NaviCol.fbx"));
		Mesh->CreateRenderingBuffer();
	}

	//if (nullptr == GameEngineFBXMeshManager::GetInst().Find(tempDir.PathToPlusFileName("School.fbx")))
	//{
	//	GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(tempDir.PathToPlusFileName("School.fbx"));
	//	Mesh->CreateRenderingBuffer();
	//}
	//
	//if (nullptr == GameEngineFBXMeshManager::GetInst().Find(tempDir.PathToPlusFileName("Downtown.fbx")))
	//{
	//	GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(tempDir.PathToPlusFileName("Downtown.fbx"));
	//	Mesh->CreateRenderingBuffer();
	//}

	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(tempDir.PathToPlusFileName("TestBox.fbx")))
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(tempDir.PathToPlusFileName("TestBox.fbx"));
		Mesh->CreateRenderingBuffer();
	}

	tempDir.MoveParent("FoxTeam");
	tempDir.MoveChild("EngineResources");
	tempDir.MoveChild("FBX");

	//if (nullptr == GameEngineFBXMeshManager::GetInst().Find(tempDir.PathToPlusFileName("AnimMan.fbx")))
	//{
	//	GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(tempDir.PathToPlusFileName("AnimMan.fbx"));
	//	Mesh->CreateRenderingBuffer();
	//	GameEngineFBXAnimationManager::GetInst().Load(tempDir.PathToPlusFileName("ALS_N_RUN_F.FBX"));
	//}


	//YSJ_Char* Player = CreateActor<YSJ_Char>();
	//Player->GetTransform()->SetWorldScaling({0.5f,0.5f, 0.5f });

	if (false == GameEngineInput::GetInst().IsKey("FreeCam"))
	{
		GameEngineInput::GetInst().CreateKey("FreeCam", 'o');
	}

	GetMainCameraActor()->FreeCameraModeSwitch();

	NaviWindow_ = GameEngineGUI::GetInst()->CreateGUIWindow<NaviMeshWindow>("NaviWindow");
}

void YSJ_PlayLevel::LevelUpdate(float _DeltaTime)
{
	static bool isLoaded = false;

	if (0 >= UserGame::LoadingFolder &&
		false == isLoaded)
	{
		CreateActorLevel();
		isLoaded = true;
	}

	if (true == GameEngineInput::GetInst().Down("FreeCam"))
	{
		GetMainCameraActor()->FreeCameraModeSwitch();
	}

	if (nullptr == Player_->GetCurrentNavi() && true == isLoaded)
	{
		Player_->SetCurrentNavi(NaviMesh_->CurrentCheck(Player_->GetTransform(), float4::DOWN));
	}
}

void YSJ_PlayLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
}

void YSJ_PlayLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	if (nullptr != GameEngineGUI::GetInst()->FindGUIWindow("RenderWindow"))
	{
		GameEngineRenderWindow* Window = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
		float4 Size = { 128, 72 };
		Window->PushRenderTarget("메인 카메라 타겟", GetMainCamera()->GetCameraRenderTarget(), Size * 3);
		Window->PushRenderTarget("UI 카메라 타겟", GetUICamera()->GetCameraRenderTarget(), Size * 3);
		Window->PushRenderTarget("메인 카메라 디퍼드", GetMainCamera()->GetCameraDeferredGBufferTarget(), Size * 3);
		Window->PushRenderTarget("메인 카메라 디퍼드 라이트", GetMainCamera()->GetCameraDeferredLightTarget(), Size * 3);
		Window->PushRenderTarget("메인 카메라 디퍼드 라이트", GetMainCamera()->GetCameraDeferredTarget(), Size * 3);
	}
}

void YSJ_PlayLevel::CreateActorLevel()
{
	LightActor* Light1 = CreateActor<LightActor>();
	Light1->GetLight()->SetDiffusePower(0.3f);
	Light1->GetLight()->SetSpacularLightPow(50.0f);

	LightActor* Light2 = CreateActor<LightActor>();
	Light2->GetLight()->SetDiffusePower(0.3f);
	Light2->GetLight()->SetSpacularLightPow(50.0f);
	Light2->GetTransform()->SetLocalRotationDegree(float4(0.0f, 90.0f, 0.0f));

	LightActor* Light3 = CreateActor<LightActor>();
	Light3->GetLight()->SetDiffusePower(0.3f);
	Light3->GetLight()->SetSpacularLightPow(50.0f);
	Light3->GetTransform()->SetLocalRotationDegree(float4(45.0f, 0.0f, 0.0f));

	SKySphereActor* SkyActor = CreateActor<SKySphereActor>();
	YSJ_LumiaMap* LumiaMap = CreateActor<YSJ_LumiaMap>();
	YSJ_Mouse* Mouse = CreateActor<YSJ_Mouse>();
	Mouse->SetPickingRenderer(LumiaMap->GetFBXNaviRenderer());

	NaviMesh_ = CreateActor<NaviMesh>();

	std::vector<GameEngineVertex> Vertex = std::vector<GameEngineVertex>(3 * 6);

	Vertex[0] = { float4({ -50.0f, 0.0f, -50.0f }),  { 0.0f, 0.0f } };
	Vertex[1] = { float4({ 50.0f, 0.0f, -50.0f }),  { 1.0f, 0.0f } };
	Vertex[2] = { float4({ 0.0f, 0.0f, 50.0f }),  { 0.0f, 1.0f } };

	Vertex[3] = { float4::RotateYDegree(Vertex[0].POSITION, 180.0f) + float4({ 50.0f, 0.0f, 0.0f, 0.0f }),	{ 0.0f, 0.0f }};
	Vertex[4] = { float4::RotateYDegree(Vertex[1].POSITION, 180.0f) + float4({ 50.0f, 0.0f, 0.0f, 0.0f }),	{ 1.0f, 0.0f }};
	Vertex[5] = { float4::RotateYDegree(Vertex[2].POSITION, 180.0f) + float4({ 50.0f, 0.0f, 0.0f, 0.0f }),	{ 0.0f, 1.0f }};

	Vertex[6] = { Vertex[0].POSITION + float4({ 100.0f, 0.0f, 0.0f, 0.0f }),	{ 0.0f, 0.0f } };
	Vertex[7] = { Vertex[1].POSITION + float4({ 100.0f, 0.0f, 0.0f, 0.0f }),	{ 1.0f, 0.0f } };
	Vertex[8] = { Vertex[2].POSITION + float4({ 100.0f, 0.0f, 0.0f, 0.0f }),	{ 0.0f, 1.0f } };

	Vertex[9] =  { Vertex[3].POSITION + float4({ 100.0f, 50.0f, 0.0f, 0.0f }),	{ 0.0f, 0.0f } };
	Vertex[10] = { Vertex[4].POSITION + float4({ 100.0f, 0.0f, 0.0f, 0.0f }),	{ 1.0f, 0.0f } };
	Vertex[11] = { Vertex[5].POSITION + float4({ 100.0f, 0.0f, 0.0f, 0.0f }),	{ 0.0f, 1.0f } };

	Vertex[12] = { Vertex[0].POSITION + float4({ 200.0f, 0.0f, 0.0f, 0.0f }),	{ 0.0f, 0.0f } };
	Vertex[13] = { Vertex[1].POSITION + float4({ 200.0f, 50.0f, 0.0f, 0.0f }),	{ 1.0f, 0.0f } };
	Vertex[14] = { Vertex[2].POSITION + float4({ 200.0f, 50.0f, 0.0f, 0.0f }),	{ 0.0f, 1.0f } };

	Vertex[15] = { Vertex[3].POSITION + float4({ 200.0f, 50.0f, 0.0f, 0.0f }),	{ 0.0f, 0.0f } };
	Vertex[16] = { Vertex[4].POSITION + float4({ 200.0f, 50.0f, 0.0f, 0.0f }),	{ 1.0f, 0.0f } };
	Vertex[17] = { Vertex[5].POSITION + float4({ 200.0f, 50.0f, 0.0f, 0.0f }),	{ 0.0f, 1.0f } };

	std::vector<UINT> Index;

	for (int i = 0; i < 6; i++)
	{
		Index.push_back(i * 3 + 2);
		Index.push_back(i * 3 + 1);
		Index.push_back(i * 3 + 0);
	}

	//NaviMesh_->CreateNaviMesh(Vertex, Index);
	NaviMesh_->CreateNaviMesh(LumiaMap->GetFBXNaviRenderer());
	NaviMesh_->GetTransform()->SetWorldScaling({ 3.0f, 3.0f, 3.0f });
	Player_->SetNaviMesh(NaviMesh_);

	NaviWindow_->Navi = NaviMesh_;
	NaviWindow_->FBXRenderer = LumiaMap->GetFBXNaviRenderer();
}
