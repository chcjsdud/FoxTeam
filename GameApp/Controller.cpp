#include "PreCompile.h"
#include "Controller.h"

Controller::Controller() 
	: Controller_Target_Unit_(nullptr)
	, Controller_Order_(Controller_Order::None_Idle)
{

}

Controller::~Controller() // default destructer 디폴트 소멸자
{

}

Controller::Controller(Controller&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

