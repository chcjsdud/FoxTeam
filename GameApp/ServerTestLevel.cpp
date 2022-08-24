#include "PreCompile.h"
#include "ServerTestLevel.h"
#include "ePacketID.h"
#include <GameEngine/GameEngineSocketServer.h>
#include <GameEngine/GameEngineSocketClient.h>

#include "ChattingPacket.h"


ServerTestLevel::ServerTestLevel() // default constructer 디폴트 생성자
	: server_(nullptr),client_(nullptr), bIsServer_(false)
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
	if (true == bIsServer_)
	{
		server_->ProcessPacket();
	}


	if (true == GameEngineInput::GetInst().Down("CloseServer"))
	{
		server_->CloseServer();
	}

	if (true == GameEngineInput::GetInst().Down("CreateServer"))
	{
		GameEngineSocketServer s;
		s.Initialize();
		s.OpenServer();
		s.AddPacketHandler(ePacketID::Chat, new ChattingPacket);

		bIsServer_ = true;
	}

	if (false == bIsServer_ && true == GameEngineInput::GetInst().Down("JoinServer"))
	{

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