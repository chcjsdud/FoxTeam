#include "PreCompile.h"
#include "PJW_GameController.h"

#include "PJW_Hyunwoo.h"

PJW_GameController::PJW_GameController() // default constructer ����Ʈ ������
{

}

PJW_GameController::~PJW_GameController() // default destructer ����Ʈ �Ҹ���
{

}

PJW_GameController::PJW_GameController(PJW_GameController&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}


void PJW_GameController::Start()
{
	// �÷��̾��Ʈ�� �÷��̾ ä��� (�κ񿡼� ĳ���͸� ����... ���������� ���ϴ�...)
	playerList_.reserve(8);

	p1_ = new PJW_Hyunwoo();

	playerList_.push_back(p1_);

	p2_ = new PJW_Hyunwoo();

	playerList_.push_back(p2_);
	// �ϴ� ���������� ����� ���� ����� �� ���� ����

	
}

void PJW_GameController::Init_Actor()
{

}

void PJW_GameController::Update(float _DeltaTime)
{

}
