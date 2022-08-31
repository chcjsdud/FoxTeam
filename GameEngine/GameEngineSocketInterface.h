#pragma once

class GameEnginePacketBase;
class GameEngineSocketInterface
{
public:
	virtual void Send(GameEnginePacketBase* _packet) = 0;

	// 0830 ������

	// ��Ŷ�� ���޹޾� Execute() �� ���� �� �ִ� �����Դϴ�.
	
	std::vector<std::string> serverPlayerList_;
	
	int playerNumber_;		// ������ ����
	int character_;			// ���� ĳ����
	int startPoint_;		// ���� ��ġ
};

