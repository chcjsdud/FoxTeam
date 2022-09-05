#pragma once
#include "PlayerInfo.h"

class GameEnginePacketBase;
class GameEngineSocketInterface
{
public:
	virtual void Send(GameEnginePacketBase* _packet) = 0;
	virtual void Send(SOCKET& _receiver, GameEnginePacketBase* _packet) = 0;


	std::vector<PlayerInfo> serverPlayerList_;
	int myPlayerNumber_;		// ������ ����
	int myCharacter_;			// ���� ĳ����
	int myStartPoint_;		// ���� ��ġ
	bool myIsReady_;
};

