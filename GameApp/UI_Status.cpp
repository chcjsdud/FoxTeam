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

	BasicValue_Pos = { -442.f, -279.f, -1.f };
	AttackSpeedValue_Pos = BasicValue_Pos + Value_YPivot;
	CriticalValue_Pos = BasicValue_Pos + (Value_YPivot * 2.0f);
	SkillDamageIncreaseValue_Pos = BasicValue_Pos + (Value_YPivot * 3.0f);

	DefenseValue_Pos = BasicValue_Pos + Value_XPivot;
	MoveSpeedValue_Pos = BasicValue_Pos + Value_YPivot + Value_XPivot;
	CooldownValue_Pos = BasicValue_Pos + (Value_YPivot * 2.0f) + Value_XPivot;
	SkillDamageReductionValue_Pos = BasicValue_Pos + (Value_YPivot * 3.0f) + Value_XPivot;


	//렌더러 초기화 모음
	{
		MainStatusRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		MainStatusRenderer->SetImage("MainStatus_Background.png", "PointSmp");
		MainStatusRenderer->GetTransform()->SetLocalPosition(MainStatus_BackgroundPos);
		MainStatusRenderer->GetTransform()->SetLocalScaling(MainStatusRenderer->GetCurrentTexture()->GetTextureSize());
		
		SubStatusRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		SubStatusRenderer->SetImage("SubStatus_Background.png", "PointSmp");
		SubStatusRenderer->GetTransform()->SetLocalPosition(SubStatus_BackgroundPos);
		SubStatusRenderer->GetTransform()->SetLocalScaling(SubStatusRenderer->GetCurrentTexture()->GetTextureSize());
		
		AttackValue_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		AttackValue_Renderer->GetTransform()->SetLocalPosition(BasicValue_Pos);
		
		DefenseValue_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		DefenseValue_Renderer->GetTransform()->SetLocalPosition(DefenseValue_Pos);
		
		AttackSpeedValue_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		AttackSpeedValue_Renderer->GetTransform()->SetLocalPosition(AttackSpeedValue_Pos);
		
		MoveSpeedValue_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		MoveSpeedValue_Renderer->GetTransform()->SetLocalPosition(MoveSpeedValue_Pos);
		
		CriticalValue_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		CriticalValue_Renderer->GetTransform()->SetLocalPosition(CriticalValue_Pos);

		CooldownValue_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		CooldownValue_Renderer->GetTransform()->SetLocalPosition(CooldownValue_Pos);

		SkillDamageIncreaseValue_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		SkillDamageIncreaseValue_Renderer->GetTransform()->SetLocalPosition(SkillDamageIncreaseValue_Pos);

		SkillDamageReductionValue_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		SkillDamageReductionValue_Renderer->GetTransform()->SetLocalPosition(SkillDamageReductionValue_Pos);
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
			MoveSpeedValue_Renderer->Off();
			CriticalValue_Renderer->Off();
			CooldownValue_Renderer->Off();
			SkillDamageIncreaseValue_Renderer->Off();
			SkillDamageReductionValue_Renderer->Off();
		}
		else
		{
			MainStatusRenderer->On();
			SubStatusRenderer->On();
			AttackValue_Renderer->On();
			DefenseValue_Renderer->On();
			AttackSpeedValue_Renderer->On();
			MoveSpeedValue_Renderer->On();
			CriticalValue_Renderer->On();
			CooldownValue_Renderer->On();
			SkillDamageIncreaseValue_Renderer->On();
			SkillDamageReductionValue_Renderer->On();
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

	//0.770000
	//to_string은 6자리까지 표현되므로 소수점 2자리까지만 표기되게 설정
	string AttackSpeed = to_string(UIStat->AttackSpeed);
	AttackSpeed.erase(4, 7);

	//크리티컬 확률, 쿨감 등은 뒤에 %가 붙음

	float CriticalChance = UIStat->CriticalChance;
	string CriticalChance_String = to_string(CriticalChance);
	AttachPercent(CriticalChance, CriticalChance_String);

	float CooldownChance = UIStat->CriticalChance;
	string CooldownChance_String = to_string(UIStat->CooldownReduction);
	AttachPercent(CriticalChance, CooldownChance_String);

	float SDIncrease = UIStat->SkillDamageAmplification;
	string SDIncrease_String = to_string(UIStat->SkillDamageAmplification);
	AttachPercent(SDIncrease, SDIncrease_String);

	float SDReduction = UIStat->SkillDamageAmplification;
	string SDReduction_String = to_string(UIStat->SkillAttackDamageReduction);
	AttachPercent(SDReduction, SDReduction_String);


	AttackValue_Renderer->TextSetting("HMKMRHD", to_string((int)(UIStat->AttackPower)), 12);
	DefenseValue_Renderer->TextSetting("HMKMRHD", to_string((int)(UIStat->Defence)), 12);

	AttackSpeedValue_Renderer->TextSetting("HMKMRHD", AttackSpeed, 12);
	MoveSpeedValue_Renderer->TextSetting("HMKMRHD", to_string((int)(UIStat->MovementSpeed)), 12);

	CriticalValue_Renderer->TextSetting("HMKMRHD", CriticalChance_String, 12);
	CooldownValue_Renderer->TextSetting("HMKMRHD", CooldownChance_String, 12);

	SkillDamageIncreaseValue_Renderer->TextSetting("HMKMRHD", SDIncrease_String, 12);
	SkillDamageReductionValue_Renderer->TextSetting("HMKMRHD", SDReduction_String, 12);
}

void UI_Status::AttachPercent(float _Value, string& _Result)
{
	//퍼센트 계산을 위해서 100을 곱함
	float value = _Value * 100.0f;


	if (value >= 100.0f)
	{
		//100퍼
		//100.000000
		_Result.erase(3,10);
	}
	else if (value >= 10.0f && value < 100.0f)
	{
		//10~99퍼
		//10.000000
		_Result.erase(2, 9);
	}
	else if (value < 10.0f)
	{
		//0~9퍼
		//0.000000
		_Result.erase(1, 8);
	}

	_Result += "%";

	return;
}