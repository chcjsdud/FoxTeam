#include "PreCompile.h"
#include "LGH_PlayLevel.h"

#include <iostream>
#include "GameEngine/GameEngineFBXMeshManager.h"
#include "GameEngine/GameEngineFBXAnimationManager.h"

#include <GameEngine/CameraComponent.h>
#include <GameEngine/GameEngineTransform.h>
#include <GameEngine/CameraActor.h>
#include <GameEngine/LightActor.h>
#include <GameEngine/GameEngineGUI.h>
#include <GameEngine/SKySphereActor.h>
#include <GameApp/LGH_Aya.h>
#include "UI_Skill.h"
//#include "LH_Map.h"
#include <GameEngine/GameEngineFBXWindow.h>
#include <GameEngine/GameEngineFBXMesh.h>

#include <GameEngine/GameEngineImageRenderer.h>
#include "UserGame.h"

void LGH_PlayLevel::LevelStart()
{

	GetMainCamera()->SetProjectionMode(ProjectionMode::Perspective);
	GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.0f, 0.0f, -100.0f));

	

	player_ = CreateActor<LGH_Aya>();

	{
		SkillUI_ = CreateActor<UI_Skill>();
		SkillUI_->GetTransform()->SetWorldPosition(float4(0.0f, 0.0f, 0.0f, 0.f));
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

		Actor = CreateActor<LightActor>();
		Actor->GetLight()->SetDiffusePower(0.3f);
		Actor->GetLight()->SetSpacularLightPow(50.0f);
		Actor->GetTransform()->SetLocalRotationDegree({ 0.0f, 90.0f, 0.0f });
	}



	{
		//SKySphereActor* Actor = CreateActor<SKySphereActor>();
		//Actor->
	}

	{
		//	GameMouse* mouse = CreateActor<GameMouse>();
		//
		//	Player* Actor = CreateActor<Player>();
		//	Actor->SetParentMouse(mouse); // �÷��̾� ĳ���͸� ���ӽ�ų ���콺 Ŀ���� �˷��ݴϴ�. ���ӸŴ��� ���� �� ������ �Լ�
		//
		//	TestMonster* Monster = CreateActor<TestMonster>();
		//	Monster->GetTransform()->SetWorldPosition(float4{ 800.0f, 0.0f, 800.0f });
		//	Monster->SetTarget(Actor); // ���ӸŴ��� �������� �÷��̾� ��ġ���� ������ �� ������ �Լ��Դϴ�.

	}

	{
		//Map* Actor = CreateActor<Map>();
	}

	{
		//TopUI* Actor = CreateActor<TopUI>();
		//Actor->GetTransform()->SetWorldPosition(float4(0.0f, 0.0f, 0.0f));

		//�÷��̾�� �̵�
	}


	

	GetMainCameraActor()->FreeCameraModeSwitch();

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
	if (false == GameEngineInput::GetInst().IsKey("PlayerMove"))
	{
		GameEngineInput::GetInst().CreateKey("FreeCameraOn", 'o');
	}
}

LGH_PlayLevel::LGH_PlayLevel()
{

}

LGH_PlayLevel::~LGH_PlayLevel()
{

}
