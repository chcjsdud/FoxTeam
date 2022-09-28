#include "PreCompile.h"
#include "CharacterCreationPacket.h"

#include "Enums.h"
#include "ePacketID.h"
#include "LoadingSyncManager.h"
#include "PlayerInfoManager.h"
#include "UserGame.h"
#include "LumiaLevel.h"

#include "LoadingEndPacket.h"

#include "LoadingLevel_LoadPercent.h"

void CharacterCreationPacket::userSerialize()
{
}

void CharacterCreationPacket::userDeserialize()
{
}

void CharacterCreationPacket::initPacketID()
{
    SetPacketID(ePacketID::CharacterCreatePacket);
}

GameEnginePacketBase* CharacterCreationPacket::getUserObject()
{
    return new CharacterCreationPacket;
}

void CharacterCreationPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
    LoadingSyncManager* LoadInfoManager = LoadingSyncManager::GetInstance();
    PlayerInfoManager* PlayerInfo = PlayerInfoManager::GetInstance();

    // 클라이언트(게스트)측 수신 처리
    if (false == _bServer)
    {
        // 강제 생성 명령 실행
        LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));
        PlayerLevel->ClientCharacterCreation();

        // 내 로딩정보 상태 갱신
        LoadInfoManager->GetCurrentLoadingInfo().CharacterLoadEnd_ = 1;

        // 캐릭터 정보 생성까지 완료시 현재 게임 진입가능
        PlayerInfo->GetPlayerList()[PlayerInfo->GetMyNumber()].IsLoading_ = 1;

        // 로딩완료 패킷 송신 후
        LoadingEndPacket EndPacket;
        EndPacket.SetUniqueID(PlayerInfo->GetMyNumber());
        EndPacket.SetLoadingFlag(PlayerInfo->GetPlayerList()[PlayerInfo->GetMyNumber()].IsLoading_);
        _network->Send(&EndPacket);

        // 내 로딩퍼센트 갱신
        LoadingLevel_LoadPercent::Percent->CheckLoadingPercent();
    }
}

CharacterCreationPacket::CharacterCreationPacket()
{
}

CharacterCreationPacket::~CharacterCreationPacket()
{
}
