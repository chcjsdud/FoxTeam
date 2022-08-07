#include "PreCompile.h"
#include "AIController.h"

#include "GameMouse.h"

#include "UI_Skill.h"
#include "LH_LockOnUI.h"

#include "LH_Player.h"

AIController::AIController() // default constructer ����Ʈ ������
{
	AIState_.CreateState<AIController>
		("Idle", this, &AIController::Idle_Start, &AIController::Idle_Update, &AIController::Idle_End);
	AIState_.CreateState<AIController>
		("Chase", this, &AIController::Chase_Start, &AIController::Chase_Update, &AIController::Chase_End);
	AIState_.CreateState<AIController>
		("Attack", this, &AIController::Attack_Start, &AIController::Attack_Update, &AIController::Attack_End);

	AIState_.ChangeState("Idle");
}

AIController::~AIController() // default destructer ����Ʈ �Ҹ���
{

}

//void AIController::PlayerStateUpdate()
//{
//	if (Key_RB_ == true)
//	{
//		if (IsAttack_ = true) // ���콺�� �浹 ó���� �ذ����ٰ�
//		{
//			MainUnit_->Unit_SetTarget(TargetActor_);
//
//
//		}
//		//�켱 ���콺�� ���� ��ǥ�� �޾ƿ��°� ����
//		MainUnit_->Unit_SetTargetDir(TmepMouseDir_);
//		MainUnit_->Unit_SetTargetPos(TempMousePos_);
//		MainUnit_->Unit_SetMove(true);
//
//		//Ÿ�ٰ��� �Ÿ��� �ſ� ª���� ���� ��ų��
//
//		//�̹� �����̴� ���ε� �ٽ� state�� ���Ž�Ű�� �ʱ� ����
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