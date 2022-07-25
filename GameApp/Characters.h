#pragma once
#include <GameEngine/GameEngineActor.h>

#include "Enums.h"

struct SkillInfo
{
	// skilltype(패시브, 공격, ...)
	// 소모자원(sp)
	// 쿨타임
	// 사거리
	// 효과
};

// 분류 : 
// 용도 : 캐릭터 최상위 클래스
// 설명 : 모든 캐릭터의 커맨드 패턴 및 기본정보를 관리
class Characters : public GameEngineActor
{
private: // 현재 메인플레이어 지정
	static Characters* MainPlayer;

public:
	static Characters* GetMainPlayer();
	static void SetMainPlayer(Characters* _Player);

private: // 생성정보
	CharacterType CharacterType_;		// (0: AI, 1: MAIN) : default(AI)

private: // 기본정보
	JobType JobType_;					// 직업타입
	float OffensePower_;				// 공격력
	int Health_;						// 체력
	float HealthRecovery_;				// 체력재생
	int Stamina_;						// 스태미너
	float StaminaRecovery_;				// 스태미너 재생
	float Defense_;						// 방어력
	float AttackSpeed_;					// 공격속도
	float CriticalHit_;					// 치명타
	float MoveSpeed_;					// 이동속도
	float Eyesight_;					// 시야

private: // 레벨업당 상승정보
	float Lv_OffensePower_;				// 공격력
	int Lv_Health_;						// 체력
	float Lv_HealthRecovery_;			// 체력재생
	int Lv_Stamina_;					// 스태미너
	float Lv_StaminaRecovery_;			// 스태미너 재생
	float Lv_Defense_;					// 방어력
	float Lv_AttackSpeed_;				// 공격속도
	float Lv_CriticalHit_;				// 치명타
	float Lv_MoveSpeed_;				// 이동속도
	float Lv_Eyesight_;					// 시야

private: // 스킬정보
	SkillInfo SkillInfo_[6];			// 스킬(패시브, Q, W, E, R, D)
										// 패시브, Q~R 고유 스킬
										// D 스킬은 무기의 스킬

private: // 커맨드 패턴
// 1. 메인캐릭터 시야 밖에서의 패턴(이동-파밍 패턴)
// 2. 메인캐릭터 시야 범위내에서의 패턴(이동-공격 or 공격-회피)


public:
	Characters();
	~Characters();

protected:		// delete constructer
	Characters(const Characters& _other) = delete;
	Characters(Characters&& _other) noexcept = delete;

private:		//delete operator
	Characters& operator=(const Characters& _other) = delete;
	Characters& operator=(const Characters&& _other) = delete;

public:
	inline void SetCharacterType(CharacterType _Type)
	{
		// 잘못된 타입 수신시 실패
		if (_Type == CharacterType::NONE || _Type == CharacterType::MAX)
		{
			return;
		}

		CharacterType_ = _Type;
	}

protected: // 각 캐릭터 고유 정보 및 스킬셋팅
	virtual void Initialize() = 0;

protected: // 각 캐릭터 커맨드 부여


};

