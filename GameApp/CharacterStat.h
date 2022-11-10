#pragma once

class CharacterStat
{
public:
	int level;
	// �⺻ ����
	float HPMax;
	float HP;
	float SPMax;
	float SP;
	float Defence;

	// �⺻ ����
	float AttackPower;
	float CriticalChance;

	// ���� ����
	float BasicAttackAmplification; // �⺻ ���� ��ȭ
	float SkillDamageAmplification; // ��ų ����� ��ȭ
	float CriticalDamageAmplification; // ũ��Ƽ�� ����� ��ȭ

	// ����
	float BasicAttackDamageReduction; // �⺻ ���� ����� ����
	float CriticalDamageReduction; // ũ��Ƽ�� ����� ����
	float SkillAttackDamageReduction; // ��ų ����� ����
	float TrapDamageReduction; // ���� ����� ����
	float Tenacity; // ������

	// ��� �� ����
	float HPRegeneration; // HP ���
	float SPRegeneration; // SP ���
	float LifeSteel; // �⺻���� ����
	float OmniVamp; // ��� ���� ����

	// �ӵ�
	float AttackSpeedRatio = 1.0; // ���ݼӵ�����
	float AttackSpeed; // �ʴ� ���� �ӵ�
	float AttackStartTime; // ���������� ������ ������ �ð�
	float AttackEndTime; // ������ ������ �ð�
	float MovementSpeed; // �̵� �ӵ�
	float MovementRatio; // (�̵� �ӵ� ����...1.0f �� ����)
	float CooldownReduction; // ��Ÿ�� ����

	// �Ÿ�
	float AttackRange; // �⺻ ���� �Ÿ�
	float VisionRange; // �þ�

	// ��ų ����
	int Level_q;
	int Level_w;
	int Level_e;
	int Level_r;
	int Level_d;
	int Level_passive;

	// 1103 ������ : CC�� ���� bool ��
	bool isUnstoppable_;
	
	//1109 �̰�ȣ : ��Ű�� �ƾ߰��� �Ҹ��ϴ� �нú�� int
	int passive_Count;

public:
	// �⺻ ������ ���� �������� �Ǿ�����
	CharacterStat()
		: level(1)
		, HPMax(782.f)
		, HP(782.f)
		, SPMax(420.f)
		, SP(420.f)
		, Defence(32.f)
		, AttackPower(29.f)
		, CriticalChance(0.0f)
		, BasicAttackAmplification(0.0f)
		, SkillDamageAmplification(0.0f)
		, CriticalDamageAmplification(FT::Char::DEFAULT_CRITICAL_DAMAGE_AMPLIFICATION)
		, BasicAttackDamageReduction(0.0f)
		, SkillAttackDamageReduction(0.0f)
		, CriticalDamageReduction(0.0f)
		, TrapDamageReduction(0.0f)
		, Tenacity(0.0f)
		, HPRegeneration(1.3f)
		, SPRegeneration(1.6f)
		, LifeSteel(0.0f)
		, OmniVamp(0.0f)
		, AttackSpeed(0.77f)
		, AttackStartTime(0.15f)
		, AttackEndTime(0.4f)
		, MovementSpeed(346.f)
		, CooldownReduction(0.0f)
		, AttackRange(425.f)
		, VisionRange(FT::Char::DEFAULT_VISION_RANGE_DAY)
		, MovementRatio(1.0f)
		, Level_q(1)
		, Level_w(1)
		, Level_e(1)
		, Level_r(1)
		, Level_d(1)
		, Level_passive(1)
		, isUnstoppable_(false)
		, passive_Count(0)
	{

	}

	~CharacterStat()
	{

	}

	CharacterStat operator+(const CharacterStat _cs) const
	{
		CharacterStat result = (*this);

		result.level += _cs.level;
		result.HPMax += _cs.HPMax;
		result.HP += _cs.HP;
		result.SPMax += _cs.SPMax;
		result.SP += _cs.SP;
		result.Defence += _cs.Defence;
		result.AttackPower += _cs.AttackPower;
		result.CriticalChance += _cs.CriticalChance;
		result.BasicAttackAmplification += _cs.BasicAttackAmplification;
		result.SkillDamageAmplification += _cs.SkillDamageAmplification;
		result.CriticalDamageAmplification += _cs.CriticalDamageAmplification;
		result.BasicAttackDamageReduction += _cs.BasicAttackDamageReduction;
		result.SkillAttackDamageReduction += _cs.SkillAttackDamageReduction;
		result.CriticalDamageReduction += _cs.CriticalDamageReduction;
		result.TrapDamageReduction += _cs.TrapDamageReduction;
		result.Tenacity += _cs.Tenacity;
		result.HPRegeneration += _cs.HPRegeneration;
		result.SPRegeneration += _cs.SPRegeneration;
		result.LifeSteel += _cs.LifeSteel;
		result.OmniVamp += _cs.OmniVamp;
		result.AttackSpeed += _cs.AttackSpeed;
		result.AttackStartTime += _cs.AttackStartTime;
		result.AttackEndTime += _cs.AttackEndTime;
		result.MovementSpeed += _cs.MovementSpeed;
		result.CooldownReduction += _cs.CooldownReduction;
		result.AttackRange += _cs.AttackRange;
		result.VisionRange += _cs.VisionRange;
		result.MovementRatio += _cs.MovementRatio;
		result.Level_q += _cs.Level_q;
		result.Level_w += _cs.Level_w;
		result.Level_e += _cs.Level_e;
		result.Level_r += _cs.Level_r;
		result.Level_d += _cs.Level_d;
		result.Level_passive += _cs.Level_passive;
		result.passive_Count += _cs.passive_Count;

		return result;
	}

	void Clear()
	{
		level = 0;
		HPMax = 0;
		HP = 0;
		SPMax = 0;
		SP = 0;
		Defence = 0;
		AttackPower = 0;
		CriticalChance = 0;
		BasicAttackAmplification = 0;
		SkillDamageAmplification = 0;
		CriticalDamageAmplification = 0;
		BasicAttackDamageReduction = 0;
		SkillAttackDamageReduction = 0;
		CriticalDamageReduction = 0;
		TrapDamageReduction = 0;
		Tenacity = 0;
		HPRegeneration = 0;
		SPRegeneration = 0;
		LifeSteel = 0;
		OmniVamp = 0;
		AttackSpeed = 0;
		AttackStartTime = 0;
		AttackEndTime = 0;
		MovementSpeed = 0;
		CooldownReduction = 0;
		AttackRange = 0;
		VisionRange = 0;
		MovementRatio = 0;
		Level_q = 0;
		Level_w = 0;
		Level_e = 0;
		Level_r = 0;
		Level_d = 0;
		Level_passive = 0;
		isUnstoppable_ = false;
		passive_Count = 0;
	}
};