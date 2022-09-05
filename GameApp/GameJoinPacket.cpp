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

    // 브로드캐스팅 받는 기존 클라이언트들에게는 신규 클라의 구좌
    
    // 신규 클라이언트에게는 서버의 구좌로 인식된다.

    if (_bServer)
    {
        _network->Send(this);
    }
}

