#include "PreCompile.h"
#include "GameJoinPacket.h"
//#include "PlayersInfo.h"
#include "ePacketID.h"


// 보류

// 사실상 GameEngineServer 에서 서버 개설 / 클라이언트 Accept 할 때 번호를 매기는 게 맞다고 생각한다.

GameJoinPacket::GameJoinPacket() // default constructer 디폴트 생성자
    : playerNumber_(-1)
{
}

GameJoinPacket::~GameJoinPacket() // default destructer 디폴트 소멸자
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
    // 해당 패킷이 해 줘야 할 일을 만들어야 한다.



    if (_bServer)
    {
        // 서버가 이미 클라 번호까지 Accept 시 다 가지고 있다...
        // 다만 이걸 어캐 알려줘야 하나?
        // 니가 사용할 인덱스는 몇이야
       // LobbyPlayerInfo info;
       // info.SetStartPoint(serverSocket_.GetClientSocketSize() + 1);
       // playerList_.emplace_back();
        // 이게 만약 서버에서 받아 준 processpacket() 이면 무엇을 해 주어야 하나?
        // gamejoinpacket 이니... 클라이언트 입장 시 순번을 알려 주고
        // 다른 클라이언트들에게 브로드캐스팅도 해 줘야지..

        _network->Send(this);
    }
}

