#include "PreCompile.h"
#include "UI_Skill.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"

#include "LH_Player.h"

UI_Skill::UI_Skill()
	: Time(1.0f), UIOn(false)
{
}

UI_Skill::~UI_Skill()
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

void UI_Skill::Start()
{
	//���ҽ����� SetImage�ϰ� ��ġ�� �����մϴ�
	//��ġ�� ������ ���ҽ����� UIRenderMap�� String�� ���� insert�˴ϴ�.
	//�ΰ�������(HP�� ���¹̳� ����, ������񿩺�)���� ������, UIRendererMap���� �̹����̸����� Find�ؼ� ���ҽ��� �ٲ� �������ϴ�.

	{
		//z���� �̿��� �տ����� �̹���/�ڿ����� �̹��� ������ ���ϰ� �ֽ��ϴ�.
		//��ġ������ �� float�� ���� �þ�� map�̳� vector�� ������ �����Դϴ�.

		float4 Skill_BackGroundPos = { -20.0f, -315.0f, 0.0f };


		{
			BackGroundRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
			BackGroundRenderer->SetImage("Skill_BackGround.png", "PointSmp");
			BackGroundRenderer->GetTransform()->SetLocalPosition(Skill_BackGroundPos);
			BackGroundRenderer->GetTransform()->SetLocalScaling(BackGroundRenderer->GetCurrentTexture()->GetTextureSize());
		//	UIRendererMap.insert(make_pair("Skill_BackGround", BackGroundRenderer));
		}
	}


	//����, Player ���� �Լ�
	//int HP = Player_->PlayerGetHP();
	//float Stmina = Player_->PlayerGetStamina();
	//Player_->PlayerSetHP(10);
	//Player_->PlayerSetStamina(100.f);


}

void UI_Skill::Update(float _Time)
{
	if (0.0f >= (Time -= _Time))
	{
		//TopRenderer->SetAlpha(Time);
	}
	
	//UI �¿��� üũ
	{
		//map<string, GameEngineUIRenderer*>::iterator FindIter = UIRendererMap.begin();

		if (false == UIOn)
		{
			BackGroundRenderer->Off();
			//for (; FindIter != UIRendererMap.end(); FindIter++)
			//{
			//	FindIter->second->Off();
			//}
		}
		else
		{
			BackGroundRenderer->On();
			//for (; FindIter != UIRendererMap.end(); FindIter++)
			//{
			//	FindIter->second->On();
			//}
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

