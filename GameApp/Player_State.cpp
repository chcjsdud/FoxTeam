#include "PreCompile.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

#include "Player.h"

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
		PlayerState_.ChangeState("Walk");

		return;
	}


}
void Player::Idle_End()
{
}

//Walk
void Player::Walk_Start()
{
	Speed_ = 300.f;
}
void Player::Walk_Update(float _DeltaTime)
{
	// ũ�Ⱑ 2¥�� ť�� �����, 2��°�� ���� Key�� �ƴϸ� return ��Ŵ, �׸��� �ȴ������� �ٽ� ��ȯ���Ѽ� ���� ������ 2���� ���� ���ֱ�
	// 


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

	MoveUpdate(_DeltaTime);
	MoveRotateUpdate(_DeltaTime);
}
void Player::Walk_End()
{
}

//Run
void Player::Run_Start()
{
	Speed_ = 600.f;
}
void Player::Run_Update(float _DeltaTime)
{
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

	MoveUpdate(_DeltaTime);
	MoveRotateUpdate(_DeltaTime);
}
void Player::Run_End()
{
}

//Attack
void Player::Attack_Start()
{
	if (AttackTurm_ == 0)
	{
		AttackTime_ = 1.f;
		AttackLevel_ = 0;
		AttackTurm_ = 0.5f;
		AttackHitTime_ = 0.1f;

		PlayerAttackHitBoxCollision_->On();
	}
}
void Player::Attack_Update(float _DeltaTime)
{
	AttackTurm_ -= _DeltaTime;
	AttackTime_ -= _DeltaTime;
	AttackHitTime_ -= _DeltaTime;

	if (AttackHitTime_ < 0)
	{
		PlayerAttackHitBoxCollision_->Off();
	}


	if (AttackTurm_ < 0.f)
	{
		if (AttackTime_ > 0.f)
		{
			if (AttackLevel_ < 3)
			{
				if (GameEngineInput::GetInst().Press("Attack"))
				{
					//���� �ܰ� ����, ���ϸ��̼� �ٲٱ�
					PlayerAttackHitBoxCollision_->On();

					AttackTime_ = 1.f;

					AttackTurm_ = 0.5f;
				}
			}
			else
			{
				PlayerState_.ChangeState("Idle");
			}
		}
		else
		{
			PlayerAttackHitBoxCollision_->Off();
		}
	}

	if (AttackTime_ < 0)
	{
		AttackTurm_ = 0.f;
		PlayerAttackHitBoxCollision_->Off();

		PlayerState_.ChangeState("Idle");
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
