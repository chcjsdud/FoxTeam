#include "PreCompile.h"
#include "LumiaUIController.h"

#include "UI_CharPicture.h"
#include "UI_Equip.h"
#include "UI_Inventory.h"
#include "UI_Skill.h"
#include "UI_Status.h"

LumiaUIController::LumiaUIController() // default constructer 디폴트 생성자
{

}

LumiaUIController::~LumiaUIController() // default destructer 디폴트 소멸자
{

}

LumiaUIController::LumiaUIController(LumiaUIController&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void LumiaUIController::InitUI()
{
	charPicture_UI = GetLevel()->CreateActor<UI_CharPicture>();
	equip_UI = GetLevel()->CreateActor<UI_Equip>();
	inventory_UI = GetLevel()->CreateActor<UI_Inventory>();
	skill_UI = GetLevel()->CreateActor<UI_Skill>();
	status_UI = GetLevel()->CreateActor<UI_Status>();
}


void LumiaUIController::Start()
{
	InitUI();
}

void LumiaUIController::Update(float _DeltaTime)
{

}

