#include "PreCompile.h"
#include "Monsters.h"

void Monsters::StartAppearState()
{
	// ���� ���� ����
	CurStateType_ = MonsterStateType::APPEAR;

	// 
}

void Monsters::UpdateAppearState(float _DeltaTime)
{
	// ù���� �ð� üũ

}

void Monsters::EndAppearState()
{

}

void Monsters::StartRegenState()
{
	// ���� ���� ����
	CurStateType_ = MonsterStateType::REGEN;

	// �ʱ⽺����ġ�� ���� �� �������

}

void Monsters::UpdateRegenState(float _DeltaTime)
{

}

void Monsters::EndRegenState()
{

}

void Monsters::StartIdleState()
{
	// ���� ���� ����
	CurStateType_ = MonsterStateType::IDLE;

	// Ÿ���������� �Ǵ�


}

void Monsters::UpdateIdleState(float _DeltaTime)
{
	// ��� ���¿��� ��ȯ�Ǵ� ������



}

void Monsters::EndIdleState()
{

}

void Monsters::StartRunState()
{
	// ���� ���� ����
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
	// ���� ���� ����
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
	// ���� ���� ����
	CurStateType_ = MonsterStateType::CHASE;

	// 
}

void Monsters::UpdateChaseState(float _DeltaTime)
{

}

void Monsters::EndChaseState()
{

}
