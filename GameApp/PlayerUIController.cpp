#include "PreCompile.h"
#include "PlayerUIController.h"
#include "PlayerInfoManager.h"


PlayerUIController::PlayerUIController() // default constructer 디폴트 생성자
{

}

PlayerUIController::~PlayerUIController() // default destructer 디폴트 소멸자
{

}

PlayerUIController::PlayerUIController(PlayerUIController&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void PlayerUIController::InitUI()
{
	charPicture_UI = GetLevel()->CreateActor<UI_CharPicture>();
	equip_UI = GetLevel()->CreateActor<UI_Equip>();
	inventory_UI = GetLevel()->CreateActor<UI_Inventory>();
	skill_UI = GetLevel()->CreateActor<UI_Skill>();
	status_UI = GetLevel()->CreateActor<UI_Status>();
}


void PlayerUIController::SetJobType(JobType _MyJob)
{
	charPicture_UI->SetJobType(_MyJob);
	skill_UI->SetJobType(_MyJob);
}

void PlayerUIController::Start()
{
	InitUI();
}

void PlayerUIController::Update(float _DeltaTime)
{
	//스테이터스를 상시 받아야함
	status_UI->SetStatus(PlayerInfoManager::GetInstance()->GetMyPlayer().stat_);
}

