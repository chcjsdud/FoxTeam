#include "PreCompile.h"
#include "LH_TopUI.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"

#include "LH_Player.h"

TopUI::TopUI()
	: Time(1.0f), UIOn(false)
{
}

TopUI::~TopUI()
{
	//for (auto& UIRenderer : UIRendererMap)
	//{
	//	if (true)
	//	{
	//		delete UIRenderer.second;
	//		UIRenderer.second = nullptr;
	//	}
	//}
}

void TopUI::Start()
{
	//���ҽ����� SetImage�ϰ� ��ġ�� �����մϴ�
	//��ġ�� ������ ���ҽ����� UIRenderMap�� String�� ���� insert�˴ϴ�.
	//�ΰ�������(HP�� ���¹̳� ����, ������񿩺�)���� ������, UIRendererMap���� �̹����̸����� Find�ؼ� ���ҽ��� �ٲ� �������ϴ�.
	{
		//z���� �̿��� �տ����� �̹���/�ڿ����� �̹��� ������ ���ϰ� �ֽ��ϴ�.
		//��ġ������ �� float�� ���� �þ�� map�̳� vector�� ������ �����Դϴ�.
		float4 Inventory_BackGroundPos = { 247.0f, -315.0f, 0.0f };
		float4 Skill_BackGroundPos = { -20.0f, -315.0f, 0.0f };
		float4 Eqiup_BackGroundPos = { -225.0f, -315.0f, 0.0f };
		float4 MainStatus_BackgroundPos = { -410.0f, -315.0f, 0.0f};
		float4 SubStatus_BackgroundPos = { -541.0f, -315.0f, 0.0f };
		float4 CharUI_Pos = { -305.0f, -308.0f, 1.0f };
		
		{
			TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
			TopRenderer->SetImage("Inventory_BackGround.png", "PointSmp");
			TopRenderer->GetTransform()->SetLocalPosition(Inventory_BackGroundPos);
			TopRenderer->GetTransform()->SetLocalScaling(TopRenderer->GetCurrentTexture()->GetTextureSize());
			UIRendererMap.insert(make_pair("Inventory_BackGround", TopRenderer));
		}

		{
			TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
			TopRenderer->SetImage("Skill_BackGround.png", "PointSmp");
			TopRenderer->GetTransform()->SetLocalPosition(Skill_BackGroundPos);
			TopRenderer->GetTransform()->SetLocalScaling(TopRenderer->GetCurrentTexture()->GetTextureSize());
			UIRendererMap.insert(make_pair("Skill_BackGround", TopRenderer));
		}

		{
			TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
			TopRenderer->SetImage("Eqiup_BackGround.png", "PointSmp");
			TopRenderer->GetTransform()->SetLocalPosition(Eqiup_BackGroundPos);
			TopRenderer->GetTransform()->SetLocalScaling(TopRenderer->GetCurrentTexture()->GetTextureSize());
			UIRendererMap.insert(make_pair("Eqiup_BackGround", TopRenderer));
		}

		{
			TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
			TopRenderer->SetImage("MainStatus_Background.png", "PointSmp");
			TopRenderer->GetTransform()->SetLocalPosition(MainStatus_BackgroundPos);
			TopRenderer->GetTransform()->SetLocalScaling(TopRenderer->GetCurrentTexture()->GetTextureSize());
			UIRendererMap.insert(make_pair("MainStatus_Background", TopRenderer));
		}

		{
			TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
			TopRenderer->SetImage("SubStatus_Background.png", "PointSmp");
			TopRenderer->GetTransform()->SetLocalPosition(SubStatus_BackgroundPos);
			TopRenderer->GetTransform()->SetLocalScaling(TopRenderer->GetCurrentTexture()->GetTextureSize());
			UIRendererMap.insert(make_pair("SubStatus_Background", TopRenderer));
		}

		{
			TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
			TopRenderer->SetImage("Hyunwoo_UI.png", "PointSmp");
			TopRenderer->GetTransform()->SetLocalPosition(CharUI_Pos);
			TopRenderer->GetTransform()->SetLocalScaling(TopRenderer->GetCurrentTexture()->GetTextureSize());
			UIRendererMap.insert(make_pair("Hyunwoo_UI", TopRenderer));
		}
	}


	//����, Player ���� �Լ�
	//int HP = Player_->PlayerGetHP();
	//float Stmina = Player_->PlayerGetStamina();
	//Player_->PlayerSetHP(10);
	//Player_->PlayerSetStamina(100.f);


}

void TopUI::Update(float _Time)
{
	if (0.0f >= (Time -= _Time))
	{
		//TopRenderer->SetAlpha(Time);
	}
	
	//UI �¿��� üũ
	{
		map<string, GameEngineUIRenderer*>::iterator FindIter = UIRendererMap.begin();

		if (false == UIOn)
		{
			for (; FindIter != UIRendererMap.end(); FindIter++)
			{
				FindIter->second->Off();
			}
		}
		else
		{
			for (; FindIter != UIRendererMap.end(); FindIter++)
			{
				FindIter->second->On();
			}
		}
	}

	if (true == GameEngineInput::GetInst().Down("Esc"))
	{
		if (UIOn == true)
		{
			UIOn = false;
		}
		else
		{
			UIOn = true;
		}
	}
}

