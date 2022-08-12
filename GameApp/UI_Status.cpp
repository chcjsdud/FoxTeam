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
	//리소스들을 SetImage하고 위치를 조정합니다
	//위치를 조정한 리소스들은 UIRenderMap에 String과 같이 insert됩니다.
	//인게임정보(HP나 스태미너 상태, 착용장비여부)등을 받으면, UIRendererMap에서 이미지이름으로 Find해서 리소스를 바꿀 예정업니다.

	float4 MainStatus_BackgroundPos = { -420.0f, -315.0f, 0.0f };
	float4 SubStatus_BackgroundPos = { -535.0f, -315.0f, 0.0f };

		//z값을 이용해 앞에오는 이미지/뒤에오는 이미지 순서를 정하고 있습니다.
		//위치정보가 될 float도 양이 늘어나면 map이나 vector로 관리할 예정입니다.
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
		StatusAttack_Renderer->TextSetting("휴먼옛체","테스트",100, float4::WHITE);
		StatusAttack_Renderer->GetTransform()->SetLocalPosition({0.0f, 0.0f, 0.0f, 0.0f});
	}

	{
		StatusDefence_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		StatusDefence_Renderer->TextSetting("KBIZ한마음고딕 M", "시험", 100, float4::WHITE);
		StatusDefence_Renderer->GetTransform()->SetLocalPosition({ 0.0f, 100.0f, 0.0f, 0.0f });
	}


	//이현, Player 예제 함수
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
	
	//UI 온오프 체크
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

