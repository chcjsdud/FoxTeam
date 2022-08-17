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
#include "UI_Inventory.h"
#include "UI_Equip.h"
#include "UI_Status.h"
#include "UI_ItemBox.h"

#include <GameEngine/GameEngineFBXWindow.h>
#include <GameEngine/GameEngineFBXMesh.h>

#include <GameEngine/GameEngineImageRenderer.h>
#include "UserGame.h"

void LGH_PlayLevel::LevelStart()
{

	GetMainCamera()->SetProjectionMode(ProjectionMode::Perspective);
	GetMainCameraActor()->GetCamera()->SetFov(60.f);
	GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.0f, 0.0f, -400.0f));
	{
		GameEngineDirectory FontDir;
		FontDir.MoveParent("FoxTeam");
		//������ / = MoveChild
		FontDir / "EngineResources" / "Font";
	
		string Fontname = FontDir.GetFullPath();
		Fontname += "\\KBIZM.ttf";
		//������ �Լ� AddFontResource
		//���� �ý��ۿ� ������ ��Ʈ�� �߰���Ű�� �Լ�
		AddFontResource(Fontname.c_str());
	}


	GameEngineFontManager::GetInst().Load("����");
	GameEngineFontManager::GetInst().Load("KBIZ�Ѹ������ M");

	//player_ = CreateActor<LGH_Aya>();


	{
		SkillUI_ = CreateActor<UI_Skill>();
	}

	{
		EquipUI_ = CreateActor<UI_Equip>();
	}

	{
		InventoryUI_ = CreateActor<UI_Inventory>();
	}


	{
		StatusUI_ = CreateActor<UI_Status>();
	}

	{
		ItemBoxUI_ = CreateActor<UI_ItemBox>();
	}


	{

		LightActor* Actor;



		Actor = CreateActor<LightActor>();
		Actor->GetLight()->SetDiffusePower(1.f);
		Actor->GetLight()->SetAmbientPower(10.f);
		Actor->GetLight()->SetSpacularLightPow(10.f);

	//	Actor = CreateActor<LightActor>();
	//	Actor->GetLight()->SetDiffusePower(0.3f);
	//	Actor->GetLight()->SetSpacularLightPow(50.0f);
	//	Actor->GetTransform()->SetLocalRotationDegree({ 45.0f, 0.0f, 0.0f });
	//
	//	Actor = CreateActor<LightActor>();
	//	Actor->GetLight()->SetDiffusePower(0.3f);
	//	Actor->GetLight()->SetSpacularLightPow(50.0f);
	//	Actor->GetTransform()->SetLocalRotationDegree({ 0.0f, 90.0f, 0.0f });
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


	

	//GetMainCameraActor()->FreeCameraModeSwitch();

}

void LGH_PlayLevel::LevelUpdate(float _DeltaTime)
{
	if (true == GameEngineInput::GetInst().Down("FreeCameraOn"))
	{
		GetMainCameraActor()->FreeCameraModeSwitch();
	}
	else
	{
		//float4 CharPosition_ = player_->GetTransform()->GetWorldPosition();
		//GetMainCameraActor()->GetTransform()->SetWorldPosition({ CharPosition_.x,  CharPosition_.y, -400.0f});
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
		GameEngineInput::GetInst().CreateKey("Esc", VK_ESCAPE);
		GameEngineInput::GetInst().CreateKey("Front", 'w');
	}
}

LGH_PlayLevel::LGH_PlayLevel()
{

}

LGH_PlayLevel::~LGH_PlayLevel()
{
	GameEngineDirectory FontDir;
	FontDir.MoveParent("FoxTeam");
	//������ / = MoveChild
	FontDir / "EngineResources" / "Font";

	string Fontname = FontDir.GetFullPath();
	Fontname += "\\KBIZM.ttf";
	//������ �Լ� RemoveFontResource
	//AddFontResource�� �߰��� ��Ʈ�� �����ϴ� �Լ�
	RemoveFontResource(Fontname.c_str());
}
