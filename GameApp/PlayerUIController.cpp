#include "PreCompile.h"
#include "PlayerUIController.h"
#include "PlayerInfoManager.h"
#include "LumiaLevel.h"



PlayerUIController::PlayerUIController()
	: MyJob(JobType::HYUNWOO), winLoseFlag_(false), blood_UI(nullptr)
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

	//�׽�Ʈ��
	notice_UI->SetText("�˸� UI �׽�Ʈ�� �ؽ�Ʈ�Դϴ�", 5.f);
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

	// ���� ������ ��̾� ������ �ƴ� ��� ó������ ����
	LumiaLevel* lumiaLevel = GetLevelConvert<LumiaLevel>();
	if (nullptr == lumiaLevel)
	{
		return;
	}

	if (pm == nullptr)
	{
		return;
	}

	//�������ͽ��� ��� �޾ƾ���
	status_UI->SetStatus(pm->GetMyPlayer().stat_);
	skill_UI->SetStatus(pm->GetMyPlayer().stat_);
	hpbars_UI->SetStatus(pm->GetMyPlayer().stat_);
	float4 pos = pm->GetMyPlayer().curPos_;
	float4 playerpos = calhelper_->Cal3Dto2D(pm->GetMyPlayer().curPos_);
	charfollow_UI->SetFollowInfo(playerpos, pm->GetMyPlayer().stat_);
}

