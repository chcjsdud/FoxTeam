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
	float4 Passive_IconSize = { 29.f,29.f };
	
	Skill_BackGroundPos = { -20.0f, -315.0f, 0.0f };
	Icon_Q_Pos = { -123.0f, -300.0f, -1.0f };
	Icon_W_Pos = { -80.0f, -300.0f, -1.0f };
	Icon_E_Pos = { -37.0f, -300.0f, -1.0f };
	Icon_R_Pos = { 7.0f, -300.0f, -1.0f };
	Icon_Weapon_Pos = { 50.0f, -300.0f, -1.0f };
	Icon_Passive_Pos = { 89.0f, -300.0f, -1.0f };

	{
		BackGroundRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::BACKDROP);
		BackGroundRenderer->SetImage("Skill_BackGround.png", "PointSmp");
		BackGroundRenderer->GetTransform()->SetLocalPosition(Skill_BackGroundPos);
		BackGroundRenderer->GetTransform()->SetLocalScaling(BackGroundRenderer->GetCurrentTexture()->GetTextureSize());
	}

	{
		Icon_Q = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIPANEL0);
		Icon_Q->GetTransform()->SetLocalScaling(Skill_IconSize);
		Icon_Q->GetTransform()->SetLocalPosition(Icon_Q_Pos);

		Icon_W = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIPANEL0);
		Icon_W->GetTransform()->SetLocalScaling(Skill_IconSize);
		Icon_W->GetTransform()->SetLocalPosition(Icon_W_Pos);

		Icon_E = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIPANEL0);
		Icon_E->GetTransform()->SetLocalScaling(Skill_IconSize);
		Icon_E->GetTransform()->SetLocalPosition(Icon_E_Pos);

		Icon_R = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIPANEL0);
		Icon_R->GetTransform()->SetLocalScaling(Skill_IconSize);
		Icon_R->GetTransform()->SetLocalPosition(Icon_R_Pos);

		Icon_Passive = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIPANEL0);
		Icon_Passive->GetTransform()->SetLocalScaling(Passive_IconSize);
		Icon_Passive->GetTransform()->SetLocalPosition(Icon_Passive_Pos);

		Icon_Weapon = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIPANEL0);
		Icon_Weapon->GetTransform()->SetLocalScaling(Skill_IconSize);
		Icon_Weapon->GetTransform()->SetLocalPosition(Icon_Weapon_Pos);

		Icon_Q_CoolCounter = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIPANEL1);
		Icon_Q_CoolCounter->SetImage("cooltime_mask.png", "PointSmp");
		Icon_Q_CoolCounter->GetTransform()->SetLocalScaling(Skill_IconSize);
		Icon_Q_CoolCounter->GetTransform()->SetLocalPosition(Icon_Q_Pos + float4{ 0.0f, 0.0f, -1.0f });
		Icon_Q_CoolCounter->SetTextColor(float4::WHITE);
		Icon_Q_CoolCounter->TextSetting("굴림", " ", 20, FW1_CENTER);
		Icon_Q_CoolCounter->Off();

		Icon_W_CoolCounter = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIPANEL1);
		Icon_W_CoolCounter->SetImage("cooltime_mask.png", "PointSmp");
		Icon_W_CoolCounter->GetTransform()->SetLocalScaling(Skill_IconSize);
		Icon_W_CoolCounter->GetTransform()->SetLocalPosition(Icon_W_Pos + float4{ 0.0f, 0.0f, -1.0f });
		Icon_W_CoolCounter->SetTextColor(float4::WHITE);
		Icon_W_CoolCounter->TextSetting("굴림", " ", 20, FW1_CENTER);
		Icon_W_CoolCounter->Off();

		Icon_E_CoolCounter = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIPANEL1);
		Icon_E_CoolCounter->SetImage("cooltime_mask.png", "PointSmp");
		Icon_E_CoolCounter->GetTransform()->SetLocalScaling(Skill_IconSize);
		Icon_E_CoolCounter->GetTransform()->SetLocalPosition(Icon_E_Pos + float4{ 0.0f, 0.0f, -1.0f });
		Icon_E_CoolCounter->SetTextColor(float4::WHITE);
		Icon_E_CoolCounter->TextSetting("굴림", " ", 20, FW1_CENTER);
		Icon_E_CoolCounter->Off();

		Icon_R_CoolCounter = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIPANEL1);
		Icon_R_CoolCounter->SetImage("cooltime_mask.png", "PointSmp");
		Icon_R_CoolCounter->GetTransform()->SetLocalScaling(Skill_IconSize);
		Icon_R_CoolCounter->GetTransform()->SetLocalPosition(Icon_R_Pos + float4{ 0.0f, 0.0f, -1.0f });
		Icon_R_CoolCounter->SetTextColor(float4::WHITE);
		Icon_R_CoolCounter->TextSetting("굴림", " ", 20, FW1_CENTER);
		Icon_R_CoolCounter->Off();

		Icon_Weapon_CoolCounter = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)RenderOrder::UIPANEL1);
		Icon_Weapon_CoolCounter->SetImage("cooltime_mask.png", "PointSmp");
		Icon_Weapon_CoolCounter->GetTransform()->SetLocalScaling(Skill_IconSize);
		Icon_Weapon_CoolCounter->GetTransform()->SetLocalPosition(Icon_Weapon_Pos + float4{ 0.0f, 0.0f, -1.0f });
		Icon_Weapon_CoolCounter->SetTextColor(float4::WHITE);
		Icon_Weapon_CoolCounter->TextSetting("굴림", " ", 20, FW1_CENTER);
		Icon_Weapon_CoolCounter->Off();
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
	{
		Icon_Q->SetImage("Yuki_Q_Icon.png", "PointSmp");
		Icon_W->SetImage("Yuki_W_Icon.png", "PointSmp");
		Icon_E->SetImage("Yuki_E_Icon.png", "PointSmp");
		Icon_R->SetImage("Yuki_R_Icon.png", "PointSmp");
		Icon_Passive->SetImage("Yuki_Passive_Icon.png", "PointSmp");
		Icon_Weapon->SetImage("Weapon_TwoHandedBlade_Icon.png", "PointSmp");
	}
		break;
	case JobType::FIORA:
	{
		Icon_Q->SetImage("Fiora_Q_Icon.png", "PointSmp");
		Icon_W->SetImage("Fiora_W_Icon.png", "PointSmp");
		Icon_E->SetImage("Fiora_E_Icon.png", "PointSmp");
		Icon_R->SetImage("Fiora_R_Icon.png", "PointSmp");
		Icon_Passive->SetImage("Fiora_Passive_Icon.png", "PointSmp");
		Icon_Weapon->SetImage("Weapon_TwoHandedBlade_Icon.png", "PointSmp");
	}
		break;
	case JobType::ZAHIR:
	{
		Icon_Q->SetImage("Zahir_Q_Icon.png", "PointSmp");
		Icon_W->SetImage("Zahir_W_Icon.png", "PointSmp");
		Icon_E->SetImage("Zahir_E_Icon.png", "PointSmp");
		Icon_R->SetImage("Zahir_R_Icon.png", "PointSmp");
		Icon_Passive->SetImage("Zahir_Passive_Icon.png", "PointSmp");
		Icon_Weapon->SetImage("Weapon_TwoHandedBlade_Icon.png", "PointSmp");
	}
		break;
	case JobType::NADINE:
	{
		Icon_Q->SetImage("Nadine_Q_Icon.png", "PointSmp");
		Icon_W->SetImage("Nadine_W_Icon.png", "PointSmp");
		Icon_E->SetImage("Nadine_E_Icon.png", "PointSmp");
		Icon_R->SetImage("Nadine_R_Icon.png", "PointSmp");
		Icon_Passive->SetImage("Nadine_Passive_Icon.png", "PointSmp");
		Icon_Weapon->SetImage("Weapon_TwoHandedBlade_Icon.png", "PointSmp");
	}
		break;
	case JobType::HYUNWOO:
	{
		Icon_Q->SetImage("HyunWoo_Q_Icon.png", "PointSmp");
		Icon_W->SetImage("HyunWoo_W_Icon.png", "PointSmp");
		Icon_E->SetImage("HyunWoo_E_Icon.png", "PointSmp");
		Icon_R->SetImage("HyunWoo_R_Icon.png", "PointSmp");
		Icon_Passive->SetImage("HyunWoo_Passive_Icon.png", "PointSmp");
		Icon_Weapon->SetImage("Weapon_Glove_Icon.png", "PointSmp");
	}
		break;
	case JobType::JACKIE:
	{
		Icon_Q->SetImage("Jackie_Q_Icon.png", "PointSmp");
		Icon_W->SetImage("Jackie_W_Icon.png", "PointSmp");
		Icon_E->SetImage("Jackie_E_Icon.png", "PointSmp");
		Icon_R->SetImage("Jackie_R_Icon.png", "PointSmp");
		Icon_Passive->SetImage("Jackie_Passive_Icon.png", "PointSmp");
		Icon_Weapon->SetImage("Weapon_Axe_Icon.png", "PointSmp");
	}
		break;
	case JobType::RIO:
	{
		//스킬아이콘 변경은 넣지 않기로 했음, 후일 쿨타임만 적용 예정
		Icon_Q->SetImage("Rio_Q_Icon.png", "PointSmp");
		Icon_W->SetImage("Rio_W_Icon.png", "PointSmp");
		Icon_E->SetImage("Rio_E_Icon.png", "PointSmp");
		Icon_R->SetImage("Rio_R_Icon.png", "PointSmp");
		Icon_Passive->SetImage("Rio_Passive_Icon.png", "PointSmp");
		Icon_Weapon->SetImage("Weapon_Bow_Icon.png", "PointSmp");
	}
		break;
	case JobType::AYA:
	{
		Icon_Q->SetImage("Aya_Q_Icon.png", "PointSmp");
		Icon_W->SetImage("Aya_W_Icon.png", "PointSmp");
		Icon_E->SetImage("Aya_E_Icon.png", "PointSmp");
		Icon_R->SetImage("Aya_R_Icon.png", "PointSmp");
		Icon_Passive->SetImage("Aya_Passive_Icon.png", "PointSmp");
		Icon_Weapon->SetImage("Weapon_TwoHandedBlade_Icon.png", "PointSmp");
	}
		break;
	case JobType::DUMMY:
		break;
	case JobType::MAX:
		break;
	default:
		break;
	}
}