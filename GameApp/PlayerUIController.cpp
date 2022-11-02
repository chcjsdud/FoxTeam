#include "PreCompile.h"
#include "PlayerUIController.h"
#include "PlayerInfoManager.h"
#include "LumiaLevel.h"



PlayerUIController::PlayerUIController()
	: MyJob(JobType::HYUNWOO), winLoseFlag_(false), blood_UI(nullptr)
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
	FOW_UI = GetLevel()->CreateActor<UI_FOW>();
	blood_UI = GetLevel()->CreateActor<UI_BloodBackground>();
	charPicture_UI = GetLevel()->CreateActor<UI_CharPicture>();
	equip_UI = GetLevel()->CreateActor<UI_Equip>();
	inventory_UI = GetLevel()->CreateActor<UI_Inventory>();
	skill_UI = GetLevel()->CreateActor<UI_Skill>();
	status_UI = GetLevel()->CreateActor<UI_Status>();
	time_UI = GetLevel()->CreateActor<UI_Time>();
	notice_UI = GetLevel()->CreateActor<UI_Notice>();
	winLose_UI = GetLevel()->CreateActor<UI_WinLose>();
	hpbars_UI = GetLevel()->CreateActor<UI_HPBars>();
	charfollow_UI = GetLevel()->CreateActor<UI_CharFollow>();

	calhelper_ = GetLevel()->CreateActor<UI_CalculateHelper>();

	//테스트용
	notice_UI->SetText("알림 UI 테스트용 텍스트입니다", 5.f);
}


void PlayerUIController::SetJobType(JobType _MyJob)
{
	MyJob = _MyJob;

	charPicture_UI->SetJobType(MyJob);
	skill_UI->SetJobType(MyJob);
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
	skill_UI->SetStatus(pm->GetMyPlayer().stat_);
	hpbars_UI->SetStatus(pm->GetMyPlayer().stat_);

	float4 playerpos = calhelper_->Cal3Dto2D(pm->GetMyPlayer().curPos_);
}

