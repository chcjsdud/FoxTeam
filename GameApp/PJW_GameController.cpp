#include "PreCompile.h"
#include "PJW_GameController.h"

#include "PJW_Hyunwoo.h"
#include "PJW_Map.h"
#include "PJW_Mouse.h"

#include <GameEngine/GameEngineWindow.h>
#include <GameEngine/GameEngineCollision.h>
#include "PJW_Enum.h"

PJW_GameController::PJW_GameController() // default constructer 디폴트 생성자
	: curPlayer_(nullptr)
{

}

PJW_GameController::~PJW_GameController() // default destructer 디폴트 소멸자
{

}

PJW_GameController::PJW_GameController(PJW_GameController&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}


void PJW_GameController::Start()
{
	Init_Player();
	Init_Keys();
}

void PJW_GameController::Init_Player()
{
	// 플레이어리스트에 플레이어를 채운다 (로비에서 캐릭터를 고르던... 인위적으로 정하던...)
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
	//// 일단 인위적으로 디버깅 위해 현우로 두 명을 생성
	//playerList_[1]->GetTransform()->SetWorldPosition({ 300.0f, 0.0f, 300.0f });
	//
	//
	//// 현재 컨트롤 주도권을 가진 플레이어 설정
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
