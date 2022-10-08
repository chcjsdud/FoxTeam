#include "PreCompile.h"
#include "PlayerUIController.h"
#include "PlayerInfoManager.h"
#include "LumiaLevel.h"


PlayerUIController::PlayerUIController()
	: Rio_bLongBow_(false), MyJob(JobType::HYUNWOO)
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
	MyJob = _MyJob;

	charPicture_UI->SetJobType(MyJob);
	skill_UI->SetJobType(MyJob);

	if (MyJob == JobType::RIO)
	{
		Rio_bLongBow_ = false;
	}
}


void PlayerUIController::Start()
{
	InitUI();
}

void PlayerUIController::Update(float _DeltaTime)
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	// 현재 레벨이 루미아 레벨이 아닌 경우 처리하지 않음
	LumiaLevel* lumiaLevel = GetLevelConvert<LumiaLevel>();
	if (nullptr == lumiaLevel)
	{
		return;
	}

	//스테이터스를 상시 받아야함
	status_UI->SetStatus(pm->GetMyPlayer().stat_);

	pm->GetMainCharacter();

	if (MyJob == JobType::RIO)
	{
		skill_UI->SetSkillType(MyJob, Rio_bLongBow_);
	}

}

