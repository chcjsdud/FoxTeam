#include "Precompile.h"
#include "UI_DamagePopUp.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include <GameEngine/GameEngineProgressBarRenderer.h>
#include "LumiaLevel.h"

UI_DamagePopUp::UI_DamagePopUp()
	: UIOn(true), MyChar(JobType::NONE), Time_(0.5f)
{
}

UI_DamagePopUp::~UI_DamagePopUp()
{
}

void UI_DamagePopUp::DamageFontAppear(string _DamageValue, float4 _Pos)
{
	HPBar_Renderer->GetTransform()->SetLocalPosition(_Pos);
	HPBar_Renderer->TextSetting("±¼¸²", "Test", 13, FW1_CENTER, float4::RED);
	Time_ = 0.5f;
}

void UI_DamagePopUp::DamageFontAppearMonster(string _DamageValue, float4 _Pos)
{
}

void UI_DamagePopUp::Start()
{
	//HpBarPos = {0.0f, 0.0f};
	HpBarSize = { 70.0f, 9.0f };


	{
		HPBar_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::FONT);
		HPBar_Renderer->GetTransform()->SetLocalPosition(HpBarPos);

		HPBar_Renderer->TextSetting("±¼¸²", "Test", 11, FW1_CENTER, float4::RED);
	}

}

void UI_DamagePopUp::Update(float _DeltaTime)
{
	{
		if (false == UIOn)
		{
			HPBar_Renderer->Off();
		}
		else
		{
			HPBar_Renderer->On();
		}
	}
	
	Time_ -= _DeltaTime;
	
	if (Time_ <= 0.0f)
	{
		HPBar_Renderer->TextSetting("±¼¸²", "", 13, FW1_CENTER, float4::RED);
	}


}
