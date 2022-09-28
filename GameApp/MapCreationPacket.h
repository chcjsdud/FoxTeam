#pragma once
#include <GameEngine/GameEnginePacketBase.h>

// 분류 : 패킷
// 용도 : 
// 설명 : 맵생성을 알리는 패킷
class MapCreationPacket : public GameEnginePacketBase
{
public:
protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:

public:
	MapCreationPacket();
	~MapCreationPacket();

protected:
	MapCreationPacket(const MapCreationPacket& _other) = delete;
	MapCreationPacket(MapCreationPacket&& _other) noexcept = delete;

private:
	MapCreationPacket& operator=(const MapCreationPacket& _other) = delete;
	MapCreationPacket& operator=(const MapCreationPacket&& _other) = delete;

public:
protected:
private:
};

