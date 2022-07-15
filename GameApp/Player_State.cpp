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
	// 크기가 2짜리 큐를 만들고, 2번째는 인접 Key가 아니면 return 시킴, 그리고 안눌렸으면 다시 반환시켜서 먼저 누른거 2개만 남게 해주기
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
	KeyDirUpdate(_DeltaTime);
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
	KeyDirUpdate(_DeltaTime);
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
	if (AttackTurm_ == 0)
	{
		// 이 시간 안에 추가 공격 하면 연속기가 나감, 공격시 갱신
		AttackTime_ = 1.f;

		//연속기 단계, 3단계 넘으면 다시 0으로
		AttackLevel_ = 0;

		//0.5초 간격으로 공격한다. 공격시 갱신
		AttackTurm_ = 0.5f;

		//0.1초 동안만 타격 판정이 있다. 공격시 갱신
		AttackHitTime_ = 0.1f;

		PlayerAttackHitBoxCollision_->On();
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
		//타격 중에는 회전하지 않음
		MoveRotateUpdate(_DeltaTime);
		PlayerAttackHitBoxCollision_->Off();
	}

	if (AttackTime_ > 0.f)
	{
		if (AttackTurm_ <= 0.f)
		{
			if (GameEngineInput::GetInst().Press("Attack"))
			{
				//다음 단계 공격, 에니메이션 바꾸기
				PlayerAttackHitBoxCollision_->On();

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

	//맞거나 이런저런 상황으로 강제로 State가 종료 되었을때
	PlayerAttackHitBoxCollision_->Off();
}
