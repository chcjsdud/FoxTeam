#include "PreCompile.h"
#include "LH_Player.h"

#include <GameEngine/GameEngineCollision.h>

#include "LH_LockOnUI.h"

//
//Player Global Func
//

Player::Player() 
	//: FBXRenderer_(nullptr)
	//, PlayerGroundCollision_(nullptr)
	//, PlayerHitBoxCollision_(nullptr)
	//, PlayerAttackHitBoxCollision_(nullptr)
	//, PlayerLockOnCollision_(nullptr)
	//, Inventory_(nullptr)
	//, LockOnUI_(nullptr)
	//, SkillUI_(nullptr)
{
	//PlayerStatusBase_.Stat_Hp_ = 0;
	//PlayerStatusBase_.Stat_MaxHp_ = 100;
	//PlayerStatusBase_.Stat_Stamina_ = 0.f;
	//PlayerStatusBase_.Stat_MaxStamina_ = 100.f;
	//PlayerStatusBase_.Stat_AttackPower_ = 10;
	//PlayerStatusBase_.Stat_Speed_ = 300;
	//PlayerStatusBase_.Stat_RunSpeed_ = 600;
	//PlayerStatusBase_.Stat_Stamina_RecoverRate_ = 1.f;
}

Player::~Player() 
{

}

void Player::Start()
{
	//GetLevel()->GetMainCameraActor()->FreeCameraModeSwitch();
}

void Player::Update(float _DeltaTime)
{
	//DEBUGUpdate(_DeltaTime);
	//
	//State_.Update(_DeltaTime);
	//CameraState_.Update(_DeltaTime);
}

void Player::CurDirUpdate(float _DeltaTime)	// 마우스 커서 우클릭한 위치로의 방향 벡터를 계산합니다.
{
	// RockOnUpdate(_DeltaTime);
	
	KeyDirUpdate(_DeltaTime);
}

void Player::StaminaRecoverUpdate(float _DeltaTime)
{
	Status_Final_.Stat_StaminaRecovery_ += _DeltaTime* Status_Final_.Stat_StaminaRecovery_;

	if (Status_Final_.Stat_StaminaRecovery_ > 100.f)
	{
		Status_Final_.Stat_StaminaRecovery_ = 100.f;
	}
}

//bool Player::EquipItem(std::string _BuffName, PlayerStatus* _PlayerStatus)
//{
//	PlayerStatus* NewPlayerStatus = new PlayerStatus;
//	*NewPlayerStatus = *_PlayerStatus;
//
//	Player_BufferList_.insert(std::make_pair(_BuffName, _PlayerStatus));
//
//	return false;
//}

void Player::RockOnUpdate(float _DeltaTime)
{
	if (true == GameEngineInput::GetInst().Press("RockOn"))
	{
		LockOnUI_->On();

		GameEngineCollision* RockOnPtr = UnitSightCollision_->CollisionPtr(CINT(CollisionGroup::Monster));

		//PlayerRockOnCollision_->Collision(CollisionType::CirCle, CollisionType::AABBBox3D, static_cast<int>(CollisionGroup::Player), std::bind(&Player::test, this));

		if (RockOnPtr != nullptr)
		{
			Target_ = RockOnPtr->GetActor();

			RockOnDirUpdate(_DeltaTime);
		}
		else
		{
			Target_ = nullptr;
		}
	}

	else
	{
		LockOnUI_->Off();
		Target_ = nullptr;
	}
}

void Player::RockOnDirUpdate(float _DeltaTime)
{
	if (nullptr != Target_)
	{
		float4 MoveDir = CalculateTargetDir(Target_->GetTransform()->GetWorldPosition());

		MoveDir.Normalize3D();
		TargetDir_ = MoveDir;
		FowordDir_ = MoveDir;
	}
}

void Player::KeyDirUpdate(float _DeltaTime)
{
	float4 MoveDir = float4::ZERO;

	bool Key = false;

	if (true == GameEngineInput::GetInst().Press("D"))
	{
		MoveDir.x += 1.f;

		//IsMove_ = true;
		Key = true;
	}
	if (true == GameEngineInput::GetInst().Press("A"))
	{
		MoveDir.x -= 1.f;

		//IsMove_ = true;
		Key = true;
	}
	if (true == GameEngineInput::GetInst().Press("W"))
	{
		MoveDir.z += 1.f;

		//IsMove_ = true;
		Key = true;
	}
	if (true == GameEngineInput::GetInst().Press("S"))
	{
		MoveDir.z -= 1.f;

		//IsMove_ = true;
		Key = true;
	}

	//if (true == IsMove_)
	if (true == Key)
	{
		MoveDir.Normalize3D();
		KeyDir_ = MoveDir;

		if (Target_ == nullptr)
		{
			FowordDir_ = KeyDir_;
		}
		//IsMove_ = false;
	}
}

void Player::MoveUpdate(float _DeltaTime)
{
	if (true == IsMove_)
	{
		GetTransform()->SetWorldMove(KeyDir_ * Status_Final_.Stat_MoveSpeed_ * _DeltaTime);
	}
}

void Player::MoveRotateUpdate(float _DeltaTime)
{
	//if (Target_ == nullptr && IsMove_ == false)
	//{
	//	return;
	//}

	if (CurFowordDir_ == FowordDir_)
	{
		return;
	}

	float4 dir = float4::Cross3D(CurFowordDir_, FowordDir_);

	float goaldegree = GameEngineMath::UnitVectorToDegree(FowordDir_.z, FowordDir_.x);

	if (dir.y >= 0.f)
	{

		GetTransform()->AddLocalRotationDegreeY(YRotateSpeed_ * _DeltaTime);
		CurFowordDir_.RotateYDegree(YRotateSpeed_ * _DeltaTime);

		dir = float4::Cross3D(CurFowordDir_, FowordDir_);
		if (dir.y < 0.f)
		{
			GetTransform()->SetLocalRotationDegree({ 0.f,goaldegree,0.f });
			CurFowordDir_ = FowordDir_;
		}
	}

	else if (dir.y < 0.f)
	{
		GetTransform()->AddLocalRotationDegreeY(-YRotateSpeed_ * _DeltaTime);
		CurFowordDir_.RotateYDegree(-YRotateSpeed_ * _DeltaTime);

		dir = float4::Cross3D(CurFowordDir_, FowordDir_);
		if (dir.y > 0.f)
		{
			GetTransform()->SetLocalRotationDegree({ 0.f,goaldegree,0.f });
			CurFowordDir_ = FowordDir_;
		}
	}
}

void Player::CameraUpdate_BackPosition(float _DeltaTime)
{
	if (true == GetLevel()->GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}
	else
	{
		float4 rot = GetLevel()->GetMainCameraActor()->GetTransform()->GetLocalRotation();

		float4 CamPos = GetTransform()->GetWorldPosition();
		CamPos.y += 200.f;
		CamPos.x -= 100 * sinf(rot.y * GameEngineMath::DegreeToRadian);
		CamPos.z -= 100 * cosf(rot.y * GameEngineMath::DegreeToRadian);

		GetLevel()->GetMainCameraActor()->GetTransform()->SetWorldPosition(CamPos);
	}
}

void Player::CameraUpdate_UpPosition(float _DeltaTime)
{
	if (true == GetLevel()->GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}
	else
	{
		//float4 rot = GetLevel()->GetMainCameraActor()->GetTransform()->GetLocalRotation();

		float4 CamPos = GetTransform()->GetWorldPosition();
		CamPos.y += 400.f;
		CamPos.z -= 400;

		GetLevel()->GetMainCameraActor()->GetTransform()->SetWorldPosition(CamPos);
		GetLevel()->GetMainCameraActor()->GetTransform()->SetWorldRotationDegree({ 25.f,0.f,0.f });
	}
}

void Player::CameraUpdate_EternalReturn(float _DeltaTime)
{
	if (true == GetLevel()->GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}
	else
	{
		float4 CamPos = GetTransform()->GetWorldPosition();
		CamPos.y += 700.f;
		CamPos.z -= 550;
		
		GetLevel()->GetMainCameraActor()->GetTransform()->SetWorldPosition(CamPos);
		GetLevel()->GetMainCameraActor()->GetTransform()->SetWorldRotationDegree({ 60.f,0.f,0.f });

	}
}