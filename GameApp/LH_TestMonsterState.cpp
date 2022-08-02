#include "PreCompile.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

#include "LH_Player.h"
#include "LH_TestMonster.h"
#include "Enums.h"

#define MMonsterChangeState(Name) 	monsterState_.ChangeState(Name); return

void TestMonster::Idle_Start()
{
	//Speed_ = 0.f;
}
void TestMonster::Idle_Update(float _DeltaTime)
{
	// Idle ���¿��� ���� ���� ���� �ȿ� �÷��̾ �浹�ϸ� �Լ� �������� State �� Pursuit(����) ���� �����ϴ�.
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
//		// �� �ð� �ȿ� �߰� ���� �ϸ� ���ӱⰡ ����, ���ݽ� ����
//		AttackTime_ = 1.f;
//
//		//���ӱ� �ܰ�, 3�ܰ� ������ �ٽ� 0����
//		AttackLevel_ = 0;
//
//		//0.5�� �������� �����Ѵ�. ���ݽ� ����
//		AttackTurm_ = 0.5f;
//
//		//0.1�� ���ȸ� Ÿ�� ������ �ִ�. ���ݽ� ����
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
//		//Ÿ�� �߿��� ȸ������ ����
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
//				//���� �ܰ� ����, ���ϸ��̼� �ٲٱ�
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

	//�°ų� �̷����� ��Ȳ���� ������ State�� ���� �Ǿ�����
	monsterAttackHitBoxCollision_->Off();
}

void TestMonster::CollisionSight(GameEngineCollision* _other)
{
	monsterState_.ChangeState("Pursuit");
	return;
}

// ������Ʈ���� ��� ����Ǿ� �÷��̾��� ���� �浹ü���� ���� üũ, �˹� ���� �����մϴ�.
void TestMonster::DamageUpdate(float _DeltaTime)
{
	if (0 >= InvincibleTermTimer_)
	{
		isDamaged_ = false;
	}
	else
	{
		InvincibleTermTimer_ -= _DeltaTime;
		
	}

	KnockBackUpdate(_DeltaTime);

	monsterHitBoxCollision_->Collision(CollisionType::AABBBox3D, CollisionType::AABBBox3D, static_cast<int>(CollisionGroup::PlayerAttack),
		std::bind(&TestMonster::CollisionGetDamaged, this, std::placeholders::_1));


	return;
}

void TestMonster::KnockBackUpdate(float _DeltaTime)
{
	if (true == isKnockBack_)
	{
		if (0 >= KnockBackTimer_)
		{
			KnockBackTimer_ = KnockBackRate_;
			isKnockBack_ = false;
			return;
		}

		KnockBackTimer_ -= _DeltaTime;
		GetTransform()->SetWorldMove(-PursuitDir_ * KnockBackSpeed_ * _DeltaTime);
	}


	return;
}

void TestMonster::DeathUpdate(float _DeltaTime)
{
	if (0 >= Hp_)
	{
		this->Death();
	}

	return;
}

void TestMonster::CollisionGetDamaged(GameEngineCollision* _other)
{
	if (false == isDamaged_)
	{
		this->Hp_ -= targetPlayer_->Unit_GetAttackPower();
		isDamaged_ = true;
		isKnockBack_ = true;
		InvincibleTermTimer_ = InvincibleTermRate_;
		return;
	}

	return;
}
