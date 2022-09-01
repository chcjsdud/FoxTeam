#include "PreCompile.h"
#include "PlayerNumberPacket.h"
#include "ePacketID.h"

PlayerNumberPacket::PlayerNumberPacket() // default constructer ����Ʈ ������
    : playerNumber_(-1)
{

}

PlayerNumberPacket::~PlayerNumberPacket() // default destructer ����Ʈ �Ҹ���
{

}

void PlayerNumberPacket::SetPlayerNumber(int _playerNumber)
{
    playerNumber_ = _playerNumber;
}


void PlayerNumberPacket::userSerialize()
{
    serializer_ << playerNumber_;
}

void PlayerNumberPacket::userDeserialize()
{
    serializer_ >> playerNumber_;
}

void PlayerNumberPacket::initPacketID()
{
    SetPacketID(ePacketID::PlayerNumberPacket);
}

GameEnginePacketBase* PlayerNumberPacket::getUserObject()
{
    return new PlayerNumberPacket;
}

void PlayerNumberPacket::execute(bool _bServer, GameEngineSocketInterface* _network)
{

    _network->playerNumber_ = playerNumber_;

    if (_bServer)
    {
        _network->Send(this);
    }
}

