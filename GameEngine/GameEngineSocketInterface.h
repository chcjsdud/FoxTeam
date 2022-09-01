#pragma once

class GameEnginePacketBase;
class GameEngineSocketInterface
{
public:
	virtual void Send(GameEnginePacketBase* _packet) = 0;

	std::vector<int> serverPlayerList_;
	int playerNumber_;		// 접속자 순번
	int character_;			// 선택 캐릭터
	int startPoint_;		// 시작 위치
	bool isReady_;
};

