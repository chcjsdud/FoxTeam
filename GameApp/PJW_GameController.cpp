#include "PreCompile.h"
#include "PJW_GameController.h"

#include "PJW_Hyunwoo.h"

PJW_GameController::PJW_GameController() // default constructer 디폴트 생성자
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
	// 플레이어리스트에 플레이어를 채운다 (로비에서 캐릭터를 고르던... 인위적으로 정하던...)
	playerList_.reserve(8);

	p1_ = new PJW_Hyunwoo();

	playerList_.push_back(p1_);

	p2_ = new PJW_Hyunwoo();

	playerList_.push_back(p2_);
	// 일단 인위적으로 디버깅 위해 현우로 두 명을 생성

	
}

void PJW_GameController::Init_Actor()
{

}

void PJW_GameController::Update(float _DeltaTime)
{

}
