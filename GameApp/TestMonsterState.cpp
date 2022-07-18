#include "PreCompile.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

#include "Player.h"
#include "TestMonster.h"
#include "Enums.h"

#define MMonsterChangeState(Name) 	monsterState_.ChangeState(Name); return

void TestMonster::Idle_Start()
{
	Speed_ = 0.f;
}
void TestMonster::Idle_Update(float _DeltaTime)
{
	// Idle 상태에서 만약 인지 범위 안에 플레이어가 충돌하면 함수 실행해줘 State 를 Pursuit(추적) 으로 돌립니다.
	monsterSightBoxCollision_->Collision(CollisionType::CirCle, CollisionType::AABBBox3D, static_cast<int>(CollisionGroup::Player), std::bind(&TestMonster::CollisionSight, this, std::placeholders::_1));
	return;
}
void TestMonster::Idle_End()
{
	return;
}

//Walk
void TestMonster::Pursuit_Start()
{
	IsMove_ = true;
	Speed_ = 70.f;
	return;
}
void TestMonster::Pursuit_Update(float _DeltaTime)
{
	PursuitDirUpdate(_DeltaTime);
	MoveUpdate(_DeltaTime);
	MoveRotateUpdate(_DeltaTime);

	return;
}
void TestMonster::Pursuit_End()
{
	IsMove_ = false;
}


//Attack
void TestMonster::Attack_Start()
{
//	if (AttackTurm_ == 0)
//	{
//		// 이 시간 안에 추가 공격 하면 연속기가 나감, 공격시 갱신
//		AttackTime_ = 1.f;
//
//		//연속기 단계, 3단계 넘으면 다시 0으로
//		AttackLevel_ = 0;
//
//		//0.5초 간격으로 공격한다. 공격시 갱신
//		AttackTurm_ = 0.5f;
//
//		//0.1초 동안만 타격 판정이 있다. 공격시 갱신
//		AttackHitTime_ = 0.1f;
//
//		PlayerAttackHitBoxCollision_->On();
//	}
}
void TestMonster::Attack_Update(float _DeltaTime)
{
//	KeyDirUpdate(_DeltaTime);
//
//	//AttackTurm_ -= _DeltaTime;
//	//AttackTime_ -= _DeltaTime;
//	//AttackHitTime_ -= _DeltaTime;
//
//	if (AttackHitTime_ <= 0)
//	{
//		//타격 중에는 회전하지 않음
//		MoveRotateUpdate(_DeltaTime);
//		monsterAttackHitBoxCollision_->Off();
//	}
//
//	if (AttackTime_ > 0.f)
//	{
//		if (AttackTurm_ <= 0.f)
//		{
//			if (GameEngineInput::GetInst().Press("Attack"))
//			{
//				//다음 단계 공격, 에니메이션 바꾸기
//				monsterAttackHitBoxCollision_->On();
//
//				AttackTime_ = 1.f;
//				AttackTurm_ = 0.5f;
//				AttackHitTime_ = 0.1f;
//
//				//++AttackLevel_;
//				if (AttackLevel_ == 2)
//				{
//					AttackLevel_ = 0;
//				}
//			}
//		}
//	}
	//
	//else
	{
		monsterState_.ChangeState("Idle");
		return;
	}
}
void TestMonster::Attack_End()
{
	//AttackTurm_ = 0.f;
	//AttackTime_ = 0.f;
	//AttackLevel_ = 0;
	//AttackHitTime_ = 0.f;

	//맞거나 이런저런 상황으로 강제로 State가 종료 되었을때
	monsterAttackHitBoxCollision_->Off();
}

void TestMonster::CollisionSight(GameEngineCollision* _other)
{
	monsterState_.ChangeState("Pursuit");
	return;
}