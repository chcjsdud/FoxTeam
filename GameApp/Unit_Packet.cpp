#include "PreCompile.h"
#include "Unit_Packet.h"

Unit_Packet::Unit_Packet() 
    : Unit_ID_(0)
    , Target_Unit_ID_(0)
    , ControlOrder_(0)
{

}

Unit_Packet::~Unit_Packet() // default destructer 디폴트 소멸자
{

}

Unit_Packet::Unit_Packet(Unit_Packet&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void Unit_Packet::initPacketID()
{
}

void Unit_Packet::userSerialize()
{
}

void Unit_Packet::userDeserialize()
{
}

GameEnginePacketBase* Unit_Packet::getUserObject()
{
    return nullptr;
}

void Unit_Packet::execute(bool _bServer, GameEngineSocketInterface* _network)
{
}

