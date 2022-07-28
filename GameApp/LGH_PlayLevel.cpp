#include "PreCompile.h"
#include "LGH_PlayLevel.h"

#include <iostream>
#include "GameEngine/GameEngineFBXMeshManager.h"
#include "GameEngine/GameEngineFBXAnimationManager.h"
#include "GameEngine/GameEngineCore.h"

#include <GameEngine/CameraComponent.h>
#include <GameEngine/GameEngineTransform.h>
#include <GameEngine/CameraActor.h>
#include <GameEngine/LightActor.h>
#include <GameEngine/GameEngineGUI.h>
#include <GameEngine/SKySphereActor.h>
//#include "LH_Map.h"
#include <GameEngine/GameEngineFBXWindow.h>
#include <GameEngine/GameEngineFBXMesh.h>

#include <GameEngine/GameEngineImageRenderer.h>
#include "UserGame.h"

void LGH_PlayLevel::LevelStart()
{
	FBXFolder.MoveParent("FoxTeam");
	FBXFolder.MoveChild("Resources");
	FBXFolder.MoveChild("FBX");

	GetMainCamera()->SetProjectionMode(ProjectionMode::Perspective);
	GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.0f, 0.0f, -100.0f));

	

	GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(FBXFolder.PathToPlusFileName("LGH_Aya.fbx"));
	Mesh->CreateRenderingBuffer();

	if (false == GameEngineInput::GetInst().IsKey("PlayerMove"))
	{
		GameEngineInput::GetInst().CreateKey("FreeCameraOn", 'o');
	}

}

void LGH_PlayLevel::LevelUpdate(float _DeltaTime)
{
	if (true == GameEngineInput::GetInst().Down("FreeCameraOn"))
	{
		GetMainCameraActor()->FreeCameraModeSwitch();
	}
}

void LGH_PlayLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{

}

void LGH_PlayLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	/*
	{
		std::string MeshName = "Fox.FBX";

		//GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(FBXFolder.PathToPlusFileName(MeshName));
		//Mesh->CreateRenderingBuffer();

		GameEngineFBXMesh* Mesh1 = GameEngineFBXMeshManager::GetInst().Load(FBXFolder.PathToPlusFileName("Fox enemy.fbx"));
		Mesh1->CreateRenderingBuffer();

		GameEngineActor* NewActor = GameEngineCore::CurrentLevel()->CreateActor<GameEngineActor>();

		GameEngineFBXRenderer* Renderer = NewActor->CreateTransformComponent<GameEngineFBXRenderer>(NewActor->GetTransform());
		// Renderer->SetFBXMesh("Fox enemy.fbx", "TextureAni");
		// Renderer->SetFBXMesh("Fox.FBX", "Texture");
		Renderer->SetFBXMeshRenderSet("Fox enemy.fbx", "TextureAni", 0);
		Renderer->SetFBXMeshRenderSet("Fox enemy.fbx", "TextureAni", 6);

		Renderer->GetTransform()->SetLocalScaling({ 10.0f, 10.0f, 10.0f });


		for (UINT i = 0; i < Renderer->GetRenderSetCount(); i++)
		{
			Renderer->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "fox.Png");
		}

		GameEngineFBXAnimation* Animation = GameEngineFBXAnimationManager::GetInst().Load(FBXFolder.PathToPlusFileName("Fox enemy.fbx"));

		for (size_t i = 0; i < Animation->AnimationCount(); i++)
		{
			auto Data = Animation->GetAnimationData(i);
		}

		Renderer->CreateFBXAnimation("Sprint", "Fox enemy.fbx", 5);
		Renderer->CreateFBXAnimation("Attack", "Fox enemy.fbx", 0);
		Renderer->ChangeFBXAnimation("Sprint");

		return;
	}
	*/

	{
		std::string MeshName = "LGH_Aya.FBX";

		//GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(FBXFolder.PathToPlusFileName(MeshName));
		//Mesh->CreateRenderingBuffer();

	//	GameEngineFBXMesh* Mesh1 = GameEngineFBXMeshManager::GetInst().Load(FBXFolder.PathToPlusFileName("LGH_Aya.fbx"));
	//	Mesh1->CreateRenderingBuffer();

		GameEngineActor* NewActor = GameEngineCore::CurrentLevel()->CreateActor<GameEngineActor>();

		GameEngineFBXRenderer* Renderer = NewActor->CreateTransformComponent<GameEngineFBXRenderer>(NewActor->GetTransform());
		// Renderer->SetFBXMesh("Fox enemy.fbx", "TextureAni");
		// Renderer->SetFBXMesh("Fox.FBX", "Texture");
		Renderer->SetFBXMeshRenderSet("LGH_Aya.fbx", "TextureAni", 0);
		Renderer->SetFBXMeshRenderSet("LGH_Aya.fbx", "TextureAni", 1);

		Renderer->GetTransform()->SetLocalScaling({ 100.0f, 100.0f, 100.0f });


		GameEngineFBXAnimation* Animation = GameEngineFBXAnimationManager::GetInst().Load(FBXFolder.PathToPlusFileName("LGH_Aya.fbx"));

		for (size_t i = 0; i < Animation->AnimationCount(); i++)
		{
			auto Data = Animation->GetAnimationData(i);
		}

		//Renderer->CreateFBXAnimation("Sprint", "Fox enemy.fbx", 5);
		//Renderer->CreateFBXAnimation("Attack", "Fox enemy.fbx", 0);
		//Renderer->ChangeFBXAnimation("Sprint");

		return;
	}

	{

		LightActor* Actor;

		Actor = CreateActor<LightActor>();
		Actor->GetLight()->SetDiffusePower(0.3f);
		Actor->GetLight()->SetSpacularLightPow(50.0f);

		Actor = CreateActor<LightActor>();
		Actor->GetLight()->SetDiffusePower(0.3f);
		Actor->GetLight()->SetSpacularLightPow(50.0f);
		Actor->GetTransform()->SetLocalRotationDegree({ 45.0f, 0.0f, 0.0f });
	}

	{
		SKySphereActor* Actor = CreateActor<SKySphereActor>();
		//Actor->
	}

	{
		//	GameMouse* mouse = CreateActor<GameMouse>();
		//
		//	Player* Actor = CreateActor<Player>();
		//	Actor->SetParentMouse(mouse); // 플레이어 캐릭터를 종속시킬 마우스 커서를 알려줍니다. 게임매니저 생성 시 삭제될 함수
		//
		//	TestMonster* Monster = CreateActor<TestMonster>();
		//	Monster->GetTransform()->SetWorldPosition(float4{ 800.0f, 0.0f, 800.0f });
		//	Monster->SetTarget(Actor); // 게임매니저 생성으로 플레이어 위치정보 공유될 시 삭제될 함수입니다.

	}

	{
		//Map* Actor = CreateActor<Map>();
	}

	{
		//TopUI* Actor = CreateActor<TopUI>();
		//Actor->GetTransform()->SetWorldPosition(float4(0.0f, 0.0f, 0.0f));

		//플레이어로 이동
	}
}

LGH_PlayLevel::LGH_PlayLevel()
{

}

LGH_PlayLevel::~LGH_PlayLevel()
{

}
