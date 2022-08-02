#include "PreCompile.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

#include "Character_ProtoType.h"

//
//State Cpp
//

// �� �ٸ������� �� �Űܾ���


//Idle
void Character_ProtoType::Idle_Start()
{
	//Speed_ = 0.f;
}
void Character_ProtoType::Idle_Update(float _DeltaTime)
{
	if (true == GameEngineInput::GetInst().Press("W")||
		true == GameEngineInput::GetInst().Press("A")||
		true == GameEngineInput::GetInst().Press("S")||
		true == GameEngineInput::GetInst().Press("D"))
	{
		//MPLAYERChangeState("Walk");
		
		State_.ChangeState("Walk");
		return;
	}

	if (GameEngineInput::GetInst().Press("Attack"))
	{
		State_.ChangeState("Attack");
		return;
	}

	StaminaRecoverUpdate(_DeltaTime);

	CurDirUpdate(_DeltaTime);

	MoveRotateUpdate(_DeltaTime);
}
void Character_ProtoType::Idle_End()
{
}

//Walk
void Character_ProtoType::Walk_Start()
{
	IsMove_ = true;
	//Speed_ = 300.f;
}
void Character_ProtoType::Walk_Update(float _DeltaTime)
{
	// ũ�Ⱑ 2¥�� ť�� �����, 2��°�� ���� Key�� �ƴϸ� return ��Ŵ, �׸��� �ȴ������� �ٽ� ��ȯ���Ѽ� ���� ������ 2���� ���� ���ֱ�
	// 
	if (GameEngineInput::GetInst().Press("Attack"))
	{
		State_.ChangeState("Attack");
		return;
	}

	if (false == GameEngineInput::GetInst().Press("W") &&
		false == GameEngineInput::GetInst().Press("A") &&
		false == GameEngineInput::GetInst().Press("S") &&
		false == GameEngineInput::GetInst().Press("D"))
	{
		State_.ChangeState("Idle");

		return;
	}

	if (true == GameEngineInput::GetInst().Press("Space"))
	{
		State_.ChangeState("Run");

		return;
	}

	CurDirUpdate(_DeltaTime);
	MoveUpdate(_DeltaTime);
	MoveRotateUpdate(_DeltaTime);
}
void Character_ProtoType::Walk_End()
{
	IsMove_ = false;
}

//Run
void Character_ProtoType::Run_Start()
{
	IsMove_ = true;
	//Speed_ = 600.f;
}
void Character_ProtoType::Run_Update(float _DeltaTime)
{
	if (GameEngineInput::GetInst().Press("Attack"))
	{
		State_.ChangeState("Attack");
		return;
	}

	if (false == GameEngineInput::GetInst().Press("Space"))
	{
		if (true == GameEngineInput::GetInst().Press("W") ||
			true == GameEngineInput::GetInst().Press("A") ||
			true == GameEngineInput::GetInst().Press("S") ||
			true == GameEngineInput::GetInst().Press("D"))
		{
			State_.ChangeState("Walk");

			return;
		}
	}

	if (false == GameEngineInput::GetInst().Press("W") &&
		false == GameEngineInput::GetInst().Press("A") &&
		false == GameEngineInput::GetInst().Press("S") &&
		false == GameEngineInput::GetInst().Press("D"))
	{
		State_.ChangeState("Idle");

		return;
	}

	Status_Final_.Stat_Stamina_ -= _DeltaTime;

	if (Status_Final_.Stat_Stamina_ < 0.f)
	{
		Status_Final_.Stat_Stamina_ = 0.f;
	}

	CurDirUpdate(_DeltaTime);
	MoveUpdate(_DeltaTime);
	MoveRotateUpdate(_DeltaTime);
}
void Character_ProtoType::Run_End()
{
	IsMove_ = false;
}

//Attack
void Character_ProtoType::Attack_Start()
{
	if (Status_Final_.Stat_Stamina_ < 5.f)
	{
		//���׹̳� ������ �����Ÿ���
		State_.ChangeState("Idle");
		return;
	}

	// �� �ð� �ȿ� �߰� ���� �ϸ� ���ӱⰡ ����, ���ݽ� ����
	AttackTime_ = 0.5f;

	//���ӱ� �ܰ�, 3�ܰ� ������ �ٽ� 0����
	AttackLevel_ = 0;

	//0.5�� �������� �����Ѵ�. ���ݽ� ����
	AttackTurm_ = 0.25f;

	//0.1�� ���ȸ� Ÿ�� ������ �ִ�. ���ݽ� ����
	AttackHitTime_ = 0.1f;

	AttackHitBoxCollision_->On();

	//Status_Final_.Stat_Stamina_ -= Status_Final_.Stat_Attack_Stamina_;

	//if (Status_Final_.Stat_Stamina_ < 0.f)
	//{
	//	Status_Final_.Stat_Stamina_ = 0.f;
	//}
}
void Character_ProtoType::Attack_Update(float _DeltaTime)
{
	KeyDirUpdate(_DeltaTime);

	AttackTurm_ -= _DeltaTime;
	AttackTime_ -= _DeltaTime;
	AttackHitTime_ -= _DeltaTime;

	if (AttackHitTime_ <= 0)
	{
		MoveRotateUpdate(_DeltaTime);
		AttackHitBoxCollision_->Off();
	}

	if (AttackTime_ > 0.f)
	{
		if (AttackTurm_ <= 0.f)
		{
			if (GameEngineInput::GetInst().Press("Attack"))
			{
				//���� �ܰ� ����, ���ϸ��̼� �ٲٱ�
				AttackHitBoxCollision_->On();

				//Status_Final_.Stat_Stamina_ -= Status_Final_.Stat_Attack_Stamina_;

				//if (Status_Final_.Stat_Stamina_ < 0.f)
				//{
				//	Status_Final_.Stat_Stamina_ = 0.f;
				//}

				AttackTime_ = 1.f;
				AttackTurm_ = 0.5f;
				AttackHitTime_ = 0.1f;

				++AttackLevel_;
				if (AttackLevel_ == 2)
				{
					AttackLevel_ = 0;
				}
			}
		}
	}

	else
	{
		State_.ChangeState("Idle");
		return;
	}
}
void Character_ProtoType::Attack_End()
{
	AttackTurm_ = 0.f;
	AttackTime_ = 0.f;
	AttackLevel_ = 0;
	AttackHitTime_ = 0.f;

	//�°ų� �̷����� ��Ȳ���� ������ State�� ���� �Ǿ�����
	AttackHitBoxCollision_->Off();
}