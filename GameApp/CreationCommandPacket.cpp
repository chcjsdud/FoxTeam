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

void CreationCommandPacket::SetTotMonsterCount(int _Count)
{
    TotMonsterCount_ = _Count;
}

void CreationCommandPacket::SetMonsterInfo(MonsterInfo _MonsterInfo)
{
    MonsterInfo_ = _MonsterInfo;
}

void CreationCommandPacket::userSerialize()
{
    serializer_ << TotMonsterCount_;
    serializer_ << MonsterInfo_.Index_;                                         // 생성인덱스(탐색용)
    serializer_ << static_cast<int>(MonsterInfo_.RegionType_);                  // 생성지역(탐색용)
    serializer_ << static_cast<int>(MonsterInfo_.MonsterType_);                 // 몬스터(야생동물) 타입
    serializer_ << MonsterInfo_.IsGroup_;                                       // 그룹생성여부
    serializer_ << MonsterInfo_.GroupCount_;                                    // 그룹생성시 생성되어야하는 몬스터(야생동물)수
    serializer_ << MonsterInfo_.SpawnPosition_;                                 // 몬스터(야생동물) 스폰(둥지) 위치
}

void CreationCommandPacket::userDeserialize()
{
    serializer_ >> TotMonsterCount_;
    serializer_ >> MonsterInfo_.Index_;                                         // 생성인덱스(탐색용)

    int RegionType = 0;
    serializer_ >> RegionType;                                                  // 생성지역(탐색용)
    MonsterInfo_.RegionType_ = static_cast<Location>(RegionType);

    int Type = 0;
    serializer_ >> Type;                                                        // 몬스터(야생동물) 타입
    MonsterInfo_.MonsterType_ = static_cast<MonsterType>(Type);

    serializer_ >> MonsterInfo_.IsGroup_;                                       // 그룹생성여부
    serializer_ >> MonsterInfo_.GroupCount_;                                    // 그룹생성시 생성되어야하는 몬스터(야생동물)수
    serializer_ >> MonsterInfo_.SpawnPosition_;                                 // 몬스터(야생동물) 스폰(둥지) 위치
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
        InfoManager->AddMonsterInfo(MonsterInfo_);

        // 221010 SJH 임시처리 : 패킷문제로 해당 패킷수신시 바로 생성으로 처리
        LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));
        PlayerLevel->GuestCreateCommand();

        // 221010 SJH : 임시주석처리
        //// 강제 생성 함수 호출(클라이언트 전용 함수) - 스레드
        //// 단, 몬스터생성 총갯수를 모두 수신했을때 호출
        //if (TotMonsterCount_ == InfoManager->GetCurMonsterListSize())
        //{
        //    LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));
        //    PlayerLevel->GuestCreateCommand();
        //}
    }
}

CreationCommandPacket::CreationCommandPacket()
    : TotMonsterCount_(-1)
    , MonsterInfo_{}
{
}

CreationCommandPacket::~CreationCommandPacket()
{
}
