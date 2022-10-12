#pragma once
#include <vector>
#include "Enums.h"

// 분류 : 정보
// 용도 : 
// 설명 : 몬스터의 상태정보
struct MonsterStateInfo
{
public: // 기본스텟
	int Level_;								// 레벨(늦게 생성된 몬스터(야생동물)일수록 레벨이 높게 설정) - 생성과 동시에 지정(지정없이 생성된 몬스터의 경우 기본 1레벨고정)
	float HPMax_;							// 최대체력
	float HP_;								// 현재체력
	float HPRegenPercent_;					// 체력회복량(% 수치)
	float SPMax_;							// 최대마력
	float SP_;								// 현재마력
	float SPRegenPercent_;					// 마력회복량(% 수치)
	float Defence_;							// 방어력

public: // 상세스텟
	float4 NestPosition_;					// 둥지위치(= 스폰위치)
	float HomingInstinctValueMax_;			// 최대 귀소본능 수치
	float HomingInstinctValue_;				// 귀소본능 수치(0.0f보다 작거나같아지면 몬스터는 스폰위치로 이동하면 체력을 회복 -> 타겟이 DetectRange_에 벗어난 시간에 따라 감소)
	float AttackRange_;						// 공격 시야거리
	float DetectRange_;						// 감지 시야거리
	float AttackSpeed_;						// 공격 속도
	float MoveSpeed_;						// 이동 속도

public: // 스킬스텟
	float SkillCoolDown_;					// 스킬재사용시간

public: // 젠스텟
	float RegenTimeMax_;					// 리젠타임(고정)
	float RegenTime_;						// 리젠타임(갱신) -> 0.0f이하일시 RegenTime_으로 초기화

public: // 기본드랍아이템정보


public: // 확률드랍아이템정보

public:
	MonsterStateInfo()
		: Level_(1)
		, HPMax_(0.0f)
		, HP_(0.0f)
		, HPRegenPercent_(0.0f)
		, SPMax_(0.0f)
		, SP_(0.0f)
		, SPRegenPercent_(0.0f)
		, Defence_(0.0f)
		, NestPosition_(float4::ZERO)
		, HomingInstinctValueMax_(0.0f)
		, HomingInstinctValue_(0.0f)
		, AttackRange_(0.0f)
		, DetectRange_(0.0f)
		, AttackSpeed_(0.0f)
		, MoveSpeed_(0.0f)
		, SkillCoolDown_(0.0f)
		, RegenTimeMax_(0.0f)
		, RegenTime_(0.0f)
	{
	}

	~MonsterStateInfo()
	{
	}
};
