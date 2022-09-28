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

    // Ŭ���̾�Ʈ(�Խ�Ʈ)�� ���� ó��
    if (false == _bServer)
    {
        // ���� ���� ��� ����
        LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));
        PlayerLevel->ClientCharacterCreation();

        // �� �ε����� ���� ����
        LoadInfoManager->GetCurrentLoadingInfo().CharacterLoadEnd_ = 1;

        // ĳ���� ���� �������� �Ϸ�� ���� ���� ���԰���
        PlayerInfo->GetPlayerList()[PlayerInfo->GetMyNumber()].IsLoading_ = 1;

        // �ε��Ϸ� ��Ŷ �۽� ��
        LoadingEndPacket EndPacket;
        EndPacket.SetUniqueID(PlayerInfo->GetMyNumber());
        EndPacket.SetLoadingFlag(PlayerInfo->GetPlayerList()[PlayerInfo->GetMyNumber()].IsLoading_);
        _network->Send(&EndPacket);

        // �� �ε��ۼ�Ʈ ����
        LoadingLevel_LoadPercent::Percent->CheckLoadingPercent();
    }
}

CharacterCreationPacket::CharacterCreationPacket()
{
}

CharacterCreationPacket::~CharacterCreationPacket()
{
}
