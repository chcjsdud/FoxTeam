#include "PreCompile.h"
#include "UI_Skill.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"

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
	float4 Skill_IconSize = { 35.f,35.f };
	float4 Passive_IconSize = { 31.f,31.f };
	
	Skill_BackGroundPos = { -20.0f, -315.0f, 0.0f };
	Icon_Q_Pos = { -123.0f, -300.0f, -1.0f };
	Icon_W_Pos = { -80.0f, -300.0f, -1.0f };
	Icon_E_Pos = { -36.0f, -300.0f, -1.0f };
	Icon_R_Pos = { 7.0f, -300.0f, -1.0f };
	Icon_Weapon_Pos = { 50.0f, -300.0f, -1.0f };

	{
		BackGroundRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		BackGroundRenderer->SetImage("Skill_BackGround.png", "PointSmp");
		BackGroundRenderer->GetTransform()->SetLocalPosition(Skill_BackGroundPos);
		BackGroundRenderer->GetTransform()->SetLocalScaling(BackGroundRenderer->GetCurrentTexture()->GetTextureSize());
	}

	{
		Icon_Q = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Icon_Q->GetTransform()->SetLocalScaling(Skill_IconSize);
		Icon_Q->GetTransform()->SetLocalPosition(Icon_Q_Pos);

		Icon_W = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Icon_W->GetTransform()->SetLocalScaling(Skill_IconSize);
		Icon_W->GetTransform()->SetLocalPosition(Icon_W_Pos);

		Icon_E = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Icon_E->GetTransform()->SetLocalScaling(Skill_IconSize);
		Icon_E->GetTransform()->SetLocalPosition(Icon_E_Pos);

		Icon_R = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Icon_R->GetTransform()->SetLocalScaling(Skill_IconSize);
		Icon_R->GetTransform()->SetLocalPosition(Icon_R_Pos);

		Icon_Passive = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	

		Icon_Weapon = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		//Icon_Weapon->GetTransform()->SetLocalScaling(Skill_IconSize);
		//Icon_Weapon->GetTransform()->SetLocalPosition(Icon_Weapon_Pos);
	}



}

void UI_Skill::Update(float _Time)
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
			Icon_Q->Off();
			Icon_W->Off();
			Icon_E->Off();
			Icon_R->Off();
			Icon_Passive->Off();
			Icon_Weapon->Off();
		}
		else
		{
			BackGroundRenderer->On();
			Icon_Q->On();
			Icon_W->On();
			Icon_E->On();
			Icon_R->On();
			Icon_Passive->On();
			Icon_Weapon->On();
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

void UI_Skill::SetJobType(JobType _myjob)
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
	{
		Icon_Q->SetImage("HyunWoo_Q_Icon.png", "PointSmp");
		Icon_W->SetImage("HyunWoo_W_Icon.png", "PointSmp");
		Icon_E->SetImage("HyunWoo_E_Icon.png", "PointSmp");
		Icon_R->SetImage("HyunWoo_R_Icon.png", "PointSmp");
	}
		break;
	case JobType::JACKIE:
		break;
	case JobType::RIO:
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