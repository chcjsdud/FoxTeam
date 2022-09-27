#pragma once

class CharacterStat
{
public:
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
	float AttackSpeed; // ���� �ӵ�
	float MovementSpeed; // �̵� �ӵ�
	float CooldownReduction; // ��Ÿ�� ����

	// �Ÿ�
	float AttackRange; // �⺻ ���� �Ÿ�
	float VisionRange; // �þ�

public:
	// �⺻ ������ ���� �������� �Ǿ�����
	CharacterStat()
		: HPMax(782.f)
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
		, MovementSpeed(346.f)
		, CooldownReduction(0.0f)
		, AttackRange(425.f)
		, VisionRange(FT::Char::DEFAULT_VISION_RANGE_DAY)
	{

	}

	~CharacterStat()
	{

	}
};