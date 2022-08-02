#include "PreCompile.h"
#include "PlayController.h"
#include "GameMouse.h"
#include "LH_Player.h"

PlayController::PlayController() // default constructer 디폴트 생성자
{

}

PlayController::~PlayController() // default destructer 디폴트 소멸자
{

}

PlayController::PlayController(PlayController&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void PlayController::InitInput()
{
	if (false == GameEngineInput::GetInst().IsKey("M1"))
	{
		GameEngineInput::GetInst().CreateKey("M1", VK_LBUTTON); // 마우스 왼쪽 키
		GameEngineInput::GetInst().CreateKey("M2", VK_RBUTTON); // 마우스 오른쪽 키
	}

	GameEngineInput::GetInst().CreateKey("W", 'W');
	GameEngineInput::GetInst().CreateKey("A", 'A');
	GameEngineInput::GetInst().CreateKey("D", 'D');
	GameEngineInput::GetInst().CreateKey("S", 'S');
	GameEngineInput::GetInst().CreateKey("Space", VK_SPACE);
	GameEngineInput::GetInst().CreateKey("MoveUp", 'Q');
	GameEngineInput::GetInst().CreateKey("MoveDown", 'E');
	GameEngineInput::GetInst().CreateKey("Attack", 'J');
	GameEngineInput::GetInst().CreateKey("Esc", VK_ESCAPE);
	GameEngineInput::GetInst().CreateKey("RockOn", VK_LSHIFT);
}

void PlayController::InitActor()
{
	//testGameMouse_ = GetLevel()->CreateActor<GameMouse>();
	//testPlayer_ = GetLevel()->CreateActor<Player>();

}


void PlayController::Start()
{
	InitInput();
	InitActor();

	{
		CameraState_.CreateState<PlayController>("Up", this, nullptr, &PlayController::CameraUpdate_UpPosition, nullptr);
		CameraState_.CreateState<PlayController>("Back", this, nullptr, &PlayController::CameraUpdate_BackPosition, nullptr);
		CameraState_.CreateState<PlayController>("EternalReturn", this, nullptr, &PlayController::CameraUpdate_EternalReturn, nullptr);
		CameraState_.ChangeState("EternalReturn");
	}
}

void PlayController::Update(float _DeltaTime)
{
	//CheckInput(_DeltaTime);
}

void PlayController::CameraUpdate_BackPosition(float _DeltaTime)
{
	if (true == GetLevel()->GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}
	else
	{
		float4 rot = GetLevel()->GetMainCameraActor()->GetTransform()->GetLocalRotation();

		float4 CamPos = MainActor_->GetTransform()->GetWorldPosition();
		CamPos.y += 200.f;
		CamPos.x -= 100 * sinf(rot.y * GameEngineMath::DegreeToRadian);
		CamPos.z -= 100 * cosf(rot.y * GameEngineMath::DegreeToRadian);

		GetLevel()->GetMainCameraActor()->GetTransform()->SetWorldPosition(CamPos);
	}
}

void PlayController::CameraUpdate_UpPosition(float _DeltaTime)
{
	if (true == GetLevel()->GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}
	else
	{
		//float4 rot = GetLevel()->GetMainCameraActor()->GetTransform()->GetLocalRotation();

		float4 CamPos = MainActor_->GetTransform()->GetWorldPosition();
		CamPos.y += 400.f;
		CamPos.z -= 400;

		GetLevel()->GetMainCameraActor()->GetTransform()->SetWorldPosition(CamPos);
		GetLevel()->GetMainCameraActor()->GetTransform()->SetWorldRotationDegree({ 25.f,0.f,0.f });
	}
}

void PlayController::CameraUpdate_EternalReturn(float _DeltaTime)
{
	if (true == GetLevel()->GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}
	else
	{
		float4 CamPos = MainActor_->GetTransform()->GetWorldPosition();
		CamPos.y += 700.f;
		CamPos.z -= 550;

		GetLevel()->GetMainCameraActor()->GetTransform()->SetWorldPosition(CamPos);
		GetLevel()->GetMainCameraActor()->GetTransform()->SetWorldRotationDegree({ 60.f,0.f,0.f });

	}
}