#include "PreCompile.h"
#include "ServerTestLevel.h"
#include "ePacketID.h"
#include <GameEngine/GameEngineSocketServer.h>
#include <GameEngine/GameEngineSocketClient.h>

#include "ChattingPacket.h"


ServerTestLevel::ServerTestLevel() // default constructer 디폴트 생성자
	: bIsServer_(false)
{

}

ServerTestLevel::~ServerTestLevel() // default destructer 디폴트 소멸자
{

}

void ServerTestLevel::LevelStart()
{
}

void ServerTestLevel::LevelUpdate(float _DeltaTime)
{
	if (bIsServer_ == true && GameEngineInput::GetInst().Down("CloseServer"))
	{
		server_.CloseServer();
		return;
	}

	if (!server_.IsOpened() && !client_.IsConnected() && GameEngineInput::GetInst().Down("CreateServer"))
	{
		bIsServer_ = true;
		server_.Initialize();
		server_.OpenServer();
		server_.AddPacketHandler(ePacketID::Chat, new ChattingPacket);
		return;
	}

	if (!bIsServer_ && GameEngineInput::GetInst().Down("JoinServer"))
	{
		client_.Initialize();
		client_.Connect("127.0.0.1");
		client_.AddPacketHandler(ePacketID::Chat, new ChattingPacket);
		return;
	}

	if (!bIsServer_ && client_.IsConnected() && GameEngineInput::GetInst().Down("SendChat"))
	{
		ChattingPacket packet;
		packet.SetText("테스트 패킷 전송");
		client_.Send(&packet);
	}

	if (server_.IsOpened())
	{
		server_.ProcessPacket();
	}
	else if (client_.IsConnected())
	{
		client_.ProcessPacket();
	}
}

void ServerTestLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{


}

void ServerTestLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	if (false == GameEngineInput::GetInst().IsKey("CreateServer"))
	{
		GameEngineInput::GetInst().CreateKey("CreateServer", '1');
		GameEngineInput::GetInst().CreateKey("JoinServer", '2');

		GameEngineInput::GetInst().CreateKey("SendChat", '3');
		GameEngineInput::GetInst().CreateKey("CloseServer", '4');


	}
}