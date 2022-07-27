#include "PreCompile.h"
#include "PlayController.h"
#include "GameMouse.h"
#include "LH_Player.h"
PlayController::PlayController() // default constructer ����Ʈ ������
{

}

PlayController::~PlayController() // default destructer ����Ʈ �Ҹ���
{

}

PlayController::PlayController(PlayController&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}

void PlayController::InitInput()
{
	if (false == GameEngineInput::GetInst().IsKey("M1"))
	{
		GameEngineInput::GetInst().CreateKey("M1", VK_LBUTTON); // ���콺 ���� Ű
		GameEngineInput::GetInst().CreateKey("M2", VK_RBUTTON); // ���콺 ������ Ű
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
		testGameMouse_->SaveCurPos(); // ���� ���콺 ��ǥ ����
		testPlayer_->SetTargetPos(testGameMouse_->GetSavedPos()); // �� ��ǥ�� �÷��̾�� �ϴ�

		// savedPos �� "Ŭ���ϴ� �κ�" �� "��ũ�� ���� XZ ��ǥ" ���� �Ѵ�...

		// ��� ������ �ϳ�?
		// ���� Ŭ���� �Ǹ�, ���콺 ��ġ���� Ray �� ������ �Ѵ�.
		// testGameMouse_->Ratshot()
		// ������ ����µ� ������ ��ǥ���� �浹ü�� ����Ǿ�� �� ��...


		int a = 0;
	}
}