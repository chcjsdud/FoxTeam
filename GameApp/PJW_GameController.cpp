#include "PreCompile.h"
#include "PJW_GameController.h"

#include "PJW_Hyunwoo.h"
#include "PJW_Map.h"

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
	map_ = GetLevel()->CreateActor<PJW_Map>();
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
		GameEngineInput::GetInst().CreateKey("Skill_Q", 'Q');

		
	}
}

void PJW_GameController::Update(float _DeltaTime)
{
	Check_Input(_DeltaTime);
}

void PJW_GameController::Check_Input(float _DeltaTime)
{
	//if (true == GameEngineInput::GetInst().Down("Test_Move"))
	//{
	//	curPlayer_->SetTarget(playerList_[1]);
	//	curPlayer_->isAttacking_ = false;
	//	curPlayer_->isMoving_ = true;
	//	// ��Ʈ�ѷ��� ��ǲ�� �ν��� ���� �÷��̾� ĳ������ bool �������� ����Ī�Ѵ�.
	//	// ������ ���� false �� ������ ���� �÷��̾� ĳ���Ͱ� �Ѵ�.
	//
	//
	//
	//}
	//
	//if (true == GameEngineInput::GetInst().Down("Skill_Q"))
	//{
	//	//curPlayer_->isMoving_ = false;
	//	//curPlayer_->isAttacking_ = true;
	//	
	//}
}
