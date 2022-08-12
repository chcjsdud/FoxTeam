#include "PreCompile.h"
#include "UI_Status.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"

#include "LH_Player.h"

UI_Status::UI_Status()
	: Time(1.0f), UIOn(false)
{
}

UI_Status::~UI_Status()
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

void UI_Status::Start()
{
	//���ҽ����� SetImage�ϰ� ��ġ�� �����մϴ�
	//��ġ�� ������ ���ҽ����� UIRenderMap�� String�� ���� insert�˴ϴ�.
	//�ΰ�������(HP�� ���¹̳� ����, ������񿩺�)���� ������, UIRendererMap���� �̹����̸����� Find�ؼ� ���ҽ��� �ٲ� �������ϴ�.

	float4 MainStatus_BackgroundPos = { -420.0f, -315.0f, 0.0f };
	float4 SubStatus_BackgroundPos = { -535.0f, -315.0f, 0.0f };

		//z���� �̿��� �տ����� �̹���/�ڿ����� �̹��� ������ ���ϰ� �ֽ��ϴ�.
		//��ġ������ �� float�� ���� �þ�� map�̳� vector�� ������ �����Դϴ�.
	{
		MainStatusRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		MainStatusRenderer->SetImage("MainStatus_Background.png", "PointSmp");
		MainStatusRenderer->GetTransform()->SetLocalPosition(MainStatus_BackgroundPos);
		MainStatusRenderer->GetTransform()->SetLocalScaling(MainStatusRenderer->GetCurrentTexture()->GetTextureSize());
	}

	{
		SubStatusRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		SubStatusRenderer->SetImage("SubStatus_Background.png", "PointSmp");
		SubStatusRenderer->GetTransform()->SetLocalPosition(SubStatus_BackgroundPos);
		SubStatusRenderer->GetTransform()->SetLocalScaling(SubStatusRenderer->GetCurrentTexture()->GetTextureSize());
	}

	{
		StatusAttack_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		StatusAttack_Renderer->TextSetting("�޸տ�ü","�׽�Ʈ",100, float4::WHITE);
		StatusAttack_Renderer->GetTransform()->SetLocalPosition({0.0f, 0.0f, 0.0f, 0.0f});
	}

	{
		StatusDefence_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		StatusDefence_Renderer->TextSetting("KBIZ�Ѹ������ M", "����", 100, float4::WHITE);
		StatusDefence_Renderer->GetTransform()->SetLocalPosition({ 0.0f, 100.0f, 0.0f, 0.0f });
	}


	//����, Player ���� �Լ�
	//int HP = Player_->PlayerGetHP();
	//float Stmina = Player_->PlayerGetStamina();
	//Player_->PlayerSetHP(10);
	//Player_->PlayerSetStamina(100.f);


}

void UI_Status::Update(float _Time)
{
	if (0.0f >= (Time -= _Time))
	{
		//TopRenderer->SetAlpha(Time);
	}
	
	//UI �¿��� üũ
	{
		if (false == UIOn)
		{
			MainStatusRenderer->Off();
			SubStatusRenderer->Off();
			StatusAttack_Renderer->Off();
			StatusDefence_Renderer->Off();
		}
		else
		{
			MainStatusRenderer->On();
			SubStatusRenderer->On();
			StatusAttack_Renderer->On();
			StatusDefence_Renderer->On();
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

