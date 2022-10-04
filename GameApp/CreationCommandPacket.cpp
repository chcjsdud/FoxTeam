#include "PreCompile.h"
#include "CreationCommandPacket.h"

#include "Enums.h"
#include "ePacketID.h"
#include "PlayerInfoManager.h"
#include "MonsterInfoManager.h"
#include "UserGame.h"
#include "LoadingLevel.h"
#include "LumiaLevel.h"

#include "LoadingEndPacket.h"

#include "LoadingLevel_LoadPercent.h"

void CreationCommandPacket::SetMonsterInfos(std::vector<MonsterInfo> _MonsterInfos)
{
    MonsterInfos_ = _MonsterInfos;
}

void CreationCommandPacket::SetMonsterInfo(MonsterInfo _MonsterInfo)
{
    MonsterInfos_.push_back(_MonsterInfo);
}

void CreationCommandPacket::userSerialize()
{
    int AllMonsterCount = static_cast<int>(MonsterInfos_.size());
    serializer_ << AllMonsterCount;

    for (int i = 0; i < AllMonsterCount; ++i)
    {
        serializer_ << MonsterInfos_[i].Index_;
        serializer_ << static_cast<int>(MonsterInfos_[i].AreaType_);
        serializer_ << static_cast<int>(MonsterInfos_[i].MonsterType_);
        serializer_ << MonsterInfos_[i].SpawnPosition_;
    }
}

void CreationCommandPacket::userDeserialize()
{
    int AllMonsterCount = -1;
    serializer_ >> AllMonsterCount;

    for (int i = 0; i < AllMonsterCount; ++i)
    {
        MonsterInfo NewMonsterInfo = {};

        serializer_ >> NewMonsterInfo.Index_;

        int RcvAreaType = -1;
        serializer_ >> RcvAreaType;
        NewMonsterInfo.AreaType_ = static_cast<Location>(RcvAreaType);

        int RcvMonsterType = -1;
        serializer_ >> RcvMonsterType;
        NewMonsterInfo.MonsterType_ = static_cast<MonsterType>(RcvMonsterType);

        serializer_ >> NewMonsterInfo.SpawnPosition_;

        MonsterInfos_.push_back(NewMonsterInfo);
    }
}

void CreationCommandPacket::initPacketID()
{
    SetPacketID(ePacketID::CreationPacket);
}

GameEnginePacketBase* CreationCommandPacket::getUserObject()
{
    return new CreationCommandPacket;
}

void CreationCommandPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
    MonsterInfoManager* InfoManager = MonsterInfoManager::GetInstance();

    // Ŭ��(�Խ�Ʈ)������ ����
    if (false == _bServer)
    {
        // ���Ź��� ���� ���� ���� ��
        InfoManager->SetMonsterInfos(MonsterInfos_);

        // ���� ���� �Լ� ȣ��(Ŭ���̾�Ʈ ���� �Լ�) - ������
        LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));
        PlayerLevel->GuestCreateCommand();
    }
}

CreationCommandPacket::CreationCommandPacket()
{
}

CreationCommandPacket::~CreationCommandPacket()
{
}
