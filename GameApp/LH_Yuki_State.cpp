#include "PreCompile.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

#include "LH_Yuki.h"

//
//State Cpp
//

// 곧 다른곳으로 다 옮겨야함


//Idle
void Player_Yuki::Idle_Start()
{
	//Speed_ = 0.f;
}
void Player_Yuki::Idle_Update(float _DeltaTime)
{
	if (EnemyInSight_ == true) //적이 시야 내에 있으면
	{
		//사거리까지 적을 추적함
	}

	StaminaRecoverUpdate(_DeltaTime);

	CurDirUpdate(_DeltaTime);

	MoveRotateUpdate(_DeltaTime);
}
void Player_Yuki::Idle_End()
{
}

//Walk
void Player_Yuki::Walk_Start()
{
	FowordDir_ = TargetDir_;
	//IsMove_ = true;
	//Speed_ = 300.f;
}
void Player_Yuki::Walk_Update(float _DeltaTime)
{
	CurDirUpdate(_DeltaTime);
	MoveUpdate(_DeltaTime);
	MoveRotateUpdate(_DeltaTime);
	// 걷다가 중간에 공격을 해도 회전을 끝마쳐야함

	// 사거리 안에 적이 있고, 특별한 명령이 내려지지 않았거나 공격 명령이 내려진 상태라면 적을 공격함

	if (GetTransform()->GetWorldPosition() == Target_->GetTransform()->GetWorldPosition())
	{
		State_.ChangeState("Idle");
		return;
	}
}
void Player_Yuki::Walk_End()
{
	//IsMove_ = false;
}

//Attack
void Player_Yuki::Attack_Start()
{
	if (Status_Final_.Stat_Stamina_ < 5.f)
	{
		//스테미나 게이지 깜빡거리기
		State_.ChangeState("Idle");
		return;
	}

	// 이 시간 안에 추가 공격 하면 연속기가 나감, 공격시 갱신
	//AttackTime_ = 0.5f;

	//연속기 단계, 3단계 넘으면 다시 0으로
	//AttackLevel_ = 0;

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
	;
}
void Player_Yuki::Attack_Update(float _DeltaTime)
{
	//KeyDirUpdate(_DeltaTime);

	AttackTurm_ -= _DeltaTime;
	//AttackTime_ -= _DeltaTime;
	AttackHitTime_ -= _DeltaTime;

	FowordDir_ = TargetDir_;
	//TargetDir_을 실시간으로 업데이트 해 줘야함
	// 어디서??

	// Attack Ready도 구현해야함

	if (AttackHitTime_ <= 0)
	{
		MoveRotateUpdate(_DeltaTime);
		AttackHitBoxCollision_->Off();
	}

	else
	{
		State_.ChangeState("Idle");
		return;
	}
}
void Player_Yuki::Attack_End()
{
	AttackTurm_ = 0.f;
	//AttackTime_ = 0.f;
	//AttackLevel_ = 0;
	AttackHitTime_ = 0.f;

	//맞거나 이런저런 상황으로 강제로 State가 종료 되었을때
	AttackHitBoxCollision_->Off();
}

void Player_Yuki::Attack_Ready_Start()
{
}

void Player_Yuki::Attack_Ready_Update(float _DeltaTime)
{
}

void Player_Yuki::Attack_Ready_End()
{
}



// 마우스 커서 이동용
// 명령을 컨트롤러에서 하달받고
// 하달받은 스테이트와 장입된 변수만으로 일을 처리합니다.

//void Player_Yuki::Stand_Start()
//{
//	IsMove_ = false;
//	return;
//}
//
//void Player_Yuki::Stand_Update(float _DeltaTime)
//{
//	if (true == IsMove_)
//	{
//		State_.ChangeState("Move");
//	}
//
//	return;
//}
//
//void Player_Yuki::Stand_End()
//{
//	return;
//}
//
//void Player_Yuki::Move_Start()
//{
//
//	int a = 0;
//	return;
//}
//
//void Player_Yuki::Move_Update(float _DeltaTime)
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
//void Player_Yuki::Move_End()
//{
//	TargetDir_ = float4::ZERO;
//	arrivalPos_ = float4::ZERO;
//	IsMove_ = false;
//	return;
//}