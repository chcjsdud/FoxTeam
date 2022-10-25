#include "PreCompile.h"
#include "UI_HPBars.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"

UI_HPBars::UI_HPBars()
	: Time(1.0f), UIOn(false)
{
}

UI_HPBars::~UI_HPBars()
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


void UI_HPBars::Start()
{
	//���ҽ����� SetImage�ϰ� ��ġ�� �����մϴ�
	//��ġ�� ������ ���ҽ����� UIRenderMap�� String�� ���� insert�˴ϴ�.
	//�ΰ�������(HP�� ���¹̳� ����, ������񿩺�)���� ������, UIRendererMap���� �̹����̸����� Find�ؼ� ���ҽ��� �ٲ� �������ϴ�.

	float4 CharUI_Pos = { -312.0f, -315.0f, 1.0f };

		//z���� �̿��� �տ����� �̹���/�ڿ����� �̹��� ������ ���ϰ� �ֽ��ϴ�.
		//��ġ������ �� float�� ���� �þ�� map�̳� vector�� ������ �����Դϴ�.

	{
		BackGroundRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		BackGroundRenderer->SetImage("Char_UI_Background.png", "PointSmp");
		BackGroundRenderer->GetTransform()->SetLocalPosition(CharUI_Pos);
		BackGroundRenderer->GetTransform()->SetLocalScaling(BackGroundRenderer->GetCurrentTexture()->GetTextureSize());
	}

	{
		CharRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		CharRenderer->SetImage("Char_UI_Hyunwoo.png", "PointSmp");
		//��溸�� �׻� �տ� ����
		CharRenderer->GetTransform()->SetLocalPosition(CharUI_Pos + float4{0.0f, 0.0f, -1.0f, 0.0f});
		CharRenderer->GetTransform()->SetLocalScaling(CharRenderer->GetCurrentTexture()->GetTextureSize());
	}

}

void UI_HPBars::Update(float _Time)
{
	if (0.0f >= (Time -= _Time))
	{
		//TopRenderer->SetAlpha(Time);
	}
	
	//UI �¿��� üũ
	{
		if (false == UIOn)
		{
			BackGroundRenderer->Off();
			CharRenderer->Off();
		}
		else
		{
			//BackGroundRenderer->On();
			//CharRenderer->On();
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

