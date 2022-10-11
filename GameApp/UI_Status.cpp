#include "PreCompile.h"
#include "UI_Status.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"

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

	float4 MainStatus_BackgroundPos = { -410.0f, -315.0f, 0.0f };
	float4 SubStatus_BackgroundPos = { -525.0f, -315.0f, 0.0f };

	Value_XPivot = {55.0f, 0.0f, 0.0f, 0.0f};
	Value_YPivot = {0.0f, -19.0f, 0.0f, 0.0f};
	AttackValue_Pos = { -442.f, -279.f, -1.f };

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
		//to_string= 숫자를 string으로
		AttackValue_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		AttackValue_Renderer->TextSetting("HMKMRHD", "a", 15);
		AttackValue_Renderer->GetTransform()->SetLocalPosition(AttackValue_Pos);
	}

	{
		DefenseValue_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		DefenseValue_Renderer->TextSetting("HMKMRHD", "a", 15);
		DefenseValue_Renderer->GetTransform()->SetLocalPosition(AttackValue_Pos + Value_XPivot);
	}

	{
		AttackSpeedValue_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		AttackSpeedValue_Renderer->TextSetting("HMKMRHD", "a", 15);
		AttackSpeedValue_Renderer->GetTransform()->SetLocalPosition(AttackValue_Pos + Value_YPivot);
	}


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
			AttackValue_Renderer->Off();
			DefenseValue_Renderer->Off();
			AttackSpeedValue_Renderer->Off();
			//MoveSpeedValue_Renderer->Off();
			//CriticalValue_Renderer->Off();
		}
		else
		{
			MainStatusRenderer->On();
			SubStatusRenderer->On();
			AttackValue_Renderer->On();
			DefenseValue_Renderer->On();
			AttackSpeedValue_Renderer->On();
			//MoveSpeedValue_Renderer->On();
			//CriticalValue_Renderer->On();
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

void UI_Status::SetStatus(CharacterStat* _Mystat)
{
	UIStat = _Mystat;


	AttackValue_Renderer->TextSetting("HMKMRHD", to_string((int)(UIStat->AttackPower)), 12);
	DefenseValue_Renderer->TextSetting("HMKMRHD", to_string((int)(UIStat->Defence)), 12);
	AttackSpeedValue_Renderer->TextSetting("HMKMRHD", to_string(UIStat->AttackSpeed), 12);
}