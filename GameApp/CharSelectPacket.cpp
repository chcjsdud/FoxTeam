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

void CharSelectPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
    _network->serverPlayerList_[targetIndex_].character_ = character_;
    _network->serverPlayerList_[targetIndex_].startPoint_ = startPoint_;
    _network->serverPlayerList_[targetIndex_].isReady_ = isReady_;
    // ���� INT �� ��ȯ�ϰ�,
    // �̴� Ŭ�������� ENUM ������ ĳ���Ϳ� ���� ������ �ؼ��մϴ�.
 

    if (_bServer)
    {

        _network->Send(this);
    }
}

void CharSelectPacket::SetTargetPlayer(int _index)
{
    targetIndex_ = _index;
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