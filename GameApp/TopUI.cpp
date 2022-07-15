#include "PreCompile.h"
#include "TopUI.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"

TopUI::TopUI()
	: Time(1.0f)
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
		float4 BackGroundBarPos = { -400.0f, -200.0f, 0.0f };
		float4 HPBarPos = { -395.0f, -202.0f, -1.0f };
		float4 HPBarTopPos = { -394.0f, -143.0f, -1.0f };
		float4 HPBarLinePos = { -401.0f, -180.0f, -2.0f};

		{
			TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
			TopRenderer->SetImage("BackGroundBar_Test.png", "PointSmp");
			TopRenderer->GetTransform()->SetLocalPosition(BackGroundBarPos);
			TopRenderer->GetTransform()->SetLocalScaling(TopRenderer->GetCurrentTexture()->GetTextureSize());
			UIRendererMap.insert(make_pair("BackGroundBar_Test", TopRenderer));
		}

		{
			TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
			TopRenderer->SetImage("HPBar_example.png", "PointSmp");
			TopRenderer->GetTransform()->SetLocalPosition(HPBarPos);
			TopRenderer->GetTransform()->SetLocalScaling(TopRenderer->GetCurrentTexture()->GetTextureSize());
			UIRendererMap.insert(make_pair("HPBar_example", TopRenderer));
		}

		{
			TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
			TopRenderer->SetImage("HPBar_Top.png", "PointSmp");
			TopRenderer->GetTransform()->SetLocalPosition(HPBarTopPos);
			TopRenderer->GetTransform()->SetLocalScaling(TopRenderer->GetCurrentTexture()->GetTextureSize());
			UIRendererMap.insert(make_pair("HPBar_Top", TopRenderer));
		}

		{
			TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
			TopRenderer->SetImage("Bar_Line.png", "PointSmp");
			TopRenderer->GetTransform()->SetLocalPosition(HPBarLinePos);
			TopRenderer->GetTransform()->SetLocalScaling(TopRenderer->GetCurrentTexture()->GetTextureSize());
			UIRendererMap.insert(make_pair("Bar_Line", TopRenderer));
		}
	}

}

void TopUI::Update(float _Time)
{
	if (0.0f >= (Time -= _Time))
	{
		//TopRenderer->SetAlpha(Time);
	}


//	TopRenderer->SetAlpha(0.1f);
}

