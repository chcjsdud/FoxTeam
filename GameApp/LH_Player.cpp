#include "PreCompile.h"
#include "LH_Player.h"

#include "PlayerController.h"

#include <GameEngine/GameEngineCollision.h>

#include "LH_LockOnUI.h"

//
//Player Global Func
//

Player::Player()
	: SkillUI_(nullptr)
	, LockOnUI_(nullptr)
	, JobType_(JobType::NONE)
{
	{
		ActionState_.CreateState<Player>
			("Action_Q", this, &Player::Action_Q_Start, &Player::Action_Q_Update, &Player::Action_Q_End);
		ActionState_.CreateState<Player>
			("Action_W", this, &Player::Action_W_Start, &Player::Action_W_Update, &Player::Action_W_End);
		ActionState_.CreateState<Player>
			("Action_E", this, &Player::Action_E_Start, &Player::Action_E_Update, &Player::Action_E_End);
		ActionState_.CreateState<Player>
			("Action_R", this, &Player::Action_R_Start, &Player::Action_R_Update, &Player::Action_R_End);
		ActionState_.CreateState<Player>
			("Action_D", this, &Player::Action_D_Start, &Player::Action_D_Update, &Player::Action_D_End);
	}

	{
		OrderState_.CreateState<Player>
			("Order_Q", this, &Player::Order_Q_Start, &Player::Order_Q_Update, &Player::Order_Q_End);

		OrderState_.CreateState<Player>
			("Order_W", this, &Player::Order_W_Start, &Player::Order_W_Update, &Player::Order_W_End);

		OrderState_.CreateState<Player>
			("Order_E", this, &Player::Order_E_Start, &Player::Order_E_Update, &Player::Order_E_End);

		OrderState_.CreateState<Player>
			("Order_R", this, &Player::Order_R_Start, &Player::Order_R_Update, &Player::Order_R_Start);

		OrderState_.CreateState<Player>
			("Order_D", this, &Player::Order_D_Start, &Player::Order_D_Update, &Player::Order_D_Start);
	}
}

Player::~Player()
{

}

void Player::Start()
{
}

void Player::Update(float _DeltaTime)
{
	//DEBUGUpdate(_DeltaTime);
	//
	//State_.Update(_DeltaTime);
	//CameraState_.Update(_DeltaTime);
}

void Player::ControllerOrderUpdate()
{
	Controller_Order_ = Controller_->Controller_GetOrder();
}

void Player::StaminaRecoverUpdate(float _DeltaTime)
{
	Unit_Status_Final_.Stat_StaminaRecovery_ += _DeltaTime * Unit_Status_Final_.Stat_StaminaRecovery_;

	if (Unit_Status_Final_.Stat_StaminaRecovery_ > 100.f)
	{
		Unit_Status_Final_.Stat_StaminaRecovery_ = 100.f;
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