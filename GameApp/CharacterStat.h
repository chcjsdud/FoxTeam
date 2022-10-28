#pragma once

class CharacterStat
{
public:
	// 기본 스탯
	float HPMax;
	float HP;
	float SPMax;
	float SP;
	float Defence;

	// 기본 공격
	float AttackPower;
	float CriticalChance;

	// 공격 증폭
	float BasicAttackAmplification; // 기본 공격 강화
	float SkillDamageAmplification; // 스킬 대미지 강화
	float CriticalDamageAmplification; // 크리티컬 대미지 강화

	// 저항
	float BasicAttackDamageReduction; // 기본 공격 대미지 감소
	float CriticalDamageReduction; // 크리티컬 대미지 감소
	float SkillAttackDamageReduction; // 스킬 대미지 감소
	float TrapDamageReduction; // 함정 대미지 감소
	float Tenacity; // 강인함

	// 재생 및 흡혈
	float HPRegeneration; // HP 재생
	float SPRegeneration; // SP 재생
	float LifeSteel; // 기본공격 흡혈
	float OmniVamp; // 모든 피해 흡혈

	// 속도
	float AttackSpeed; // 초당 공격 속도
	float AttackStartTime; // 실질적으로 공격이 나가는 시간
	float AttackEndTime; // 공격이 끝나는 시간
	float MovementSpeed; // 이동 속도
	float MovementRatio; // (이동 속도 비율...1.0f 이 정상)
	float CooldownReduction; // 쿨타임 감소

	// 거리
	float AttackRange; // 기본 공격 거리
	float VisionRange; // 시야

	// 쿨타임 기준
	float Cooltime_q;
	float Cooltime_w;
	float Cooltime_e;
	float Cooltime_r;
	float Cooltime_d;

	// 스킬 레벨
	int Level_q;
	int Level_w;
	int Level_e;
	int Level_r;
	int Level_d;

public:
	// 기본 세팅은 리오 기준으로 되어있음
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
		, AttackStartTime(0.15f)
		, AttackEndTime(0.4f)
		, MovementSpeed(346.f)
		, CooldownReduction(0.0f)
		, AttackRange(425.f)
		, VisionRange(FT::Char::DEFAULT_VISION_RANGE_DAY)
		, MovementRatio(1.0f)
		, Cooltime_q(1.0f)
		, Cooltime_w(12.0f)
		, Cooltime_e(8.0f)
		, Cooltime_r(80.0f)
		, Cooltime_d(30.0f)
		, Level_q(1)
		, Level_w(1)
		, Level_e(1)
		, Level_r(1)
		, Level_d(1)
	{

	}

	~CharacterStat()
	{

	}
};