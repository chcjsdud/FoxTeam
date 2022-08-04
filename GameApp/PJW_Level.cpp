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
	Init_Resources();

	Init_Actors();

}

void PJW_Level::Init_Resources()
{	
	std::string MeshName = "Jackie_01_LOD1.FBX";
	GameEngineDirectory dir;

	dir.MoveParent("FoxTeam");
	dir.MoveChild("Resources");
	dir.MoveChild("FBX");
	dir.MoveChild("PJW");
	// 렌더러		
	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(dir.PathToPlusFileName(MeshName)))
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName(MeshName));
		Mesh->CreateRenderingBuffer();
	}

	GameEngineFBXAnimation* Animation = GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Jackie_01_LOD1.FBX"));

	for (size_t i = 0; i < Animation->AnimationCount(); i++)
	{
		auto Data = Animation->GetAnimationData(i);
	}
}



void PJW_Level::Init_Actors()
{
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

