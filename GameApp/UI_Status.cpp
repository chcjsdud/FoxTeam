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
	//���ҽ����� SetImage�ϰ� ��ġ�� �����մϴ�
	//��ġ�� ������ ���ҽ����� UIRenderMap�� String�� ���� insert�˴ϴ�.
	//�ΰ�������(HP�� ���¹̳� ����, ������񿩺�)���� ������, UIRendererMap���� �̹����̸����� Find�ؼ� ���ҽ��� �ٲ� �������ϴ�.

	float4 MainStatus_BackgroundPos = { -410.0f, -315.0f, 0.0f };
	float4 SubStatus_BackgroundPos = { -525.0f, -315.0f, 0.0f };

	Value_XPivot = {55.0f, 0.0f, 0.0f, 0.0f};
	Value_YPivot = {0.0f, -19.0f, 0.0f, 0.0f};
	float4 SubStat_XPivot = { 50.0f, 0.0f, 0.0f, 0.0f };


	BasicValue_Pos = { -442.f, -279.f, -1.f };
	AttackSpeedValue_Pos = BasicValue_Pos + Value_YPivot;
	CriticalValue_Pos = BasicValue_Pos + (Value_YPivot * 2.0f);
	SkillDamageIncreaseValue_Pos = BasicValue_Pos + (Value_YPivot * 3.0f);

	DefenseValue_Pos = BasicValue_Pos + Value_XPivot;
	MoveSpeedValue_Pos = BasicValue_Pos + Value_YPivot + Value_XPivot;
	CooldownValue_Pos = BasicValue_Pos + (Value_YPivot * 2.0f) + Value_XPivot;
	SkillDamageReductionValue_Pos = BasicValue_Pos + (Value_YPivot * 3.0f) + Value_XPivot;


	SubStatBasicValue_Pos = { -550.f, -279.f, -1.f };
	BasicAttackDamageReduction_Pos = SubStatBasicValue_Pos + Value_YPivot;
	DefPenetration_Pos = SubStatBasicValue_Pos + (Value_YPivot * 2.0f);
	HPRegeneration_Pos = SubStatBasicValue_Pos + (Value_YPivot * 3.0f);


	AttackRange_Pos = SubStatBasicValue_Pos + SubStat_XPivot;
	Tenacity_Pos = SubStatBasicValue_Pos + SubStat_XPivot + Value_YPivot;
	VampValue_Pos = SubStatBasicValue_Pos + SubStat_XPivot + (Value_YPivot * 2.0f);
	SPRegeneration_Pos = SubStatBasicValue_Pos + SubStat_XPivot + (Value_YPivot * 3.0f);

	//������ �ʱ�ȭ ����
	{
		MainStatusRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		MainStatusRenderer->SetImage("MainStatus_Background.png", "PointSmp");
		MainStatusRenderer->GetTransform()->SetLocalPosition(MainStatus_BackgroundPos);
		MainStatusRenderer->GetTransform()->SetLocalScaling(MainStatusRenderer->GetCurrentTexture()->GetTextureSize());

		SubStatusRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		SubStatusRenderer->SetImage("SubStatus_Background.png", "PointSmp");
		SubStatusRenderer->GetTransform()->SetLocalPosition(SubStatus_BackgroundPos);
		SubStatusRenderer->GetTransform()->SetLocalScaling(SubStatusRenderer->GetCurrentTexture()->GetTextureSize());


		//���� �������ͽ�
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
		

		//���꽺�����ͽ�
		VisionRange_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		VisionRange_Renderer->GetTransform()->SetLocalPosition(SubStatBasicValue_Pos);

		AttackRange_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		AttackRange_Renderer->GetTransform()->SetLocalPosition(AttackRange_Pos);

		BasicAttackDamageReduction_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		BasicAttackDamageReduction_Renderer->GetTransform()->SetLocalPosition(BasicAttackDamageReduction_Pos);

		Tenacity_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		Tenacity_Renderer->GetTransform()->SetLocalPosition(Tenacity_Pos);

		DefPenetration_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		DefPenetration_Renderer->GetTransform()->SetLocalPosition(DefPenetration_Pos);

		VampValue_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		VampValue_Renderer->GetTransform()->SetLocalPosition(VampValue_Pos);

		HPRegeneration_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		HPRegeneration_Renderer->GetTransform()->SetLocalPosition(HPRegeneration_Pos);

		SPRegeneration_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		SPRegeneration_Renderer->GetTransform()->SetLocalPosition(SPRegeneration_Pos);
	}

}

void UI_Status::Update(float _Time)
{
	if (0.0f >= (Time -= _Time))
	{
		//TopRenderer->SetAlpha(Time);
	}
	
	//UI �¿��� üũ
	//���߿� ������ �����Ƿ� ��������
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
			VisionRange_Renderer->Off();
			AttackRange_Renderer						 ->Off();
			BasicAttackDamageReduction_Renderer		 ->Off();
			Tenacity_Renderer							 ->Off();
			DefPenetration_Renderer					 ->Off();
			VampValue_Renderer							 ->Off();
			HPRegeneration_Renderer					 ->Off();
			SPRegeneration_Renderer					 ->Off();
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
			VisionRange_Renderer->On();
			AttackRange_Renderer->On();
			BasicAttackDamageReduction_Renderer->On();
			Tenacity_Renderer->On();
			DefPenetration_Renderer->On();
			VampValue_Renderer->On();
			HPRegeneration_Renderer->On();
			SPRegeneration_Renderer->On();
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
	//to_string�� 6�ڸ����� ǥ���ǹǷ� �Ҽ��� 2�ڸ������� ǥ��ǰ� ����
	string AttackSpeed = to_string(UIStat->AttackSpeed);
	AttackSpeed.erase(4, 7);

	//ũ��Ƽ�� Ȯ��, �� ���� �ڿ� %�� ���̴� �۾��� ��
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

	float ADReduction = UIStat->BasicAttackDamageReduction;
	string ADReduction_String = to_string(UIStat->BasicAttackDamageReduction);
	AttachPercent(ADReduction, ADReduction_String);

	float Tenacity = UIStat->Tenacity;
	string Tenacity_String = to_string(UIStat->Tenacity);
	AttachPercent(Tenacity, Tenacity_String);

	float LifeSteel = UIStat->LifeSteel;
	string LifeSteel_String = to_string(UIStat->LifeSteel);
	AttachPercent(LifeSteel, LifeSteel_String);

	//���� �������ͽ�
	AttackValue_Renderer->TextSetting("HMKMRHD", to_string((int)(UIStat->AttackPower)), 12);
	DefenseValue_Renderer->TextSetting("HMKMRHD", to_string((int)(UIStat->Defence)), 12);
	AttackSpeedValue_Renderer->TextSetting("HMKMRHD", AttackSpeed, 12);
	MoveSpeedValue_Renderer->TextSetting("HMKMRHD", to_string((int)(UIStat->MovementSpeed)), 12);
	CriticalValue_Renderer->TextSetting("HMKMRHD", CriticalChance_String, 12);
	CooldownValue_Renderer->TextSetting("HMKMRHD", CooldownChance_String, 12);
	SkillDamageIncreaseValue_Renderer->TextSetting("HMKMRHD", SDIncrease_String, 12);
	SkillDamageReductionValue_Renderer->TextSetting("HMKMRHD", SDReduction_String, 12);

	//���� �������ͽ�
	VisionRange_Renderer->TextSetting("HMKMRHD", to_string((int)(UIStat->VisionRange)), 12);
	AttackRange_Renderer->TextSetting("HMKMRHD", to_string((int)(UIStat->AttackRange)), 12);
	BasicAttackDamageReduction_Renderer->TextSetting("HMKMRHD", ADReduction_String, 12);
	Tenacity_Renderer->TextSetting("HMKMRHD", Tenacity_String, 12);
	//���� �����ü�� ������ ����
	DefPenetration_Renderer->TextSetting("HMKMRHD", "0%", 12);
	VampValue_Renderer->TextSetting("HMKMRHD", LifeSteel_String, 12);
	HPRegeneration_Renderer->TextSetting("HMKMRHD", to_string((int)(UIStat->HPRegeneration)), 12);
	SPRegeneration_Renderer->TextSetting("HMKMRHD", to_string((int)(UIStat->SPRegeneration)), 12);
}

void UI_Status::AttachPercent(float _Value, string& _Result)
{
	//�ۼ�Ʈ ����� ���ؼ� 100�� ����
	float value = _Value * 100.0f;


	if (value >= 100.0f)
	{
		//100��
		//100.000000
		_Result.erase(3,10);
	}
	else if (value >= 10.0f && value < 100.0f)
	{
		//10~99��
		//10.000000
		_Result.erase(2, 9);
	}
	else if (value < 10.0f)
	{
		//0~9��
		//0.000000
		_Result.erase(1, 8);
	}

	_Result += "%";

	return;
}