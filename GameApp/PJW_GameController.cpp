#include "PreCompile.h"
#include "PJW_GameController.h"

#include "PJW_Hyunwoo.h"
#include "PJW_Map.h"
#include "PJW_Mouse.h"

#include <GameEngine/GameEngineWindow.h>
#include <GameEngine/GameEngineCollision.h>
#include "PJW_Enum.h"

PJW_GameController::PJW_GameController() // default constructer ����Ʈ ������
	: curPlayer_(nullptr)
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
	Init_Player();
	Init_Keys();
}

void PJW_GameController::Init_Player()
{
	// �÷��̾��Ʈ�� �÷��̾ ä��� (�κ񿡼� ĳ���͸� ����... ���������� ���ϴ�...)
	//playerList_.reserve(2);
	//
	//PJW_Hyunwoo* p1_ = GetLevel()->CreateActor<PJW_Hyunwoo>();
	//p1_->SetCollisionBody(InGameCollisionType::Player1_Body);
	//p1_->SetCollisionAttackRange(InGameCollisionType::Player1_AttackRange);
	//
	//playerList_.push_back(p1_);
	//
	//PJW_Hyunwoo* p2_ = GetLevel()->CreateActor<PJW_Hyunwoo>();
	//p2_->SetCollisionBody(InGameCollisionType::Player2_Body);
	//p2_->SetCollisionAttackRange(InGameCollisionType::Player2_AttackRange);
	//
	//playerList_.push_back(p2_);
	//// �ϴ� ���������� ����� ���� ����� �� ���� ����
	//playerList_[1]->GetTransform()->SetWorldPosition({ 300.0f, 0.0f, 300.0f });
	//
	//
	//// ���� ��Ʈ�� �ֵ����� ���� �÷��̾� ����
	//curPlayer_ = playerList_[0];
	//
	//
	curPlayer_ = GetLevel()->CreateActor<PJW_Hyunwoo>();
	mouse_ = GetLevel()->CreateActor<PJW_Mouse>();
	//map_ = GetLevel()->CreateActor<PJW_Map>();
}

void PJW_GameController::Init_Keys()
{
	if (false == GameEngineInput::GetInst().IsKey("Skill_Q"))
	{
		GameEngineInput::GetInst().CreateKey("FreeCameraOn", 'O');
		GameEngineInput::GetInst().CreateKey("FreeCamera_Left", 'A');
		GameEngineInput::GetInst().CreateKey("FreeCamera_Right", 'D');
		GameEngineInput::GetInst().CreateKey("FreeCamera_Forward", 'W');
		GameEngineInput::GetInst().CreateKey("FreeCamera_Back", 'S');


		GameEngineInput::GetInst().CreateKey("Q", 'Z');
		GameEngineInput::GetInst().CreateKey("W", 'X');
		GameEngineInput::GetInst().CreateKey("E", 'C');
		GameEngineInput::GetInst().CreateKey("R", 'V');
		GameEngineInput::GetInst().CreateKey("M1", VK_LBUTTON);

	}
}

void PJW_GameController::Update(float _DeltaTime)
{
	Check_MousePicking(_DeltaTime);
}

void PJW_GameController::Check_MousePicking(float _DeltaTime)
{
	float mouseX = GameEngineInput::GetInst().GetMousePos().x;
	float mouseY = GameEngineInput::GetInst().GetMousePos().y;



}
