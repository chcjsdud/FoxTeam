#include "PreCompile.h"
#include "UI_HPBars.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"

UI_HPBars::UI_HPBars()
	: Time(1.0f), UIOn(true)
{
}

UI_HPBars::~UI_HPBars()
{
}


void UI_HPBars::Start()
{
	//Skill_BackGroundPos = { -20.0f, -315.0f, 0.0f };

	HpBarPos = { -19.0f, -335.0f, -1.0f };
	HpBarSize = {246.0f, 11.0f};

	SpBarPos = { -19.0f, -345.0f, -1.0f };
	SpBarSize = { 246.f, 8.f };

	{
		HPBar_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIICON);
		HPBar_Renderer->SetImage("HPBar_UI.png", "PointSmp");
		HPBar_Renderer->GetTransform()->SetLocalPosition(HpBarPos);
		HPBar_Renderer->GetTransform()->SetLocalScaling(HpBarSize);
	}

	{
		SPBar_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIICON);
		SPBar_Renderer->SetImage("SPBar_UI.png", "PointSmp");
		//배경보다 항상 앞에 나옴
		SPBar_Renderer->GetTransform()->SetLocalPosition(SpBarPos);
		SPBar_Renderer->GetTransform()->SetLocalScaling(SpBarSize);
	}

}

void UI_HPBars::Update(float _Time)
{
	
	//UI 온오프 체크
	{
		if (false == UIOn)
		{
			HPBar_Renderer->Off();
			SPBar_Renderer->Off();
		}
		else
		{
			HPBar_Renderer->On();
			SPBar_Renderer->On();
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

void UI_HPBars::SetStatus(CharacterStat* _Mystat)
{

	// 현재체력 / 최대체력을 해서 현재 체력 비율을 구함 
	float HPPercent = _Mystat->HP / _Mystat->HPMax;
	float SPPercent = _Mystat->SP / _Mystat->SPMax;

	//HP바의 X값을 비율에 맞춰서 축소시킴
	float4 NewHPBarSize = HpBarSize * float4{ HPPercent , 1.f, 1.f ,1.f };
	float4 NewSPBarSize = SpBarSize * float4{ SPPercent , 1.f, 1.f ,1.f };

	//작아진 크기의 절대값을 구해서 pivot을 맞춰준다
	//이미지 렌더링은 이미지 중심이 pivot이므로 작아진크기/2 만큼 이동시키면 pivot이 맞을것
	float4 NewHPBarPos = {HpBarPos.x - ((HpBarSize.x - NewHPBarSize.x) / 2.0f), HpBarPos.y, HpBarPos.z};
	float4 NewSPBarPos = { SpBarPos.x - ((SpBarSize.x - NewSPBarSize.x) / 2.0f), SpBarPos.y, SpBarPos.z };

	HPBar_Renderer->GetTransform()->SetLocalScaling(NewHPBarSize);
	HPBar_Renderer->GetTransform()->SetLocalPosition(NewHPBarPos);

	
	SPBar_Renderer->GetTransform()->SetLocalScaling(NewSPBarSize);
	SPBar_Renderer->GetTransform()->SetLocalPosition(NewSPBarPos);

}

