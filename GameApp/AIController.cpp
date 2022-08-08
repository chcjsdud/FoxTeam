#include "PreCompile.h"
#include "AIController.h"

#include "GameMouse.h"

#include "UI_Skill.h"
#include "LH_LockOnUI.h"

#include "LH_Unit.h"

AIController::AIController() // default constructer 디폴트 생성자
{
}

AIController::~AIController() // default destructer 디폴트 소멸자
{

}

void AIController::Start()
{
}

void AIController::Update(float _DeltaTime)
{
}

void AIController::ChangePlayerState(std::string _State)
{
	
	if (dynamic_cast<Unit*>(GetActor())->Unit_GetStateName() != _State)
	{
		dynamic_cast<Unit*>(GetActor())->Unit_ChangeState(_State);
	}
}

void AIController::AIController_Off()
{

}

void AIController::AIController_On()
{

}