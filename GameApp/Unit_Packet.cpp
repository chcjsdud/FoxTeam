#include "PreCompile.h"
#include "Unit_Packet.h"
#include "ePacketID.h"

Unit_Packet::Unit_Packet() 
    : Unit_ID_(0)
    , Target_Unit_ID_(0)
    , ControlOrder_(0)
{

}

Unit_Packet::~Unit_Packet() // default destructer 디폴트 소멸자
{

}

Unit_Packet::Unit_Packet(Unit_Packet&& _other) noexcept
    : Unit_ID_(0)
    , Target_Unit_ID_(0)
    , ControlOrder_(0)
{

}

void Unit_Packet::initPacketID()
{
    SetPacketID(ePacketID::UnitPakket);
}

void Unit_Packet::userSerialize()
{
    serializer_ << Unit_ID_;
    serializer_ << ControlOrder_;
    serializer_ << Target_Pos_ ;
    serializer_ << Target_Unit_ID_ ;
}

void Unit_Packet::userDeserialize()
{
    serializer_ >> Unit_ID_;
    serializer_ >> ControlOrder_;
    serializer_ >> Target_Pos_;
    serializer_ >> Target_Unit_ID_;
}

GameEnginePacketBase* Unit_Packet::getUserObject()
{
    return new Unit_Packet;
}

void Unit_Packet::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
    //std::cout << text_ << std::endl;

    if (_bServer)
    {
        _network->Send(this);
    }
}

