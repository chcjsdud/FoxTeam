#include "PreCompile.h"
#include "PlayerUIController.h"
#include "PlayerInfoManager.h"
#include "LumiaLevel.h"


PlayerUIController::PlayerUIController()
	: MyJob(JobType::HYUNWOO)
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
	time_UI = GetLevel()->CreateActor<UI_Time>();
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

	// ���� ������ ��̾� ������ �ƴ� ��� ó������ ����
	LumiaLevel* lumiaLevel = GetLevelConvert<LumiaLevel>();
	if (nullptr == lumiaLevel)
	{
		return;
	}

	//�������ͽ��� ��� �޾ƾ���
	status_UI->SetStatus(pm->GetMyPlayer().stat_);

}

