#include "PreCompile.h"
#include "MonsterCreationPacket.h"

#include "Enums.h"
#include "ePacketID.h"
#include "LoadingSyncManager.h"
#include "MonsterInfoManager.h"
#include "UserGame.h"
#include "LumiaLevel.h"

void MonsterCreationPacket::SetMonsterInfos(std::vector<MonsterInfo> _MonsterInfos)
{
    MonsterInfos_ = _MonsterInfos;
}

void MonsterCreationPacket::SetMonsterInfo(MonsterInfo _MonsterInfo)
{
    MonsterInfos_.push_back(_MonsterInfo);
}

void MonsterCreationPacket::userSerialize()
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

void MonsterCreationPacket::userDeserialize()
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

void MonsterCreationPacket::initPacketID()
{
    SetPacketID(ePacketID::MonsterCreatePacket);
}

GameEnginePacketBase* MonsterCreationPacket::getUserObject()
{
    return new MonsterCreationPacket;
}

void MonsterCreationPacket::execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer)
{
    LoadingSyncManager* LoadInfoManager = LoadingSyncManager::GetInstance();
    MonsterInfoManager* InfoManager = MonsterInfoManager::GetInstance();

    // Ŭ���̾�Ʈ(�Խ�Ʈ)�� ���� ó��
    if (false == _bServer)
    {
        // ���Ź��� ���� ���� ���� ��
        InfoManager->SetMonsterInfos(MonsterInfos_);

        // ���� ���� �Լ� ȣ��(Ŭ���̾�Ʈ ���� �Լ�)
        LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));
        PlayerLevel->ClientMonsterCreation();

        // �� �ε����� ���� ����
        LoadInfoManager->GetCurrentLoadingInfo().MonsterLoadEnd_ = 1;
    }
}

MonsterCreationPacket::MonsterCreationPacket()
{
}

MonsterCreationPacket::~MonsterCreationPacket()
{
}
