#include "PreCompile.h"
#include "GameJoinPacket.h"
#include "ePacketID.h"
#include "LobbyLevel.h"

// ��ǻ� GameEngineServer ���� ���� ���� / Ŭ���̾�Ʈ Accept �� �� ��ȣ�� �ű�� �� �´ٰ� �����Ѵ�.

GameJoinPacket::GameJoinPacket() // default constructer ����Ʈ ������
    : playerNumber_(-1)
{
}

GameJoinPacket::~GameJoinPacket() // default destructer ����Ʈ �Ҹ���
{

}

void GameJoinPacket::SetPlayerNumber(int _playerNumber)
{
    playerNumber_ = _playerNumber;
}

void GameJoinPacket::SetAllPlayerList(std::vector<std::string> _vector)
{
   allPlayerList_ = _vector;
}



void GameJoinPacket::userSerialize()
{
    serializer_ << playerNumber_;
    serializer_.WriteVector(allPlayerList_);
}

void GameJoinPacket::userDeserialize()
{
    serializer_ >> playerNumber_;
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
    // �ش� ��Ŷ�� �� ��� �� ���� ������ �Ѵ�.

    // join ��Ŷ�� �������Լ� ������ �� ������ �������༭ �������־�� �Ѵ�.

    _network->serverPlayerList_ = allPlayerList_;
    _network->playerNumber_ = static_cast<int>(allPlayerList_.size());


    if (_bServer)
    {
        // ������ �̹� Ŭ�� ��ȣ���� Accept �� �� ������ �ִ�...
        // �ٸ� �̰� ��ĳ �˷���� �ϳ�?
        // �ϰ� ����� �ε����� ���̾�
       // LobbyPlayerInfo info;
       // info.SetStartPoint(serverSocket_.GetClientSocketSize() + 1);
       // playerList_.emplace_back();
        // �̰� ���� �������� �޾� �� processpacket() �̸� ������ �� �־�� �ϳ�?
        // gamejoinpacket �̴�... Ŭ���̾�Ʈ ���� �� ������ �˷� �ְ�
        // �ٸ� Ŭ���̾�Ʈ�鿡�� ��ε�ĳ���õ� �� �����..

        _network->Send(this);
    }
}

