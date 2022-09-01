#include "PreCompile.h"
#include "GameJoinPacket.h"
#include "ePacketID.h"
#include "LobbyLevel.h"

// 사실상 GameEngineServer 에서 서버 개설 / 클라이언트 Accept 할 때 번호를 매기는 게 맞다고 생각한다.

GameJoinPacket::GameJoinPacket() // default constructer 디폴트 생성자
{
}

GameJoinPacket::~GameJoinPacket() // default destructer 디폴트 소멸자
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

