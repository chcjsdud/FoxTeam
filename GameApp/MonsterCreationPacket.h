#pragma once
#include <GameEngine/GameEnginePacketBase.h>

#include "MonsterInfo.h"

// �з� : ��Ŷ
// �뵵 : 
// ���� : ���� ���� ��� ��Ŷ
class MonsterCreationPacket : public GameEnginePacketBase
{
public:
	void SetMonsterInfos(std::vector<MonsterInfo> _MonsterInfos);
	void SetMonsterInfo(MonsterInfo _MonsterInfo);

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:

public:
	MonsterCreationPacket();
	~MonsterCreationPacket();

protected:
	MonsterCreationPacket(const MonsterCreationPacket& _other) = delete;
	MonsterCreationPacket(MonsterCreationPacket&& _other) noexcept = delete;

private:
	MonsterCreationPacket& operator=(const MonsterCreationPacket& _other) = delete;
	MonsterCreationPacket& operator=(const MonsterCreationPacket&& _other) = delete;

public:
protected:
private:
	std::vector<MonsterInfo> MonsterInfos_;						// ���� �ʿ� ��ġ�Ǵ� ��� ���� ����
};

