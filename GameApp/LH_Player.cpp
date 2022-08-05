#include "PreCompile.h"
#include "LH_Player.h"

#include "PlayerController.h"

#include <GameEngine/GameEngineCollision.h>

#include "LH_LockOnUI.h"

//
//Player Global Func
//

Player::Player() 
{
}

Player::~Player() 
{

}

void Player::Start()
{

	PlayController_->AddPlayerMap(GetName(), this);
	//GetLevel()->GetMainCameraActor()->FreeCameraModeSwitch();
}

void Player::Update(float _DeltaTime)
{
	//DEBUGUpdate(_DeltaTime);
	//
	//State_.Update(_DeltaTime);
	//CameraState_.Update(_DeltaTime);
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

//void Player::RockOnUpdate(float _DeltaTime)
//{
//	if (true == GameEngineInput::GetInst().Press("RockOn"))
//	{
//		LockOnUI_->On();
//
//		GameEngineCollision* RockOnPtr = UnitSightCollision_->CollisionPtr(CINT(CollisionGroup::Monster));
//
//		//PlayerRockOnCollision_->Collision(CollisionType::CirCle, CollisionType::AABBBox3D, static_cast<int>(CollisionGroup::Player), std::bind(&Player::test, this));
//
//		if (RockOnPtr != nullptr)
//		{
//			Target_ = RockOnPtr->GetActor();
//
//			RockOnDirUpdate(_DeltaTime);
//		}
//		else
//		{
//			Target_ = nullptr;
//		}
//	}
//
//	else
//	{
//		LockOnUI_->Off();
//		Target_ = nullptr;
//	}
//}