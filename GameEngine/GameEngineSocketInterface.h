#pragma once
#include "PlayerInfo.h"

class GameEnginePacketBase;
class GameEngineSocketInterface
{
public:
	virtual void Send(GameEnginePacketBase* _packet) = 0;
	virtual void Send(SOCKET& _receiver, GameEnginePacketBase* _packet) = 0;


	std::vector<PlayerInfo> serverPlayerList_;
	int myPlayerNumber_;		// 접속자 순번
	int myCharacter_;			// 선택 캐릭터
	int myStartPoint_;		// 시작 위치
	bool myIsReady_;
};

