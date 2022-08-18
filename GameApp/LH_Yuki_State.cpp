#include "PreCompile.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

#include "LH_Yuki.h"

//
//State Cpp
//

// �� �ٸ������� �� �Űܾ���

void Player_Yuki::Order_Q_Start()
{
}

void Player_Yuki::Order_Q_Update(float _DeltaTime)
{
}

void Player_Yuki::Order_Q_End()
{
}

void Player_Yuki::Order_W_Start()
{
}

void Player_Yuki::Order_W_Update(float _DeltaTime)
{
}

void Player_Yuki::Order_W_End()
{
}

void Player_Yuki::Order_E_Start()
{
}

void Player_Yuki::Order_E_Update(float _DeltaTime)
{
}

void Player_Yuki::Order_E_End()
{
}

void Player_Yuki::Order_R_Start()
{
}

void Player_Yuki::Order_R_Update(float _DeltaTime)
{
}

void Player_Yuki::Order_R_End()
{
}

void Player_Yuki::Order_D_Start()
{
}

void Player_Yuki::Order_D_Update(float _DeltaTime)
{
}

void Player_Yuki::Order_D_End()
{
}


//Idle
void Player_Yuki::Action_Idle_Start()
{
	//Speed_ = 0.f;
}
void Player_Yuki::Action_Idle_Update(float _DeltaTime)
{
	if (EnemyInSight_ == true) //���� �þ� ���� ������
	{
		//��Ÿ����� ���� ������
	}

	StaminaRecoverUpdate(_DeltaTime);

	CurDirUpdate(_DeltaTime);

	MoveRotateUpdate(_DeltaTime);
}
void Player_Yuki::Action_Idle_End()
{
}

//Walk
void Player_Yuki::Action_Walk_Start()
{
	//Move_ForwardDir_ = TargetDir_;
	//IsMove_ = true;
	//Speed_ = 300.f;
}
void Player_Yuki::Action_Walk_Update(float _DeltaTime)
{
	MoveUpdate(_DeltaTime);
	// �ȴٰ� �߰��� ������ �ص� ȸ���� �����ľ���

	// ��Ÿ� �ȿ� ���� �ְ�, Ư���� ����� �������� �ʾҰų� ���� ����� ������ ���¶�� ���� ������

	if (GetTransform()->GetWorldPosition() == Target_Unit_->GetTransform()->GetWorldPosition())
	{
		ActionState_.ChangeState("Idle");
		return;
	}
}
void Player_Yuki::Action_Walk_End()
{
	//IsMove_ = false;
}

//Attack
void Player_Yuki::Action_Attack_Start()
{
	if (Unit_Status_Final_.Stat_Stamina_ < 5.f)
	{
		//���׹̳� ������ �����Ÿ���
		ActionState_.ChangeState("Idle");
		return;
	}

	// �� �ð� �ȿ� �߰� ���� �ϸ� ���ӱⰡ ����, ���ݽ� ����
	//AttackTime_ = 0.5f;

	//���ӱ� �ܰ�, 3�ܰ� ������ �ٽ� 0����
	//AttackLevel_ = 0;

	//0.5�� �������� �����Ѵ�. ���ݽ� ����
	Unit_AttackTurm_ = 0.25f;

	//0.1�� ���ȸ� Ÿ�� ������ �ִ�. ���ݽ� ����
	Unit_AttackHitTime_ = 0.1f;

	//Status_Final_.Stat_Stamina_ -= Status_Final_.Stat_Attack_Stamina_;

	//if (Status_Final_.Stat_Stamina_ < 0.f)
	//{
	//	Status_Final_.Stat_Stamina_ = 0.f;
	//}
	;
}
void Player_Yuki::Action_Attack_Update(float _DeltaTime)
{
	//KeyDirUpdate(_DeltaTime);

	Unit_AttackTurm_ -= _DeltaTime;
	//AttackTime_ -= _DeltaTime;
	Unit_AttackHitTime_ -= _DeltaTime;

	//Move_ForwardDir_ = TargetDir_;
	//TargetDir_�� �ǽð����� ������Ʈ �� �����
	// ���??

	// Attack Ready�� �����ؾ���

	if (Unit_AttackHitTime_ <= 0)
	{
		MoveRotateUpdate(_DeltaTime);
	}

	else
	{
		ActionState_.ChangeState("Idle");
		return;
	}
}
void Player_Yuki::Action_Attack_End()
{
	Unit_AttackTurm_ = 0.f;
	//AttackTime_ = 0.f;
	//AttackLevel_ = 0;
	Unit_AttackHitTime_ = 0.f;
}


void Player_Yuki::Action_Q_Start()
{
}

void Player_Yuki::Action_Q_Update(float _DeltaTime)
{
}

void Player_Yuki::Action_Q_End()
{
}

void Player_Yuki::Action_W_Start()
{
}

void Player_Yuki::Action_W_Update(float _DeltaTime)
{
}

void Player_Yuki::Action_W_End()
{
}

void Player_Yuki::Action_E_Start()
{
}

void Player_Yuki::Action_E_Update(float _DeltaTime)
{
}

void Player_Yuki::Action_E_End()
{
}

void Player_Yuki::Action_R_Start()
{
}

void Player_Yuki::Action_R_Update(float _DeltaTime)
{
}

void Player_Yuki::Action_R_End()
{
}

void Player_Yuki::Action_D_Start()
{
}

void Player_Yuki::Action_D_Update(float _DeltaTime)
{
}

void Player_Yuki::Action_D_End()
{
}
// ���콺 Ŀ�� �̵���
// ����� ��Ʈ�ѷ����� �ϴ޹ް�
// �ϴ޹��� ������Ʈ�� ���Ե� ���������� ���� ó���մϴ�.

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
//	// ���콺 Ŀ�� ��Ŭ���� ��ġ���� ���� ���͸� ����մϴ�.
//
//	if (static_cast<int>(arrivalPos_.x) == static_cast<int>(GetTransform()->GetWorldPosition().x) 
//		&& static_cast<int>(arrivalPos_.z) == static_cast<int>(GetTransform()->GetWorldPosition().z)) // ��ǥ ��ġ�� �ٴٸ���
//	{
//		IsMove_ = false;
//		State_.ChangeState("Stand");
//	}
//
//	if (targetPos_ != GetTransform()->GetWorldPosition())
//	{
//		TargetDir_ = targetPos_;
//		TargetDir_.Normalize3D(); // ����ȭ
//	}
//
//	//MoveUpdate(_DeltaTime);
//	// ��Ŭ�� ��ġ�� �̵��մϴ�.
//	{
//		GetTransform()->SetWorldMoveXZ(TargetDir_ * 200.0f * _DeltaTime);
//	}
//
//
//
//	// ��Ŭ�� ��ġ�� ���� ���մϴ�.
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