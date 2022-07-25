#include "PreCompile.h"
#include "Characters.h"

Characters* Characters::MainPlayer = nullptr;

Characters* Characters::GetMainPlayer()
{
	return MainPlayer;
}

void Characters::SetMainPlayer(Characters* _Player)
{
	if (nullptr != _Player)
	{
		// 메인플레이어 변경
		MainPlayer = _Player;

		// 메인플레이어로 변경되었으므로 타입변경
		MainPlayer->CharacterType_ = CharacterType::MAIN;
	}
}

Characters::Characters() 
	: CharacterType_(CharacterType::AI)
	, JobType_(JobType::NONE)
	, OffensePower_(0.f)
	, Health_(0)
	, HealthRecovery_(0.f)
	, Stamina_(0)
	, StaminaRecovery_(0.f)
	, Defense_(0.f)
	, AttackSpeed_(0.f)
	, CriticalHit_(0.f)
	, MoveSpeed_(0.f)
	, Eyesight_(0.f)
	, Lv_OffensePower_(0.f)
	, Lv_Health_(0)
	, Lv_HealthRecovery_(0.f)
	, Lv_Stamina_(0)
	, Lv_StaminaRecovery_(0.f)
	, Lv_Defense_(0.f)
	, Lv_AttackSpeed_(0.f)
	, Lv_CriticalHit_(0.f)
	, Lv_MoveSpeed_(0.f)
	, Lv_Eyesight_(0.f)
	, SkillInfo_{}
{
}

Characters::~Characters()
{
}
