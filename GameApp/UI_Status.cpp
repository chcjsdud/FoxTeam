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

	BasicValue_Pos = { -442.f, -279.f, -1.f };
	AttackSpeedValue_Pos = BasicValue_Pos + Value_YPivot;
	CriticalValue_Pos = BasicValue_Pos + (Value_YPivot * 2.0f);
	SkillDamageIncreaseValue_Pos = BasicValue_Pos + (Value_YPivot * 3.0f);

	DefenseValue_Pos = BasicValue_Pos + Value_XPivot;
	MoveSpeedValue_Pos = BasicValue_Pos + Value_YPivot + Value_XPivot;
	CooldownValue_Pos = BasicValue_Pos + (Value_YPivot * 2.0f) + Value_XPivot;
	SkillDamageReductionValue_Pos = BasicValue_Pos + (Value_YPivot * 3.0f) + Value_XPivot;


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
		
		//to_string= ���ڸ� string����
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
	
	//UI �¿��� üũ
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

	/*
	float HPMax;
	float HP;
	float SPMax;
	float SP;
	float Defence; //����

	// �⺻ ����
	float AttackPower; //����
	float CriticalChance; //����

	// ���� ����
	float BasicAttackAmplification; // �⺻ ���� ��ȭ
	float SkillDamageAmplification; //����
	float CriticalDamageAmplification; // ũ��Ƽ�� ����� ��ȭ

	// ����
	float BasicAttackDamageReduction; // �⺻ ���� ����� ����
	float CriticalDamageReduction; // ũ��Ƽ�� ����� ����
	float SkillAttackDamageReduction; //����
	float TrapDamageReduction; // ���� ����� ����
	float Tenacity; // ������

	// ��� �� ����
	float HPRegeneration; // HP ���
	float SPRegeneration; // SP ���
	float LifeSteel; // �⺻���� ����
	float OmniVamp; // ��� ���� ����

	// �ӵ�
	float AttackSpeed; //����
	float AttackStartTime; // ���������� ������ ������ �ð�
	float AttackEndTime; // ������ ������ �ð�
	float MovementSpeed; //����
	float CooldownReduction; // ��Ÿ�� ����

	// �Ÿ�
	float AttackRange; // �⺻ ���� �Ÿ�
	float VisionRange; // �þ�
	*/

	//0.770000
	//to_string�� 6�ڸ����� ǥ���ǹǷ� �Ҽ��� 2�ڸ������� ǥ��ǰ� ����
	string AttackSpeed = to_string(UIStat->AttackSpeed);
	AttackSpeed.erase(4, 7);

	//ũ��Ƽ�� Ȯ��, �� ���� �ڿ� %�� ����
	string CriticalChance = to_string((int)(UIStat->CriticalChance));
	CriticalChance += "%";

	string CooldownChance = to_string((int)(UIStat->CooldownReduction));
	CooldownChance += "%";

	string SDIncrease = to_string((int)(UIStat->SkillDamageAmplification));
	SDIncrease += "%";

	string SDReduction = to_string((int)(UIStat->SkillAttackDamageReduction));
	SDReduction += "%";

	AttackValue_Renderer->TextSetting("HMKMRHD", to_string((int)(UIStat->AttackPower)), 12);
	DefenseValue_Renderer->TextSetting("HMKMRHD", to_string((int)(UIStat->Defence)), 12);

	AttackSpeedValue_Renderer->TextSetting("HMKMRHD", AttackSpeed, 12);
	MoveSpeedValue_Renderer->TextSetting("HMKMRHD", to_string((int)(UIStat->MovementSpeed)), 12);

	CriticalValue_Renderer->TextSetting("HMKMRHD", CriticalChance, 12);
	CooldownValue_Renderer->TextSetting("HMKMRHD", CooldownChance, 12);

	SkillDamageIncreaseValue_Renderer->TextSetting("HMKMRHD", SDIncrease, 12);
	SkillDamageReductionValue_Renderer->TextSetting("HMKMRHD", SDReduction, 12);
}