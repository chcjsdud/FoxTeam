#include "PreCompile.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

#include "Player.h"

#define MPLAYERChangeState(Name) 	PlayerState_.ChangeState(Name); return

//
//State Cpp
//

//Idle
void Player::Idle_Start()
{
	Speed_ = 0.f;
}
void Player::Idle_Update(float _DeltaTime)
{
	if (true == GameEngineInput::GetInst().Press("W")||
		true == GameEngineInput::GetInst().Press("A")||
		true == GameEngineInput::GetInst().Press("S")||
		true == GameEngineInput::GetInst().Press("D"))
	{
		//MPLAYERChangeState("Walk");
		
		PlayerState_.ChangeState("Walk");
		return;
	}

	if (GameEngineInput::GetInst().Press("Attack"))
	{
		PlayerState_.ChangeState("Attack");
		return;
	}

	CurDirUpdate(_DeltaTime);

	MoveRotateUpdate(_DeltaTime);
}
void Player::Idle_End()
{
}

//Walk
void Player::Walk_Start()
{
	IsMove_ = true;
	Speed_ = 300.f;
}
void Player::Walk_Update(float _DeltaTime)
{
	// ũ�Ⱑ 2¥�� ť�� �����, 2��°�� ���� Key�� �ƴϸ� return ��Ŵ, �׸��� �ȴ������� �ٽ� ��ȯ���Ѽ� ���� ������ 2���� ���� ���ֱ�
	// 
	if (GameEngineInput::GetInst().Press("Attack"))
	{
		PlayerState_.ChangeState("Attack");
		return;
	}

	if (false == GameEngineInput::GetInst().Press("W") &&
		false == GameEngineInput::GetInst().Press("A") &&
		false == GameEngineInput::GetInst().Press("S") &&
		false == GameEngineInput::GetInst().Press("D"))
	{
		PlayerState_.ChangeState("Idle");

		return;
	}

	if (true == GameEngineInput::GetInst().Press("Space"))
	{
		PlayerState_.ChangeState("Run");

		return;
	}

	CurDirUpdate(_DeltaTime);
	MoveUpdate(_DeltaTime);
	MoveRotateUpdate(_DeltaTime);
}
void Player::Walk_End()
{
	IsMove_ = false;
}

//Run
void Player::Run_Start()
{
	IsMove_ = true;
	Speed_ = 600.f;
}
void Player::Run_Update(float _DeltaTime)
{
	if (GameEngineInput::GetInst().Press("Attack"))
	{
		PlayerState_.ChangeState("Attack");
		return;
	}

	if (false == GameEngineInput::GetInst().Press("Space"))
	{
		if (true == GameEngineInput::GetInst().Press("W") ||
			true == GameEngineInput::GetInst().Press("A") ||
			true == GameEngineInput::GetInst().Press("S") ||
			true == GameEngineInput::GetInst().Press("D"))
		{
			PlayerState_.ChangeState("Walk");

			return;
		}
	}

	if (false == GameEngineInput::GetInst().Press("W") &&
		false == GameEngineInput::GetInst().Press("A") &&
		false == GameEngineInput::GetInst().Press("S") &&
		false == GameEngineInput::GetInst().Press("D"))
	{
		PlayerState_.ChangeState("Idle");

		return;
	}

	CurDirUpdate(_DeltaTime);
	MoveUpdate(_DeltaTime);
	MoveRotateUpdate(_DeltaTime);
}
void Player::Run_End()
{
	IsMove_ = false;
}

//Attack
void Player::Attack_Start()
{
	if (Stamina_ < 5.f)
	{
		//���׹̳� ������ �����Ÿ���
		MPLAYERChangeState("Idle");
	}

	AttackStateInit();
	PlayerAttackHitBoxCollision_->On();

	Stamina_ -= Attack_Stamina_;

	if (Stamina_ < 0.f)
	{
		Stamina_ = 0.f;
	}
}
void Player::Attack_Update(float _DeltaTime)
{
	KeyDirUpdate(_DeltaTime);

	AttackTurm_ -= _DeltaTime;
	AttackTime_ -= _DeltaTime;
	AttackHitTime_ -= _DeltaTime;

	if (AttackHitTime_ <= 0)
	{
		MoveRotateUpdate(_DeltaTime);
		PlayerAttackHitBoxCollision_->Off();
	}

	if (AttackTime_ > 0.f)
	{
		if (AttackTurm_ <= 0.f)
		{
			if (GameEngineInput::GetInst().Press("Attack"))
			{
				//���� �ܰ� ����, ���ϸ��̼� �ٲٱ�
				PlayerAttackHitBoxCollision_->On();

				Stamina_ -= Attack_Stamina_;

				if (Stamina_ < 0.f)
				{
					Stamina_ = 0.f;
				}

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
		PlayerState_.ChangeState("Idle");
		return;
	}
}
void Player::Attack_End()
{
	AttackTurm_ = 0.f;
	AttackTime_ = 0.f;
	AttackLevel_ = 0;
	AttackHitTime_ = 0.f;

	//�°ų� �̷����� ��Ȳ���� ������ State�� ���� �Ǿ�����
	PlayerAttackHitBoxCollision_->Off();
}

