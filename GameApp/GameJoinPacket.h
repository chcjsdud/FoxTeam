#pragma once
#include <GameEngine/GameEnginePacketBase.h>

// 접속한 클라이언트에게 플레이어 번호를 부여합니다.
// 호스트 플레이어가 1번으로 고정되기 때문에, 2번부터 순차적으로 클라이언트에게 부여합니다.

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


	std::vector<int> GetAllPlayerList()
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
	std::vector<int> allPlayerList_;
};

