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
		//to_string= ���ڸ� string����
		int a = 0;
		//��������� x�� ������ ���� -> ���߾� 0,0 ���� x����� ����
		//�׻� ��溸�� �տ� ���;� �ϹǷ� z�ప�� -1
		//float4 NumberPos_ = { MainStatus_BackgroundPos.x *= -1.0f, MainStatus_BackgroundPos.y, MainStatus_BackgroundPos.z += -1.0f };
		//float4 pivot = {0.0f,0.0f};
		//NumberPos_ += pivot;
		AttackValue_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		AttackValue_Renderer->TextSetting("����", to_string(a), 100, float4::WHITE);
		AttackValue_Renderer->GetTransform()->SetLocalPosition({0.0f,0.0f,0.0f});
	}

	{
		DefenseValue_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		DefenseValue_Renderer->TextSetting("KBIZ�Ѹ������ M", "����", 100, float4::WHITE);
		DefenseValue_Renderer->GetTransform()->SetLocalPosition({ 100.0f, 100.0f, 0.0f, 0.0f });
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
			AttackValue_Renderer->Off();
			DefenseValue_Renderer->Off();
		}
		else
		{
			MainStatusRenderer->On();
			SubStatusRenderer->On();
			AttackValue_Renderer->On();
			DefenseValue_Renderer->On();
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

