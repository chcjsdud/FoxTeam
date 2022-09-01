#pragma once
#include <GameEngine/GameEnginePacketBase.h>

// 분류 : 
// 용도 : 
// 설명 : 
class PlayerNumberPacket : public GameEnginePacketBase
{
public:
	PlayerNumberPacket(); // default constructer 디폴트 생성자
	~PlayerNumberPacket(); // default destructer 디폴트 소멸자
	PlayerNumberPacket(const PlayerNumberPacket& _other) = delete; // default Copy constructer 디폴트 복사생성자
	PlayerNumberPacket(PlayerNumberPacket&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	PlayerNumberPacket& operator=(const PlayerNumberPacket& _other) = delete; // default Copy operator 디폴트 대입 연산자
	PlayerNumberPacket& operator=(const PlayerNumberPacket&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void SetPlayerNumber(int _playerNumber);

	int GetPlayerNumber()
	{
		return playerNumber_;
	}

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(bool _bServer, GameEngineSocketInterface* _network) override;

private:
	int playerNumber_;
};

