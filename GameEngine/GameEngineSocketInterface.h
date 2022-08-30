#pragma once

class GameEnginePacketBase;
class GameEngineSocketInterface
{
public:
	virtual void Send(GameEnginePacketBase* _packet) = 0;

	// 0830 ╧за╬©Ь
	std::vector<std::string> serverPlayerList_;
	int playerNumber_;

};

