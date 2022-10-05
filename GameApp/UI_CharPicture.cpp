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
	//리소스들을 SetImage하고 위치를 조정합니다
	//위치를 조정한 리소스들은 UIRenderMap에 String과 같이 insert됩니다.
	//인게임정보(HP나 스태미너 상태, 착용장비여부)등을 받으면, UIRendererMap에서 이미지이름으로 Find해서 리소스를 바꿀 예정업니다.

	float4 CharUI_Pos = { -312.0f, -315.0f, 1.0f };

		//z값을 이용해 앞에오는 이미지/뒤에오는 이미지 순서를 정하고 있습니다.
		//위치정보가 될 float도 양이 늘어나면 map이나 vector로 관리할 예정입니다.

	{
		BackGroundRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		BackGroundRenderer->SetImage("Char_UI_Background.png", "PointSmp");
		BackGroundRenderer->GetTransform()->SetLocalPosition(CharUI_Pos);
		BackGroundRenderer->GetTransform()->SetLocalScaling(BackGroundRenderer->GetCurrentTexture()->GetTextureSize());
	}

	{
		CharRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		CharRenderer->SetImage("Char_UI_Hyunwoo.png", "PointSmp");
		//배경보다 항상 앞에 나옴
		CharRenderer->GetTransform()->SetLocalPosition(CharUI_Pos + float4{0.0f, 0.0f, -1.0f, 0.0f});
		CharRenderer->GetTransform()->SetLocalScaling(CharRenderer->GetCurrentTexture()->GetTextureSize());
	}

	//이현, Player 예제 함수
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
	
	//UI 온오프 체크
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

