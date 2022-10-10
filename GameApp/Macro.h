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
	void SendPacket(GameEnginePacketBase& packet);
}

