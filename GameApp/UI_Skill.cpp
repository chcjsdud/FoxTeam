#include "PreCompile.h"
#include "UI_Skill.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"

#include "LH_Player.h"

UI_Skill::UI_Skill()
	: Time(1.0f), UIOn(false)
{
}

UI_Skill::~UI_Skill()
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

void UI_Skill::Start()
{
	//리소스들을 SetImage하고 위치를 조정합니다
	//위치를 조정한 리소스들은 UIRenderMap에 String과 같이 insert됩니다.
	//인게임정보(HP나 스태미너 상태, 착용장비여부)등을 받으면, UIRendererMap에서 이미지이름으로 Find해서 리소스를 바꿀 예정업니다.

	{
		//z값을 이용해 앞에오는 이미지/뒤에오는 이미지 순서를 정하고 있습니다.
		//위치정보가 될 float도 양이 늘어나면 map이나 vector로 관리할 예정입니다.

		float4 Skill_BackGroundPos = { -20.0f, -315.0f, 0.0f };


		{
			BackGroundRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
			BackGroundRenderer->SetImage("Skill_BackGround.png", "PointSmp");
			BackGroundRenderer->GetTransform()->SetLocalPosition(Skill_BackGroundPos);
			BackGroundRenderer->GetTransform()->SetLocalScaling(BackGroundRenderer->GetCurrentTexture()->GetTextureSize());
		//	UIRendererMap.insert(make_pair("Skill_BackGround", BackGroundRenderer));
		}
	}


	//이현, Player 예제 함수
	//int HP = Player_->PlayerGetHP();
	//float Stmina = Player_->PlayerGetStamina();
	//Player_->PlayerSetHP(10);
	//Player_->PlayerSetStamina(100.f);


}

void UI_Skill::Update(float _Time)
{
	if (0.0f >= (Time -= _Time))
	{
		//TopRenderer->SetAlpha(Time);
	}
	
	//UI 온오프 체크
	{
		//map<string, GameEngineUIRenderer*>::iterator FindIter = UIRendererMap.begin();

		if (false == UIOn)
		{
			BackGroundRenderer->Off();
			//for (; FindIter != UIRendererMap.end(); FindIter++)
			//{
			//	FindIter->second->Off();
			//}
		}
		else
		{
			BackGroundRenderer->On();
			//for (; FindIter != UIRendererMap.end(); FindIter++)
			//{
			//	FindIter->second->On();
			//}
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

