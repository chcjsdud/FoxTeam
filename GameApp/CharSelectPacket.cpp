#include "PreCompile.h"
#include "CharSelectPacket.h"

#include "ePacketID.h"
CharSelectPacket::CharSelectPacket() // default constructer ����Ʈ ������
{

}

CharSelectPacket::~CharSelectPacket() // default destructer ����Ʈ �Ҹ���
{

}

CharSelectPacket::CharSelectPacket(CharSelectPacket&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}


void CharSelectPacket::userSerialize()
{
    serializer_ << character_;
    serializer_ << startPoint_;
    serializer_ << isReady_;
}

void CharSelectPacket::userDeserialize()
{
    serializer_ >> character_;
    serializer_ >> startPoint_;
    serializer_ >> isReady_;
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
    _network->character_ = character_;
    _network->startPoint_ = startPoint_;
    _network->serverPlayerList_[_network->playerNumber_] = isReady_;
    // ���� INT �� ��ȯ�ϰ�,
    // �̴� Ŭ�������� ENUM ������ ĳ���Ϳ� ���� ������ �ؼ��մϴ�.
 

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

void CharSelectPacket::SetIsReady(bool _isReady)
{
    isReady_ = _isReady;
}