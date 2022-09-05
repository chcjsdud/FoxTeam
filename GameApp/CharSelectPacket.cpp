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
    serializer_ << targetIndex_;
    serializer_ << character_;
    serializer_ << startPoint_;
}

void CharSelectPacket::userDeserialize()
{
    serializer_ >> targetIndex_;
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

void CharSelectPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{

    // ���� INT �� ��ȯ�ϰ�,
    // �̴� Ŭ�������� ENUM ������ ĳ���Ϳ� ���� ������ �ؼ��մϴ�.
    _network->serverPlayerList_[targetIndex_-1].character_ = character_;
    _network->serverPlayerList_[targetIndex_-1].startPoint_ = startPoint_;

    if (_bServer)
    {

        _network->Send(this);
    }
}

void CharSelectPacket::SetTargetIndex(int _index)
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
