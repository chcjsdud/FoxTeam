#include "Precompile.h"
#include "PacketSoundPlay.h"
#include <GameEngineBase/GameEngineSoundPlayer.h>

std::unique_ptr<GameEngineSoundPlayer> FT::BGMPlayer = std::make_unique<GameEngineSoundPlayer>();

void FT::SendPacket(GameEnginePacketBase& packet)
{
	if (true == GameServer::GetInstance()->IsOpened())
	{
		GameServer::GetInstance()->Send(&packet);
	}
	else if (true == GameClient::GetInstance()->IsConnected())
	{
		GameClient::GetInstance()->Send(&packet);
	}
}

void FT::PlaySoundAndSendPacket(const std::string& _name, const float4& _position)
{
	GameEngineSoundManager::GetInstance()->PlaySoundByName(_name);
	PacketSoundPlay packet;
	packet.SetSound(_name, _position);
	FT::SendPacket(packet);
}
