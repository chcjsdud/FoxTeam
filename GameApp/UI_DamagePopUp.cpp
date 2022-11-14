#include "Precompile.h"
#include "UI_DamagePopUp.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include <GameEngine/GameEngineProgressBarRenderer.h>
#include "LumiaLevel.h"

UI_DamagePopUp::UI_DamagePopUp()
	: UIOn(true), MyChar(JobType::NONE)
{
}

UI_DamagePopUp::~UI_DamagePopUp()
{
}

void UI_DamagePopUp::Start()
{
	//HpBarPos = {0.0f, 0.0f};
	HpBarSize = { 70.0f, 9.0f };


	{
		HPBar_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIPANEL0);
		HPBar_Renderer->SetImage("HPBar_UI.png", "PointSmp");
		HPBar_Renderer->GetTransform()->SetLocalPosition(HpBarPos);
		HPBar_Renderer->GetTransform()->SetLocalScaling(HpBarSize);
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

void UI_DamagePopUp::SetFollowInfo(float4 _Pos, CharacterStat* _Stat)
{
	if (_Stat->HP <= 0.0f)
	{
		this->Off();
	}

	float HPPercent = _Stat->HP / _Stat->HPMax;

	HpBarPos = _Pos;
	//계산된 플레이어의 위치에 맞춰서 피봇을 맞춘다
	HpBarPos = HpBarPos + float4{0.f, 80.f, 0.f ,0.f};
	

	HPBar_Renderer->GetTransform()->SetLocalPosition(HpBarPos);
}

void UI_DamagePopUp::SetFollowInfoMonster(float4 _Pos, MonsterStateInfo _Stat)
{

	float HPPercent = _Stat.HP_ / _Stat.HPMax_;

	HpBarPos = _Pos;
	HpBarPos = HpBarPos + float4{ 0.f, 60.f, 0.f ,0.f };
	HPBar_Renderer->GetTransform()->SetLocalPosition(HpBarPos);
	if (_Stat.HP_ <= 0.0f)
	{
		this->Off();
	}

}