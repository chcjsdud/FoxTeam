#include "PreCompile.h"
#include "Controller.h"

Controller::Controller() 
	: Controller_Target_Unit_(nullptr)
	, Controller_Order_(Controller_Order::None_Idle)
{

}

Controller::~Controller() // default destructer ����Ʈ �Ҹ���
{

}

Controller::Controller(Controller&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}

