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

    // Ŭ���̾�Ʈ(�Խ�Ʈ)������ ���� ó��
    if (false == _bServer)
    {
        // �ش� ��Ŷ ���Ž� LumiaLevel ���� �ʻ��� ��� ����
        LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));
        PlayerLevel->ClientMapCreation();

        // �� �ε����� ���� ����
        LoadSyncInfo->GetCurrentLoadingInfo().MapLoadEnd_ = 1;
    }
}

MapCreationPacket::MapCreationPacket()
{
}

MapCreationPacket::~MapCreationPacket()
{
}
