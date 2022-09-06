#pragma once

enum class ePacketID
{
	None = -1,
	Chat,
	UnitPakket,

	// LobbyLevel
	PlayerNumberPacket,
	GameJoinPacket,
	GameJoinPacket2,
	SetPlayerNumberPacket,
	CharSelectPacket,
	ReadyPacket,
	

	MAX
};