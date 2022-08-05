#include "PreCompile.h"
#include "AIController.h"

#include "GameMouse.h"

#include "UI_Skill.h"
#include "LH_LockOnUI.h"

#include "LH_Player.h"

AIController::AIController() // default constructer ����Ʈ ������
{

}

AIController::~AIController() // default destructer ����Ʈ �Ҹ���
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
		if (IsAttack_ = true) // ���콺�� �浹 ó���� �ذ����ٰ�
		{
			MainUnit_->Unit_SetTarget(TargetActor_);


		}
		//�켱 ���콺�� ���� ��ǥ�� �޾ƿ��°� ����
		MainUnit_->Unit_SetTargetDir(TmepMouseDir_);
		MainUnit_->Unit_SetTargetPos(TempMousePos_);
		MainUnit_->Unit_SetMove(true);

		//Ÿ�ٰ��� �Ÿ��� �ſ� ª���� ���� ��ų��

		//�̹� �����̴� ���ε� �ٽ� state�� ���Ž�Ű�� �ʱ� ����
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
