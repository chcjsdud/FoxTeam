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

void GameJoinPacket::SetAllPlayerList(std::vector<int> _vector)
{
   allPlayerList_ = _vector;
}

void GameJoinPacket::SetAllPlayerCount(int _size)
{
    allPlayerCount_ = _size;
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

void GameJoinPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
    PlayerInfo info;
    info.character_ = -1;
    info.startPoint_ = -1;
    info.isReady_ = false;
    _network->serverPlayerList_.push_back(info);

    // ��ε�ĳ���� �޴� ���� Ŭ���̾�Ʈ�鿡�Դ� �ű� Ŭ���� ����
    
    // �ű� Ŭ���̾�Ʈ���Դ� ������ ���·� �νĵȴ�.

    if (_bServer)
    {
        _network->Send(this);
    }
}

