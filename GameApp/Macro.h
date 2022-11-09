#pragma once

#include "GameServer.h"
#include "GameClient.h"

#define MakeState(ClassName, StateName) #StateName, std::bind(&ClassName::update##StateName, this, std::placeholders::_1) \
		, std::bind(&ClassName::start##StateName, this)

#define MakeStateWithEnd(ClassName, StateName) #StateName, std::bind(&ClassName::update##StateName, this, std::placeholders::_1) \
		, std::bind(&ClassName::start##StateName, this) \
		, std::bind(&ClassName::end##StateName, this)


namespace FT
{
	extern std::unique_ptr<GameEngineSoundPlayer> BGMPlayer;
	void SendPacket(GameEnginePacketBase& packet);
	void PlaySoundAndSendPacket(const std::string& _name, const float4& _position);

	void AddText(const std::string& _text);
}

