#include "PreCompile.h"
#include "GameJoinPacket.h"
//#include "PlayersInfo.h"
#include "ePacketID.h"


// ����

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



void GameJoinPacket::userSerialize()
{
    serializer_ << playerNumber_;
}

void GameJoinPacket::userDeserialize()
{
    serializer_ >> playerNumber_;
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

