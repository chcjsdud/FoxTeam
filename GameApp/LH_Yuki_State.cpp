#include "PreCompile.h"
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

#include "LH_Yuki.h"

//
//State Cpp
//

// �� �ٸ������� �� �Űܾ���


//Idle
void Player_Yuki::Idle_Start()
{
	//Speed_ = 0.f;
}
void Player_Yuki::Idle_Update(float _DeltaTime)
{
	// ��Ʈ�ѷ��� ���� ���� ����� �������� �̵� ������ �����ǰԲ�

	//if (true == GameEngineInput::GetInst().Press("W")||
	//	true == GameEngineInput::GetInst().Press("A")||
	//	true == GameEngineInput::GetInst().Press("S")||
	//	true == GameEngineInput::GetInst().Press("D"))
	if(IsMove_ == true)
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
void Player_Yuki::Idle_End()
{
}

//Walk
void Player_Yuki::Walk_Start()
{
	//IsMove_ = true;
	//Speed_ = 300.f;
}
void Player_Yuki::Walk_Update(float _DeltaTime)
{
	// ũ�Ⱑ 2¥�� ť�� �����, 2��°�� ���� Key�� �ƴϸ� return ��Ŵ, �׸��� �ȴ������� �ٽ� ��ȯ���Ѽ� ���� ������ 2���� ���� ���ֱ�
	// 
	if (GameEngineInput::GetInst().Press("Attack"))
	{
		State_.ChangeState("Attack");
		return;
	}

	//if (false == GameEngineInput::GetInst().Press("W") &&
	//	false == GameEngineInput::GetInst().Press("A") &&
	//	false == GameEngineInput::GetInst().Press("S") &&
	//	false == GameEngineInput::GetInst().Press("D"))
	if(IsMove_ == false)
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
void Player_Yuki::Walk_End()
{
	//IsMove_ = false;
}

//Attack
void Player_Yuki::Attack_Start()
{
	if (Status_Final_.Stat_Stamina_ < 5.f)
	{
		//���׹̳� ������ �����Ÿ���
		State_.ChangeState("Idle");
		return;
	}

	// �� �ð� �ȿ� �߰� ���� �ϸ� ���ӱⰡ ����, ���ݽ� ����
	//AttackTime_ = 0.5f;

	//���ӱ� �ܰ�, 3�ܰ� ������ �ٽ� 0����
	//AttackLevel_ = 0;

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
void Player_Yuki::Attack_Update(float _DeltaTime)
{
	//KeyDirUpdate(_DeltaTime);

	AttackTurm_ -= _DeltaTime;
	//AttackTime_ -= _DeltaTime;
	AttackHitTime_ -= _DeltaTime;

	// Attack Ready�� �����ؾ���

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

	//�°ų� �̷����� ��Ȳ���� ������ State�� ���� �Ǿ�����
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