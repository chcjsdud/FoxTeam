#include "PreCompile.h"
#include "Monsters.h"

void Monsters::StartAppearState()
{
	// 현재 상태 지정
	CurStateType_ = MonsterStateType::APPEAR;

	// 
}

void Monsters::UpdateAppearState(float _DeltaTime)
{
	// 첫등장 시간 체크

}

void Monsters::EndAppearState()
{

}

void Monsters::StartRegenState()
{
	// 현재 상태 지정
	CurStateType_ = MonsterStateType::REGEN;

	// 초기스폰위치로 셋팅 후 등장시작

}

void Monsters::UpdateRegenState(float _DeltaTime)
{

}

void Monsters::EndRegenState()
{

}

void Monsters::StartIdleState()
{
	// 현재 상태 지정
	CurStateType_ = MonsterStateType::IDLE;

	// 타겟지정여부 판단


}

void Monsters::UpdateIdleState(float _DeltaTime)
{
	// 모든 상태에서 전환되는 대기상태



}

void Monsters::EndIdleState()
{

}

void Monsters::StartRunState()
{
	// 현재 상태 지정
	CurStateType_ = MonsterStateType::RUN;

	// 
}

void Monsters::UpdateRunState(float _DeltaTime)
{

}

void Monsters::EndRunState()
{

}

void Monsters::StartHomingInstinctState()
{
	// 현재 상태 지정
	CurStateType_ = MonsterStateType::HOMINGINSTINCT;

	// 
}

void Monsters::UpdateHomingInstinctState(float _DeltaTime)
{

}

void Monsters::EndHomingInstinctState()
{

}

void Monsters::StartChaseState()
{
	// 현재 상태 지정
	CurStateType_ = MonsterStateType::CHASE;

	// 
}

void Monsters::UpdateChaseState(float _DeltaTime)
{

}

void Monsters::EndChaseState()
{

}
