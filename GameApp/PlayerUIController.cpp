#include "PreCompile.h"
#include "PlayerUIController.h"
#include "PlayerInfoManager.h"
#include "LumiaLevel.h"



PlayerUIController::PlayerUIController()
	: MyJob(JobType::HYUNWOO), winLoseFlag_(false), blood_UI(nullptr), followcreateflag(false)
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
	//charfollow_UI = GetLevel()->CreateActor<UI_CharFollow>();
	minimap_UI = GetLevel()->CreateActor<UI_Minimap>();

	calhelper_ = GetLevel()->CreateActor<UI_CalculateHelper>();

	//테스트용
	//notice_UI->SetText("알림 UI 테스트용 텍스트입니다", 5.f);
}


void PlayerUIController::SetJobType(JobType _MyJob)
{
	MyJob = _MyJob;

	charPicture_UI->SetJobType(MyJob);
	skill_UI->SetJobType(MyJob);
}

void PlayerUIController::UIOff()
{
	hpbars_UI->Off();
	charPicture_UI->Off();
	equip_UI ->Off();
	inventory_UI->Off();
	skill_UI->Off();
	status_UI->Off();
	time_UI->Off();
	notice_UI->Off();
	hpbars_UI->Off();
	charfollow_UI->Off();
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

	if (pm == nullptr)
	{
		return;
	}

	//스테이터스를 상시 받아야함
	status_UI->SetStatus(pm->GetMyPlayer().stat_);
	skill_UI->SetStatus(pm->GetMyPlayer().stat_);
	hpbars_UI->SetStatus(pm->GetMyPlayer().stat_);
	//float4 pos = lumiaLevel->GetCharacterActorList()[pm->GetMyNumber()]->GetTransform()->GetLocalPosition();
	//charfollow_UI->SetFollowInfo(calhelper_->Cal3Dto2D(pos), pm->GetMyPlayer().stat_);


	if (false == followcreateflag)
	{
		//딱 한번만 실행되게 설정
		for (size_t i = 0; i < lumiaLevel->GetCharacterActorList().size(); i++)
		{
			UI_CharFollow* follow_UI = GetLevel()->CreateActor<UI_CharFollow>();
			//float4 pos = lumiaLevel->GetCharacterActorList()[pm->GetMyNumber()]->GetTransform()->GetLocalPosition();
			//follow_UI->SetFollowInfo(calhelper_->Cal3Dto2D(pos), pm->GetPlayerList()[i].stat_);
			//lumiaLevel->GetCharacterActorList()[i]->GetTransform();
			charfollows_.push_back(follow_UI);
		}
		followcreateflag = true;
	}

	for (size_t i = 0; i < lumiaLevel->GetCharacterActorList().size(); i++)
	{
		float4 pos = lumiaLevel->GetCharacterActorList()[i]->GetTransform()->GetLocalPosition();
		charfollows_[i]->SetFollowInfo(calhelper_->Cal3Dto2D(pos), pm->GetPlayerList()[i].stat_);
		//charfollow_UI->SetFollowInfo(calhelper_->Cal3Dto2D(pos), pm->GetMyPlayer().stat_);
		//lumiaLevel->GetCharacterActorList()[i]->GetTransform();
	}


	//if (true == GameEngineInput::GetInst().Down("L"))
	//{
	//	notice_UI->SetText("알림 UI 테스트용 텍스트입니다", 2.f);
	//}
	
}

