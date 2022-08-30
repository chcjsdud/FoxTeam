#pragma once

class GameEnginePacketBase;
class GameEngineSocketInterface
{
public:
	virtual void Send(GameEnginePacketBase* _packet) = 0;

	// 0830 ������
	std::vector<std::string> serverPlayerList_;
	int playerNumber_;

};

