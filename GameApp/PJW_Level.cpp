#include "PreCompile.h"
#include "PJW_Level.h"
#include "UserGame.h"

#include "PJW_GameController.h"
#include "GameEngine/SKySphereActor.h"
#include "PJW_Hyunwoo.h"
#include "PJW_Map.h"
#include "GameEngine/LightActor.h"
#include <GameEngine/GameEngineFBXWindow.h>
#include <GameEngine/GameEngineGUI.h>
#include <GameEngine/GameEngineRenderWindow.h>

PJW_Level::PJW_Level() // default constructer 디폴트 생성자
	: gameController_(nullptr), player_(nullptr), map_(nullptr), light_A(nullptr), light_B(nullptr)
{

}

PJW_Level::~PJW_Level() // default destructer 디폴트 소멸자
{

}



void PJW_Level::LevelStart()
{

}

void PJW_Level::LevelUpdate(float _DeltaTime)
{
	static bool CreateActorCheck = false;

	static bool Check = false;

	if (false == Check && nullptr != GameEngineGUI::GetInst()->FindGUIWindow("RenderWindow"))
	{
		GameEngineRenderWindow* Window = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
		float4 Size = { 128, 72 };
		Window->PushRenderTarget("메인 카메라 타겟", GetMainCamera()->GetCameraRenderTarget(), Size * 3);
		Window->PushRenderTarget("디퍼드 G버퍼", GetMainCamera()->GetCameraDeferredGBufferTarget(), Size * 3);
		Window->PushRenderTarget("디퍼드 라이트", GetMainCamera()->GetCameraDeferredLightTarget(), Size * 3);
		Window->PushRenderTarget("디퍼드 G버터 + 라이트", GetMainCamera()->GetCameraDeferredTarget(), Size * 3);
		Check = true;
	}

	if (true == GameEngineInput::GetInst().Down("FreeCameraOn"))
	{
		GetMainCameraActor()->FreeCameraModeSwitch();
	}

	if (true == GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}

	float4 MoveDir = float4::ZERO;

	if (true == GameEngineInput::GetInst().Press("MoveLeft"))
	{
		MoveDir += GetMainCamera()->GetTransform()->GetWorldLeftVector();
	}
	if (true == GameEngineInput::GetInst().Press("MoveRight"))
	{
		MoveDir += GetMainCamera()->GetTransform()->GetWorldRightVector();
	}
	if (true == GameEngineInput::GetInst().Press("MoveForward"))
	{
		MoveDir += GetMainCamera()->GetTransform()->GetWorldForwardVector();
	}
	if (true == GameEngineInput::GetInst().Press("MoveBack"))
	{
		MoveDir += GetMainCamera()->GetTransform()->GetWorldBackVector();
	}

	MoveDir.Normalize3D();

	GetMainCamera()->GetTransform()->SetWorldDeltaTimeMove(MoveDir * 200.0f);
}

void PJW_Level::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{

}

void PJW_Level::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	GetMainCamera()->SetProjectionMode(ProjectionMode::Perspective);

	GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.0f, 400.0f, -100.0f));
	GetMainCameraActor()->GetTransform()->SetLocalRotationDegree(float4(60.0f, 0.0f, 0.0f));
	//Init_Resources();


	FBXWindow = GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineFBXWindow>("FBXWindow");

	// IMGUI 사용해 바이너리 세이브/로드 할려면 먼저 GUI에게 경로들 지정해주어야 함

	// FBXFolder : 가져 올 FBX 가 들어 있는 폴더
	FBXWindow->FBXFolder.MoveParent("FoxTeam");
	FBXWindow->FBXFolder.MoveChild("Resources");
	FBXWindow->FBXFolder.MoveChild("FBX");
	FBXWindow->FBXFolder.MoveChild("PJW");

	// UserMeshSaveFolder : 메시 바이너리 파일이 세이브될 폴더
	FBXWindow->UserMeshSaveFolder.MoveParent("FoxTeam");
	FBXWindow->UserMeshSaveFolder.MoveChild("Resources");
	FBXWindow->UserMeshSaveFolder.MoveChild("FBX");
	FBXWindow->UserMeshSaveFolder.MoveChild("UserMesh");

	// UserAnimationSaveFolder : 애니메이션 바이너리 파일이 세이브될 폴더
	FBXWindow->UserAnimationSaveFolder.MoveParent("FoxTeam");
	FBXWindow->UserAnimationSaveFolder.MoveChild("Resources");
	FBXWindow->UserAnimationSaveFolder.MoveChild("FBX");
	FBXWindow->UserAnimationSaveFolder.MoveChild("UserAni");

	Init_Actors();
}

void PJW_Level::Init_Resources()
{	
	std::string MeshName = "Hyunwoo_01_LOD1.FBX";

	GameEngineDirectory dir;

	dir.MoveParent("FoxTeam");
	dir.MoveChild("Resources");
	dir.MoveChild("FBX");
	dir.MoveChild("PJW");
	// 렌더러		
	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(dir.PathToPlusFileName("Hyunwoo_01_LOD1.FBX")))
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_01_LOD1.FBX"));
		Mesh->CreateRenderingBuffer();
	}

	GameEngineFBXAnimation* Animation = GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_01_LOD1_Run.FBX"));

	for (size_t i = 0; i < Animation->AnimationCount(); i++)
	{
		auto Data = Animation->GetAnimationData(i);
	}


	GameEngineFBXAnimation* Animation1 = GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_01_LOD1.FBX"));

	for (size_t i = 0; i < Animation1->AnimationCount(); i++)
	{
		auto Data = Animation1->GetAnimationData(i);
	}

}



void PJW_Level::Init_Actors()
{
	// 바이너리 파일 추출을 완료했다면 하단 예시처럼 액터의 메시와 애니메이션을 생성해주면 됩니다.

	//GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().LoadUser(FBXWindow->UserMeshSaveFolder.PathToPlusFileName("Hyunwoo_01_LOD1.UserMesh"));
	//GameEngineFBXAnimation* Animation = GameEngineFBXAnimationManager::GetInst().LoadUser(FBXWindow->UserAnimationSaveFolder.PathToPlusFileName("Hyunwoo_01_LOD1.UserAnimation"));
	//
	//GameEngineActor* NewActor = CreateActor<GameEngineActor>();
	//GameEngineFBXRenderer* Renderer = NewActor->CreateTransformComponent<GameEngineFBXRenderer>(NewActor->GetTransform());
	//
	//Renderer->SetFBXMesh(Mesh->GetName(), "TextureDeferredLightAni");
	//
	//for (size_t i = 0; i < Renderer->GetRenderSetCount(); i++)
	//{
	//	Renderer->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Hyunwoo_01_LOD1.png");
	//}
	//
	//Renderer->GetTransform()->SetLocalPosition({ 0.0f, -50.0f, 0.0f });
	//Renderer->GetTransform()->SetLocalScaling({ 30.0f, 30.0f, 30.0f });
	//Renderer->GetTransform()->SetLocalRotationDegree({ -90.0f, 0.0f, 0.0f });
	//
	//Renderer->CreateFBXAnimation("Idle", Animation->GetName(), 0); // 루프 애니메이션
	//Renderer->CreateFBXAnimation("Skill_R_Start", Animation->GetName(), 1, false); // 루프안함
	//Renderer->CreateFBXAnimation("Skill_R_Charge", Animation->GetName(), 2); // 루프안함
	//Renderer->CreateFBXAnimation("Skill_R_End", Animation->GetName(), 3, false); // 루프안함
	//Renderer->CreateFBXAnimation("Dash", Animation->GetName(), 5, false); // 루프안함
	//Renderer->CreateFBXAnimation("Skill_Q", Animation->GetName(), 8, false); // 루프안함
	//Renderer->CreateFBXAnimation("Attack0", Animation->GetName(), 10, false); // 루프안함
	//Renderer->CreateFBXAnimation("Attack1", Animation->GetName(), 11, false); // 루프안함
	//Renderer->CreateFBXAnimation("Skill_Weapon", Animation->GetName(), 12, false); // 루프안함
	//Renderer->ChangeFBXAnimation("Skill_R_Charge");
	//
	//
	gameController_ = CreateActor<PJW_GameController>();
	
	{
		SKySphereActor* Actor = CreateActor<SKySphereActor>();
	}

	// 빛 생성
	{
		light_A = CreateActor<LightActor>();
		light_A->GetLight()->SetDiffusePower(1.0f);
		light_A->GetLight()->SetSpacularLightPow(10.0f);
		light_A->GetLight()->SetAmbientPower(10.0f);
	}
}

