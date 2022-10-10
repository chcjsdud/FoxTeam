#include "Precompile.h"

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
