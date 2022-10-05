#include "PreCompile.h"
#include "UI_CharPicture.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"

UI_CharPicture::UI_CharPicture()
	: Time(1.0f), UIOn(false)
{
}

UI_CharPicture::~UI_CharPicture()
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

void UI_CharPicture::SetJobType(JobType _myjob)
{
	switch (_myjob)
	{
	case JobType::NONE:
		break;
	case JobType::YUKI:
		break;
	case JobType::FIORA:
		break;
	case JobType::ZAHIR:
		break;
	case JobType::NADINE:
		break;
	case JobType::HYUNWOO:
		CharRenderer->SetImage("Char_UI_Hyunwoo.png", "PointSmp");
		break;
	case JobType::JACKIE:
		break;
	case JobType::RIO:
		CharRenderer->SetImage("Char_UI_Rio.png", "PointSmp");
		break;
	case JobType::AYA:
		break;
	case JobType::DUMMY:
		break;
	case JobType::MAX:
		break;
	default:
		break;
	}
}

void UI_CharPicture::Start()
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

	//����, Player ���� �Լ�
	//int HP = Player_->PlayerGetHP();
	//float Stmina = Player_->PlayerGetStamina();
	//Player_->PlayerSetHP(10);
	//Player_->PlayerSetStamina(100.f);


}

void UI_CharPicture::Update(float _Time)
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
			BackGroundRenderer->On();
			CharRenderer->On();
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

