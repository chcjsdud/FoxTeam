#include "PreCompile.h"
#include "PlayController.h"
#include "GameMouse.h"
#include "Player.h"
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
}

void PlayController::InitActor()
{
	testGameMouse_ = GetLevel()->CreateActor<GameMouse>();
	testPlayer_ = GetLevel()->CreateActor<Player>();

}


void PlayController::Start()
{
	InitInput();
	InitActor();
}

void PlayController::Update(float _DeltaTime)
{
	CheckInput(_DeltaTime);
}

void PlayController::CheckInput(float _DeltaTime)
{
	if (true == GameEngineInput::GetInst().Down("M2"))
	{
		testGameMouse_->SaveCurPos(); // 현재 마우스 좌표 저장
		testPlayer_->SetTargetPos(testGameMouse_->GetSavedPos()); // 그 좌표를 플레이어에게 하달

		// savedPos 는 "클릭하는 부분" 의 "스크린 상의 XZ 좌표" 여야 한다...
		int a = 0;
	}
}