#pragma once
#include <GameEngine/GameEnginePacketBase.h>

#include "MonsterInfo.h"

// 분류 : 패킷
// 용도 : 
// 설명 : 클라이언트에게 리소스 로드 및 액터생성시작 명령을 내리는 패킷
class CreationCommandPacket : public GameEnginePacketBase
{
public:
	void SetTotMonsterCount(int _Count);
	void SetCurMonsterIndex(int _Index);
	void SetMonsterInfo(MonsterInfo _MonsterInfo);

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:

public:
	CreationCommandPacket();
	~CreationCommandPacket();

protected:
	CreationCommandPacket(const CreationCommandPacket& _other) = delete;
	CreationCommandPacket(CreationCommandPacket&& _other) noexcept = delete;

private:
	CreationCommandPacket& operator=(const CreationCommandPacket& _other) = delete;
	CreationCommandPacket& operator=(const CreationCommandPacket&& _other) = delete;

public:
protected:
private:
	int TotMonsterCount_;										// 생성되는 몬스터 총갯수
	int CurMonsterCount_;										// 현재 수신된 몬스터 인덱스
	MonsterInfo MonsterInfo_;									// 몬스터정보
};

