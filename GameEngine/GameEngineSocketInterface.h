#pragma once

class GameEnginePacketBase;
class GameEngineSocketInterface
{
public:
	virtual void Send(GameEnginePacketBase* _packet) = 0;

	std::vector<bool> serverPlayerList_;
	int playerNumber_;		// ������ ����
	int character_;			// ���� ĳ����
	int startPoint_;		// ���� ��ġ
	bool isReady_;
};

