#include "PreCompile.h"
#include "CharSelectPacket.h"

#include "ePacketID.h"
CharSelectPacket::CharSelectPacket() // default constructer 디폴트 생성자
{

}

CharSelectPacket::~CharSelectPacket() // default destructer 디폴트 소멸자
{

}

CharSelectPacket::CharSelectPacket(CharSelectPacket&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}


void CharSelectPacket::userSerialize()
{
    serializer_ << character_;
    serializer_ << startPoint_;
}

void CharSelectPacket::userDeserialize()
{
    serializer_ >> character_;
    serializer_ >> startPoint_;
}

void CharSelectPacket::initPacketID()
{
    SetPacketID(ePacketID::CharSelectPacket);
}

GameEnginePacketBase* CharSelectPacket::getUserObject()
{
    return new CharSelectPacket;
}

void CharSelectPacket::execute(bool _bServer, GameEngineSocketInterface* _network)
{


    if (_bServer)
    {
        _network->Send(this);
    }
}

void CharSelectPacket::SetCharacter(int _character)
{
    character_ = _character;
}

void CharSelectPacket::SetStartPoint(int _startPoint)
{
    startPoint_ = _startPoint;
}