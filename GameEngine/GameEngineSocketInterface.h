#pragma once

class GameEnginePacketBase;
class GameEngineSocketInterface
{
public:
	virtual void Send(GameEnginePacketBase* _packet) = 0;

	// 0830 박종원

	// 패킷을 전달받아 Execute() 로 받을 수 있는 값들입니다.
	
	std::vector<std::string> serverPlayerList_;
	
	int playerNumber_;		// 접속자 순번
	int character_;			// 선택 캐릭터
	int startPoint_;		// 시작 위치
};

