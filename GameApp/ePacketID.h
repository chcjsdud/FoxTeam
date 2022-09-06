#pragma once

enum class ePacketID
{
	None = 0,
	Chat = 1,
	UnitPakket = 2,

	// LobbyLevel
	PlayerNumberPacket = 3,
	GameJoinPacket = 4,
	CharSelectPacket = 5,
	ReadyPacket = 6,
	//
	
	MAX = 999
};