#include "PreCompile.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

#include "YSJ_Char.h"

//
//State Cpp
//

// 곧 다른곳으로 다 옮겨야함


//Idle
void YSJ_Char::Idle_Start()
{
	//Speed_ = 0.f;
}
void YSJ_Char::Idle_Update(float _DeltaTime)
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
void YSJ_Char::Idle_End()
{
}

//Walk
void YSJ_Char::Walk_Start()
{
	IsMove_ = true;
	//Speed_ = 300.f;
}
void YSJ_Char::Walk_Update(float _DeltaTime)
{
	// 크기가 2짜리 큐를 만들고, 2번째는 인접 Key가 아니면 return 시킴, 그리고 안눌렸으면 다시 반환시켜서 먼저 누른거 2개만 남게 해주기
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
void YSJ_Char::Walk_End()
{
	IsMove_ = false;
}

//Run
void YSJ_Char::Run_Start()
{
	IsMove_ = true;
	//Speed_ = 600.f;
}
void YSJ_Char::Run_Update(float _DeltaTime)
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
void YSJ_Char::Run_End()
{
	IsMove_ = false;
}

//Attack
void YSJ_Char::Attack_Start()
{
	if (Status_Final_.Stat_Stamina_ < 5.f)
	{
		//스테미나 게이지 깜빡거리기
		State_.ChangeState("Idle");
		return;
	}

	// 이 시간 안에 추가 공격 하면 연속기가 나감, 공격시 갱신
	AttackTime_ = 0.5f;

	//연속기 단계, 3단계 넘으면 다시 0으로
	AttackLevel_ = 0;

	//0.5초 간격으로 공격한다. 공격시 갱신
	AttackTurm_ = 0.25f;

	//0.1초 동안만 타격 판정이 있다. 공격시 갱신
	AttackHitTime_ = 0.1f;

	AttackHitBoxCollision_->On();

	//Status_Final_.Stat_Stamina_ -= Status_Final_.Stat_Attack_Stamina_;

	//if (Status_Final_.Stat_Stamina_ < 0.f)
	//{
	//	Status_Final_.Stat_Stamina_ = 0.f;
	//}
}
void YSJ_Char::Attack_Update(float _DeltaTime)
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
				//다음 단계 공격, 에니메이션 바꾸기
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
void YSJ_Char::Attack_End()
{
	AttackTurm_ = 0.f;
	AttackTime_ = 0.f;
	AttackLevel_ = 0;
	AttackHitTime_ = 0.f;

	//맞거나 이런저런 상황으로 강제로 State가 종료 되었을때
	AttackHitBoxCollision_->Off();
}



// 마우스 커서 이동용
// 명령을 컨트롤러에서 하달받고
// 하달받은 스테이트와 장입된 변수만으로 일을 처리합니다.

//void YSJ_Char::Stand_Start()
//{
//	IsMove_ = false;
//	return;
//}
//
//void YSJ_Char::Stand_Update(float _DeltaTime)
//{
//	if (true == IsMove_)
//	{
//		State_.ChangeState("Move");
//	}
//
//	return;
//}
//
//void YSJ_Char::Stand_End()
//{
//	return;
//}
//
//void YSJ_Char::Move_Start()
//{
//
//	int a = 0;
//	return;
//}
//
//void YSJ_Char::Move_Update(float _DeltaTime)
//{
//	//CurDirUpdate(_DeltaTime);
//	// 마우스 커서 우클릭한 위치로의 방향 벡터를 계산합니다.
//
//	if (static_cast<int>(arrivalPos_.x) == static_cast<int>(GetTransform()->GetWorldPosition().x) 
//		&& static_cast<int>(arrivalPos_.z) == static_cast<int>(GetTransform()->GetWorldPosition().z)) // 목표 위치에 다다르면
//	{
//		IsMove_ = false;
//		State_.ChangeState("Stand");
//	}
//
//	if (targetPos_ != GetTransform()->GetWorldPosition())
//	{
//		TargetDir_ = targetPos_;
//		TargetDir_.Normalize3D(); // 정규화
//	}
//
//	//MoveUpdate(_DeltaTime);
//	// 우클릭 위치로 이동합니다.
//	{
//		GetTransform()->SetWorldMoveXZ(TargetDir_ * 200.0f * _DeltaTime);
//	}
//
//
//
//	// 우클릭 위치로 몸을 향합니다.
//	float4 dir = float4::Cross3D(CurFowordDir_, TargetDir_);
//
//	float goaldegree = GameEngineMath::UnitVectorToDegree(TargetDir_.z, TargetDir_.x);
//	if (goaldegree == GetTransform()->GetLocalRotation().y)
//	{
//		return;
//	}
//	if (dir.y >= 0.f)
//	{
//
//		GetTransform()->AddLocalRotationDegreeY(YRotateSpeed_ * _DeltaTime);
//		CurFowordDir_.RotateYDegree(YRotateSpeed_ * _DeltaTime);
//
//		dir = float4::Cross3D(CurFowordDir_, TargetDir_);
//		if (dir.y < 0.f)
//		{
//			GetTransform()->SetLocalRotationDegree({ 0.f,goaldegree,0.f });
//			CurFowordDir_ = TargetDir_;
//		}
//	}
//
//	else if (dir.y < 0.f)
//	{
//		GetTransform()->AddLocalRotationDegreeY(-YRotateSpeed_ * _DeltaTime);
//		CurFowordDir_.RotateYDegree(-YRotateSpeed_ * _DeltaTime);
//
//		dir = float4::Cross3D(CurFowordDir_, TargetDir_);
//		if (dir.y > 0.f)
//		{
//			GetTransform()->SetLocalRotationDegree({ 0.f,goaldegree,0.f });
//			CurFowordDir_ = TargetDir_;
//		}
//	}
//
//	
//	
//}
//
//void YSJ_Char::Move_End()
//{
//	TargetDir_ = float4::ZERO;
//	arrivalPos_ = float4::ZERO;
//	IsMove_ = false;
//	return;
//}