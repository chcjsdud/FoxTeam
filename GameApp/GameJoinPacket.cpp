#include "PreCompile.h"
#include "GameJoinPacket.h"
#include "ePacketID.h"
#include "LobbyLevel.h"

// ��ǻ� GameEngineServer ���� ���� ���� / Ŭ���̾�Ʈ Accept �� �� ��ȣ�� �ű�� �� �´ٰ� �����Ѵ�.

GameJoinPacket::GameJoinPacket() // default constructer ����Ʈ ������
{
}

GameJoinPacket::~GameJoinPacket() // default destructer ����Ʈ �Ҹ���
{

}

void GameJoinPacket::SetAllPlayerList(std::vector<bool> _vector)
{
   allPlayerList_ = _vector;
}



void GameJoinPacket::userSerialize()
{
    serializer_.WriteVector(allPlayerList_);
}

void GameJoinPacket::userDeserialize()
{
    serializer_.ReadVector(allPlayerList_);
}

void GameJoinPacket::initPacketID()
{
    SetPacketID(ePacketID::GameJoinPacket);
}

GameEnginePacketBase* GameJoinPacket::getUserObject()
{
    return new GameJoinPacket;
}

void GameJoinPacket::execute(bool _bServer, GameEngineSocketInterface* _network)
{
    _network->serverPlayerList_ = allPlayerList_;

    if (_bServer)
    {
        _network->Send(this);
    }
}

