#include "Precompile.h"
#include "GameStartPacket.h"

#include "Enums.h"
#include "ePacketID.h"
#include "PlayerInfoManager.h"
#include "UserGame.h"

void GameStartPacket::userSerialize()
{
    //serializer_ << MapName_;
}

void GameStartPacket::userDeserialize()
{
    //serializer_ >> MapName_;
}

void GameStartPacket::initPacketID()
{
    SetPacketID(ePacketID::StartPacket);
}

GameEnginePacketBase* GameStartPacket::getUserObject()
{
    return new GameStartPacket;
}

void GameStartPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
    // 서버(호스트)측에서만 송신하므로 클라이언트(게스트)측 수신처리
    if (false == _bServer)
    {
        // 로딩레벨로 체인지(임시주석)
        //UserGame::LevelChange("LoadingLevel");

        // 로딩레벨 구현중이므로 루미아레벨로 강제이동 - 임시사용
        UserGame::LevelChange("LumiaLevel");
    }
}

GameStartPacket::GameStartPacket()
{
}

GameStartPacket::~GameStartPacket()
{
}
