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
        serializer_ << MonsterInfos_[i].Index_;                                         // 생성인덱스(탐색용)
        serializer_ << static_cast<int>(MonsterInfos_[i].AreaType_);                    // 생성지역(탐색용)
        serializer_ << static_cast<int>(MonsterInfos_[i].MonsterType_);                 // 몬스터(야생동물) 타입
        serializer_ << MonsterInfos_[i].IsGroup_;                                       // 그룹생성여부
        serializer_ << MonsterInfos_[i].GroupCount_;                                    // 그룹생성시 생성되어야하는 몬스터(야생동물)수
        serializer_ << MonsterInfos_[i].SpawnPosition_;                                 // 몬스터(야생동물) 스폰(둥지) 위치
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

        serializer_ >> NewMonsterInfo.IsGroup_;
        serializer_ >> NewMonsterInfo.GroupCount_;
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

    // 클라(게스트)에서만 수신
    if (false == _bServer)
    {
        // 수신받은 몬스터 정보 셋팅 후
        InfoManager->SetMonsterInfos(MonsterInfos_);

        // 강제 생성 함수 호출(클라이언트 전용 함수) - 스레드
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
