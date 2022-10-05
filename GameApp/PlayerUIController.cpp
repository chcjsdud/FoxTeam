#include "PreCompile.h"
#include "PlayerUIController.h"


PlayerUIController::PlayerUIController() // default constructer ����Ʈ ������
{

}

PlayerUIController::~PlayerUIController() // default destructer ����Ʈ �Ҹ���
{

}

PlayerUIController::PlayerUIController(PlayerUIController&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
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
}

void PlayerUIController::Start()
{
	InitUI();
}

void PlayerUIController::Update(float _DeltaTime)
{
	
}

