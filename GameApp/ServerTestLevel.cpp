#include "PreCompile.h"
#include "ServerTestLevel.h"
#include "ePacketID.h"
#include <GameEngine/GameEngineSocketServer.h>
#include <GameEngine/GameEngineSocketClient.h>

#include "ChattingPacket.h"


GameEngineSocketServer* ServerTestLevel::server_ = nullptr;

ServerTestLevel::ServerTestLevel() // default constructer 디폴트 생성자
	: client_(nullptr), bIsServer_(false)
{

}

ServerTestLevel::~ServerTestLevel() // default destructer 디폴트 소멸자
{

}

ServerTestLevel::ServerTestLevel(ServerTestLevel&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void ServerTestLevel::LevelStart()
{

}

void ServerTestLevel::LevelUpdate(float _DeltaTime)
{
	if (nullptr != server_ && true == GameEngineInput::GetInst().Down("CloseServer"))
	{
		server_->CloseServer();
	}

	if (nullptr == server_ && nullptr == client_ && true == GameEngineInput::GetInst().Down("CreateServer"))
	{
		GameEngineSocketServer s;
		s.Initialize();
		s.OpenServer();
		s.AddPacketHandler(ePacketID::Chat, new ChattingPacket);

		server_ = &s;
		bIsServer_ = true;
	}

	if (nullptr == server_ && nullptr == client_ && false == bIsServer_ && true == GameEngineInput::GetInst().Down("JoinServer"))
	{
		GameEngineSocketClient c;
		c.Initialize();
		c.Connect("127.0.0.1");
		c.AddPacketHandler(ePacketID::Chat, new ChattingPacket);
		
		client_ = &c;
	}


	if (nullptr != server_)
	{
	//	server_->ProcessPacket();
	}
	else if (nullptr != client_)
	{
	//	client_->ProcessPacket();
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