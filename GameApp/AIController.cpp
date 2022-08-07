#include "PreCompile.h"
#include "AIController.h"

#include "GameMouse.h"

#include "UI_Skill.h"
#include "LH_LockOnUI.h"

#include "LH_Player.h"

AIController::AIController() // default constructer 디폴트 생성자
{
	AIState_.CreateState<AIController>
		("Idle", this, &AIController::Idle_Start, &AIController::Idle_Update, &AIController::Idle_End);
	AIState_.CreateState<AIController>
		("Chase", this, &AIController::Chase_Start, &AIController::Chase_Update, &AIController::Chase_End);
	AIState_.CreateState<AIController>
		("Attack", this, &AIController::Attack_Start, &AIController::Attack_Update, &AIController::Attack_End);

	AIState_.ChangeState("Idle");
}

AIController::~AIController() // default destructer 디폴트 소멸자
{

}

//void AIController::PlayerStateUpdate()
//{
//	if (Key_RB_ == true)
//	{
//		if (IsAttack_ = true) // 마우스의 충돌 처리가 해결해줄것
//		{
//			MainUnit_->Unit_SetTarget(TargetActor_);
//
//
//		}
//		//우선 마우스로 부터 좌표를 받아오는게 먼저
//		MainUnit_->Unit_SetTargetDir(TmepMouseDir_);
//		MainUnit_->Unit_SetTargetPos(TempMousePos_);
//		MainUnit_->Unit_SetMove(true);
//
//		//타겟과의 거리가 매우 짧으면 리턴 시킬것
//
//		//이미 움직이는 중인데 다시 state를 갱신시키지 않기 위해
//		ChangePlayerState("Walk");
//	}
//
//	if (Key_Stop_ == true)
//	{
//		MainUnit_->Unit_SetTargetDir(float4::ZERO);
//		MainUnit_->Unit_SetTargetPos(float4::ZERO);
//		MainUnit_->Unit_SetMove(false);
//
//		ChangePlayerState("Idle");
//	}
//}

void AIController::Update(float _DeltaTime)
{
	AIState_.Update(_DeltaTime);
}

void AIController::ChangePlayerState(std::string _State)
{
	if (MainUnit_->Unit_GetStateName() != _State)
	{
		MainUnit_->Unit_ChangeState(_State);
	}
}

void AIController::AIController_Off()
{

}

void AIController::AIController_On()
{

}






void AIController::Idle_Start()
{
}

void AIController::Idle_Update(float _DeltaTime)
{
}

void AIController::Idle_End()
{
}

void AIController::Chase_Start()
{
}

void AIController::Chase_Update(float _DeltaTime)
{
}

void AIController::Chase_End()
{
}

void AIController::Attack_Start()
{
}

void AIController::Attack_Update(float _DeltaTime)
{
}

void AIController::Attack_End()
{
}