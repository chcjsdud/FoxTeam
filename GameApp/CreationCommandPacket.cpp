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

void CreationCommandPacket::SetCurMonsterIndex(int _Index)
{
    CurMonsterCount_ = _Index;
}

void CreationCommandPacket::SetMonsterInfo(MonsterInfo _MonsterInfo)
{
    MonsterInfo_ = _MonsterInfo;
}

void CreationCommandPacket::userSerialize()
{
    serializer_ << TotMonsterCount_;
    serializer_ << CurMonsterCount_;
    serializer_ << MonsterInfo_.Index_;                                         // �����ε���(Ž����)
    serializer_ << static_cast<int>(MonsterInfo_.RegionType_);                  // ��������(Ž����)
    serializer_ << static_cast<int>(MonsterInfo_.MonsterType_);                 // ����(�߻�����) Ÿ��
    serializer_ << MonsterInfo_.IsGroup_;                                       // �׷��������
    serializer_ << MonsterInfo_.GroupCount_;                                    // �׷������ �����Ǿ���ϴ� ����(�߻�����)��
    serializer_ << MonsterInfo_.SpawnPosition_;                                 // ����(�߻�����) ����(����) ��ġ
}

void CreationCommandPacket::userDeserialize()
{
    serializer_ >> TotMonsterCount_;
    serializer_ >> CurMonsterCount_;
    serializer_ >> MonsterInfo_.Index_;                                         // �����ε���(Ž����)

    int RegionType = 0;
    serializer_ >> RegionType;                                                  // ��������(Ž����)
    MonsterInfo_.RegionType_ = static_cast<Location>(RegionType);

    int Type = 0;
    serializer_ >> Type;                                                        // ����(�߻�����) Ÿ��
    MonsterInfo_.MonsterType_ = static_cast<MonsterType>(Type);

    serializer_ >> MonsterInfo_.IsGroup_;                                       // �׷��������
    serializer_ >> MonsterInfo_.GroupCount_;                                    // �׷������ �����Ǿ���ϴ� ����(�߻�����)��
    serializer_ >> MonsterInfo_.SpawnPosition_;                                 // ����(�߻�����) ����(����) ��ġ
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
        InfoManager->AddMonsterInfo(MonsterInfo_);

        // �ӽ�ó��
        LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));
        PlayerLevel->GuestCreateCommand();

        //// ���� ���� �Լ� ȣ��(Ŭ���̾�Ʈ ���� �Լ�) - ������
        //// ��, ���ͻ��� �Ѱ����� ��� ���������� ȣ��
        //if (TotMonsterCount_ == InfoManager->GetCurMonsterListSize())
        //{
        //    LumiaLevel* PlayerLevel = reinterpret_cast<LumiaLevel*>(UserGame::LevelFind("LumiaLevel"));
        //    PlayerLevel->GuestCreateCommand();
        //}
    }
}

CreationCommandPacket::CreationCommandPacket()
    : TotMonsterCount_(-1)
    , CurMonsterCount_(0)
    , MonsterInfo_{}
{
}

CreationCommandPacket::~CreationCommandPacket()
{
}
