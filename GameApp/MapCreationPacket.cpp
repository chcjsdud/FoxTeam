#include "PreCompile.h"
#include "MapCreationPacket.h"

#include "Enums.h"
#include "ePacketID.h"
#include "LoadingSyncManager.h"
#include "PlayerInfoManager.h"
#include "UserGame.h"
#include "LumiaLevel.h"

void MapCreationPacket::userSerialize()
{
}

void MapCreationPacket::userDeserialize()
{
}

void MapCreationPacket::initPacketID()
{
    SetPacketID(ePacketID::MapCreatPacket);
}

GameEnginePacketBase* MapCreationPacket::getUserObject()
{
    return new MapCreationPacket;
}

void MapCreationPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
    LoadingSyncManager* LoadSyncInfo = LoadingSyncManager::GetInstance();
    PlayerInfoManager* PlayerInfo = PlayerInfoManager::GetInstance();

    // 클라이언트(게스트)측에서 수신 처리
    if (false == _bServer)
    {
        // 해당 패킷 수신시 LumiaLevel 강제 맵생성 명령 전달
        LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));
        PlayerLevel->ClientMapCreation();

        // 내 로딩정보 상태 갱신
        LoadSyncInfo->GetCurrentLoadingInfo().MapLoadEnd_ = 1;
    }
}

MapCreationPacket::MapCreationPacket()
{
}

MapCreationPacket::~MapCreationPacket()
{
}
