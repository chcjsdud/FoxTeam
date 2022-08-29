#include "PreCompile.h"
#include "LobbyPlayerInfo.h"

LobbyPlayerInfo::LobbyPlayerInfo() // default constructer 디폴트 생성자
    : playerNumber_(-1),character_(-1),startPoint_(-1)
{

}

LobbyPlayerInfo::~LobbyPlayerInfo() // default destructer 디폴트 소멸자
{

}


void LobbyPlayerInfo::SetPlayerNumber(int _playerNumber)
{
    playerNumber_ = _playerNumber;
}


void LobbyPlayerInfo::SetCharacter(int _character)
{
    character_ = _character;
}

void LobbyPlayerInfo::SetStartPoint(int _startPoint)
{
    startPoint_ = _startPoint;
}