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
	StartPointSelectPacket,
	ReadyPacket,
	LoadingLumiaPacket,
	LobbyToLumiaPacket,
	StartPacket,

	// LoadingLevel
	MapCreatPacket,
	MonsterCreatePacket,
	CharacterCreatePacket,
	LoadEndPacket,
	LvChangePacket,

	// LumiaLevel
	CharMovementPacket,
	CharAnimationPacket,
	CharStatPacket,

	MAX
};