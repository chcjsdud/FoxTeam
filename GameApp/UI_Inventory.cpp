#include "PreCompile.h"
#include "UI_Inventory.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"

#include "LH_Player.h"

UI_Inventory::UI_Inventory()
	: Time(1.0f), UIOn(false)
{
}

UI_Inventory::~UI_Inventory()
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

void UI_Inventory::Start()
{
	//���ҽ����� SetImage�ϰ� ��ġ�� �����մϴ�
	//��ġ�� ������ ���ҽ����� UIRenderMap�� String�� ���� insert�˴ϴ�.
	//�ΰ�������(HP�� ���¹̳� ����, ������񿩺�)���� ������, UIRendererMap���� �̹����̸����� Find�ؼ� ���ҽ��� �ٲ� �������ϴ�.
	{
		//z���� �̿��� �տ����� �̹���/�ڿ����� �̹��� ������ ���ϰ� �ֽ��ϴ�.
		//��ġ������ �� float�� ���� �þ�� map�̳� vector�� ������ �����Դϴ�.
		float4 Inventory_BackGroundPos = { 247.0f, -315.0f, 0.0f };
		
		{
			Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
			Renderer->SetImage("Inventory_BackGround.png", "PointSmp");
			Renderer->GetTransform()->SetLocalPosition(Inventory_BackGroundPos);
			Renderer->GetTransform()->SetLocalScaling(Renderer->GetCurrentTexture()->GetTextureSize());
		}

	}


	//����, Player ���� �Լ�
	//int HP = Player_->PlayerGetHP();
	//float Stmina = Player_->PlayerGetStamina();
	//Player_->PlayerSetHP(10);
	//Player_->PlayerSetStamina(100.f);


}

void UI_Inventory::Update(float _Time)
{
	if (0.0f >= (Time -= _Time))
	{
		//TopRenderer->SetAlpha(Time);
	}
	
	//UI �¿��� üũ
	{
		if (false == UIOn)
		{
			Renderer->Off();
		}
		else
		{

			Renderer->On();
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
