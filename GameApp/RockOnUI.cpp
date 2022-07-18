#include "PreCompile.h"
#include "RockOnUI.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"

#include "Player.h"

RockOnUI::RockOnUI()
	: Time(1.0f), UIOn(false)
{
}

RockOnUI::~RockOnUI()
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

void RockOnUI::Start()
{
	//�÷��̾ ������ Ÿ���� ������
	GameEngineActor* Target = Player_->PlayerGetTarget();

	/*
	�׽�Ʈ �̹����� TestRockOn.png �� ����Ͻø� �˴ϴ�.
	
	RockOnUI�� �÷��̾� Ŭ���� �ȿ��� �����˴ϴ�.
	�÷��̾��� ��ü �Լ��� RockOnUI Ŭ������ L_SHift�� ������ On() �ǰ� �� ������ Off() �˴ϴ�.
	�÷��̾ Ÿ������ ��ǥ�� ��ǥ�� �����ؼ� UI�� �Ÿ�, ȸ�� ��� ���� ������ ũ��� Ÿ������ ��µǱ� �ٶ��ϴ�.
	
	
	
	*/






	//���ҽ����� SetImage�ϰ� ��ġ�� �����մϴ�
	//��ġ�� ������ ���ҽ����� UIRenderMap�� String�� ���� insert�˴ϴ�.
	//�ΰ�������(HP�� ���¹̳� ����, ������񿩺�)���� ������, UIRendererMap���� �̹����̸����� Find�ؼ� ���ҽ��� �ٲ� �������ϴ�.
	//{
	//	//z���� �̿��� �տ����� �̹���/�ڿ����� �̹��� ������ ���ϰ� �ֽ��ϴ�.
	//	//��ġ������ �� float�� ���� �þ�� map�̳� vector�� ������ �����Դϴ�.
	//	float4 BackGroundBarPos = { -400.0f, -200.0f, 0.0f };
	//	float4 HPBarPos = { -395.0f, -202.0f, -1.0f };
	//	float4 HPBarTopPos = { -394.0f, -143.0f, -1.0f };
	//	float4 HPBarLinePos = { -401.0f, -180.0f, -2.0f};
	//	
	//	{
	//		TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	//		TopRenderer->SetImage("BackGroundBar_Test.png", "PointSmp");
	//		TopRenderer->GetTransform()->SetLocalPosition(BackGroundBarPos);
	//		TopRenderer->GetTransform()->SetLocalScaling(TopRenderer->GetCurrentTexture()->GetTextureSize());
	//		UIRendererMap.insert(make_pair("BackGroundBar_Test", TopRenderer));
	//	}

	//	{
	//		TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	//		TopRenderer->SetImage("HPBar_example.png", "PointSmp");
	//		TopRenderer->GetTransform()->SetLocalPosition(HPBarPos);
	//		TopRenderer->GetTransform()->SetLocalScaling(TopRenderer->GetCurrentTexture()->GetTextureSize());
	//		UIRendererMap.insert(make_pair("HPBar_example", TopRenderer));
	//	}

	//	{
	//		TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	//		TopRenderer->SetImage("HPBar_Top.png", "PointSmp");
	//		TopRenderer->GetTransform()->SetLocalPosition(HPBarTopPos);
	//		TopRenderer->GetTransform()->SetLocalScaling(TopRenderer->GetCurrentTexture()->GetTextureSize());
	//		UIRendererMap.insert(make_pair("HPBar_Top", TopRenderer));
	//	}

	//	{
	//		TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	//		TopRenderer->SetImage("Bar_Line.png", "PointSmp");
	//		TopRenderer->GetTransform()->SetLocalPosition(HPBarLinePos);
	//		TopRenderer->GetTransform()->SetLocalScaling(TopRenderer->GetCurrentTexture()->GetTextureSize());
	//		UIRendererMap.insert(make_pair("Bar_Line", TopRenderer));
	//	}
	//}


	//����, Player ���� �Լ�
	//int HP = Player_->PlayerGetHP();
	//float Stmina = Player_->PlayerGetStamina();
	//Player_->PlayerSetHP(10);
	//Player_->PlayerSetStamina(100.f);


}

void RockOnUI::Update(float _Time)
{
	//if (0.0f >= (Time -= _Time))
	//{
	//	//TopRenderer->SetAlpha(Time);
	//}
	//
	////UI �¿��� üũ
	//{
	//	map<string, GameEngineUIRenderer*>::iterator FindIter = UIRendererMap.begin();

	//	if (false == UIOn)
	//	{
	//		for (; FindIter != UIRendererMap.end(); FindIter++)
	//		{
	//			FindIter->second->Off();
	//		}
	//	}
	//	else
	//	{
	//		for (; FindIter != UIRendererMap.end(); FindIter++)
	//		{
	//			FindIter->second->On();
	//		}
	//	}
	//}

	//if (true == GameEngineInput::GetInst().Down("Esc"))
	//{
	//	if (UIOn == true)
	//	{
	//		UIOn = false;
	//	}
	//	else
	//	{
	//		UIOn = true;
	//	}
	//}
}

