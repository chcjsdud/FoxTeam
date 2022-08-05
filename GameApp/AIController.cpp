#include "PreCompile.h"
#include "AIController.h"

#include "GameMouse.h"

#include "UI_Skill.h"
#include "LH_LockOnUI.h"

#include "LH_Player.h"

AIController::AIController() // default constructer 디폴트 생성자
{

}

AIController::~AIController() // default destructer 디폴트 소멸자
{

}

void AIController::InitInput()
{

}

void AIController::UIInit()
{

}


void AIController::Start()
{
	InitInput();
}

void AIController::PlayerStateUpdate()
{
	if (Key_RB_ == true)
	{
		if (IsAttack_ = true) // 마우스의 충돌 처리가 해결해줄것
		{
			MainUnit_->Unit_SetTarget(TargetActor_);


		}
		//우선 마우스로 부터 좌표를 받아오는게 먼저
		MainUnit_->Unit_SetTargetDir(TmepMouseDir_);
		MainUnit_->Unit_SetTargetPos(TempMousePos_);
		MainUnit_->Unit_SetMove(true);

		//타겟과의 거리가 매우 짧으면 리턴 시킬것

		//이미 움직이는 중인데 다시 state를 갱신시키지 않기 위해
		ChangePlayerState("Walk");
	}

	if (Key_Stop_ == true)
	{
		MainUnit_->Unit_SetTargetDir(float4::ZERO);
		MainUnit_->Unit_SetTargetPos(float4::ZERO);
		MainUnit_->Unit_SetMove(false);

		ChangePlayerState("Idle");
	}
}

void AIController::KeyStateUpdate()
{
	Key_RB_ = GameEngineInput::GetInst().Down("Key_RB");
	Key_LB_ = GameEngineInput::GetInst().Down("Key_LB");
	Key_Stop_ = GameEngineInput::GetInst().Down("Key_Stop");
}

void AIController::Update(float _DeltaTime)
{
	KeyStateUpdate();
	MouseUpdate();

	PlayerStateUpdate();
	CameraUpdate(_DeltaTime);
}

void AIController::CameraUpdate_EternalReturn(float _DeltaTime)
{
	if (true == GetLevel()->GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}
	else
	{
		float4 CamPos = MainUnit_->GetTransform()->GetWorldPosition();
		CamPos.y += 700.f;
		CamPos.z -= 550;

		GetLevel()->GetMainCameraActor()->GetTransform()->SetWorldPosition(CamPos);
		GetLevel()->GetMainCameraActor()->GetTransform()->SetWorldRotationDegree({ 60.f,0.f,0.f });

	}
}
