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
	//Speed_ = 0.f;
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

	StaminaRecoverUpdate(_DeltaTime);

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
	//Speed_ = 300.f;
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
	//Speed_ = 600.f;
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

	PlayerStatusFinal_.Stat_Stamina_ -= _DeltaTime;

	if (PlayerStatusFinal_.Stat_Stamina_ < 0.f)
	{
		PlayerStatusFinal_.Stat_Stamina_ = 0.f;
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
	if (PlayerStatusFinal_.Stat_Stamina_ < 5.f)
	{
		//스테미나 게이지 깜빡거리기
		MPLAYERChangeState("Idle");
	}

	AttackStateInit();
	PlayerAttackHitBoxCollision_->On();

	PlayerStatusFinal_.Stat_Stamina_ -= PlayerStatusFinal_.Stat_Attack_Stamina_;

	if (PlayerStatusFinal_.Stat_Stamina_ < 0.f)
	{
		PlayerStatusFinal_.Stat_Stamina_ = 0.f;
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
				//다음 단계 공격, 에니메이션 바꾸기
				PlayerAttackHitBoxCollision_->On();

				PlayerStatusFinal_.Stat_Stamina_ -= PlayerStatusFinal_.Stat_Attack_Stamina_;

				if (PlayerStatusFinal_.Stat_Stamina_ < 0.f)
				{
					PlayerStatusFinal_.Stat_Stamina_ = 0.f;
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

	//맞거나 이런저런 상황으로 강제로 State가 종료 되었을때
	PlayerAttackHitBoxCollision_->Off();
}



// 마우스 커서 이동용
// 명령을 컨트롤러에서 하달받고
// 하달받은 스테이트와 장입된 변수만으로 일을 처리합니다.

void Player::Stand_Start()
{
	IsMove_ = false;
	return;
}

void Player::Stand_Update(float _DeltaTime)
{
	if (true == IsMove_)
	{
		PlayerState_.ChangeState("Move");
	}

	return;
}

void Player::Stand_End()
{
	return;
}

void Player::Move_Start()
{
	arrivalPos_ = { targetPos_.x + GetTransform()->GetWorldPosition().x, 0.0f,  targetPos_.y + GetTransform()->GetWorldPosition().y };
	int a = 0;
	return;
}

void Player::Move_Update(float _DeltaTime)
{
	//CurDirUpdate(_DeltaTime);
	// 마우스 커서 우클릭한 위치로의 방향 벡터를 계산합니다.

	if (static_cast<int>(arrivalPos_.x) == static_cast<int>(GetTransform()->GetWorldPosition().x) 
		&& static_cast<int>(arrivalPos_.z) == static_cast<int>(GetTransform()->GetWorldPosition().z)) // 목표 위치에 다다르면
	{
		IsMove_ = false;
		PlayerState_.ChangeState("Stand");
	}

	if (targetPos_ != GetTransform()->GetWorldPosition())
	{
		TargetDir_ = targetPos_;
		TargetDir_.Normalize3D(); // 정규화
	}

	//MoveUpdate(_DeltaTime);
	// 우클릭 위치로 이동합니다.
	{
		GetTransform()->SetWorldMoveXZ(TargetDir_ * 200.0f * _DeltaTime);
	}

	//MoveRotateUpdate(_DeltaTime);
	// 우클릭 위치로 몸을 향합니다.
	
	
}

void Player::Move_End()
{
	TargetDir_ = float4::ZERO;
	arrivalPos_ = float4::ZERO;
	IsMove_ = false;
	return;
}