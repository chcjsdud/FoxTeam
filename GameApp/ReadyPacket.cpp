#include "PreCompile.h"
#include "ReadyPacket.h"
#include "ePacketID.h"
ReadyPacket::ReadyPacket() // default constructer ����Ʈ ������
{

}

ReadyPacket::~ReadyPacket() // default destructer ����Ʈ �Ҹ���
{

}

ReadyPacket::ReadyPacket(ReadyPacket&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}


void ReadyPacket::userSerialize()
{
    serializer_ << targetIndex_;
    serializer_ << isReady_;
}

void ReadyPacket::userDeserialize()
{
    serializer_ >> targetIndex_;
    serializer_ >> isReady_;
}

void ReadyPacket::initPacketID()
{
    SetPacketID(ePacketID::ReadyPacket);
}

GameEnginePacketBase* ReadyPacket::getUserObject()
{
    return new ReadyPacket;
}

void ReadyPacket::SetTargetIndex(int _index)
{
    targetIndex_ = _index;
}

void ReadyPacket::SetReadyStatus(int _isReady)
{
    isReady_ = _isReady;
}

void ReadyPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
    _network->serverPlayerList_[targetIndex_-1].isReady_ = static_cast<bool>(isReady_);

    if (_bServer)
    {
        GameEngineDebug::OutPutDebugString("�÷��̾� " + std::to_string(targetIndex_) + " �� ���� �ھҽ��ϴ�.");
        _network->Send(this);
    }
}