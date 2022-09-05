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
	void SetAllPlayerList(std::vector<int> _vector);
	
	void SetAllPlayerCount(int _size);


	std::vector<int> GetAllPlayerList()
	{
		return allPlayerList_;
	}

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
	std::vector<int> allPlayerList_;
	int allPlayerCount_;
};

