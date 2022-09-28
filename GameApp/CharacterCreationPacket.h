#pragma once
#include <GameEngine/GameEnginePacketBase.h>

// 분류 : 패킷
// 용도 : 
// 설명 : 캐릭터 생성 명령 패킷
class CharacterCreationPacket : public GameEnginePacketBase
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
	CharacterCreationPacket();
	~CharacterCreationPacket();

protected:
	CharacterCreationPacket(const CharacterCreationPacket& _other) = delete;
	CharacterCreationPacket(CharacterCreationPacket&& _other) noexcept = delete;

private:
	CharacterCreationPacket& operator=(const CharacterCreationPacket& _other) = delete;
	CharacterCreationPacket& operator=(const CharacterCreationPacket&& _other) = delete;

public:
protected:
private:
};

