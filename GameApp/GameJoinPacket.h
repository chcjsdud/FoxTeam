#pragma once
#include <GameEngine/GameEnginePacketBase.h>

// ������ Ŭ���̾�Ʈ���� �÷��̾� ��ȣ�� �ο��մϴ�.
// ȣ��Ʈ �÷��̾ 1������ �����Ǳ� ������, 2������ ���������� Ŭ���̾�Ʈ���� �ο��մϴ�.

class GameJoinPacket : public GameEnginePacketBase
{
public:
	GameJoinPacket();
	~GameJoinPacket();
	GameJoinPacket(const GameJoinPacket& _other) = delete;
	GameJoinPacket(GameJoinPacket&& _other) noexcept;
	GameJoinPacket& operator=(const GameJoinPacket& _other) = delete;
	GameJoinPacket& operator=(const GameJoinPacket&& _other) = delete;

public:
	void SetPlayerNumber(int _playerNumber);
	void SetAllPlayerList(std::vector<std::string> _vector);

	int GetPlayerNumber()
	{
		return playerNumber_;
	}

	std::vector<std::string> GetAllPlayerList()
	{
		return allPlayerList_;
	}

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(bool _bServer, GameEngineSocketInterface* _network) override;

private:
	std::vector<std::string> allPlayerList_;
	int playerNumber_;
};

