#include "Precompile.h"
#include "UI_CharFollow.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include <GameEngine/GameEngineProgressBarRenderer.h>

UI_CharFollow::UI_CharFollow()
	: UIOn(false)
{
}

UI_CharFollow::~UI_CharFollow()
{
}

void UI_CharFollow::Start()
{
	HpBarPos = {0.0f, 0.0f};
	HpBarSize = { 70.0f, 9.0f };

	SpBarPos = {0.0f,0.0f};
	SpBarSize = { 70.f, 3.f };

	EmptyBarPos = {0.0f,0.0f };
	EmptyBarSize = {70.f, 13.f};

	{
		HPBar_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		HPBar_Renderer->SetImage("HPBar_UI.png", "PointSmp");
		//HPBar_Renderer->GetTransform()->SetLocalPosition(HpBarPos);
		HPBar_Renderer->GetTransform()->SetLocalScaling(HpBarSize);

		SPBar_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		SPBar_Renderer->SetImage("SPBar_UI.png", "PointSmp");
		SPBar_Renderer->GetTransform()->SetLocalScaling(SpBarSize);

		EmptyBar_Renderer = CreateTransformComponent<GameEngineProgressBarRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		EmptyBar_Renderer->SetImage("EmptyBar_UI.png", "PointSmp");
		EmptyBar_Renderer->GetTransform()->SetLocalScaling(EmptyBarSize);
		EmptyBar_Renderer->SetProgressBarDirect(static_cast<int>(ProgressBarDirect::RightToLeft));
		EmptyBar_Renderer->SetPercent(0.5f);
	}


}

void UI_CharFollow::Update(float _DeltaTime)
{
	{
		if (false == UIOn)
		{
			HPBar_Renderer->Off();
			SPBar_Renderer->Off();
			EmptyBar_Renderer->Off();
		}
		else
		{
			HPBar_Renderer->On();
			SPBar_Renderer->On();
			EmptyBar_Renderer->On();
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

void UI_CharFollow::SetFollowInfo(float4 _Pos, CharacterStat* _Stat) 
{

}
