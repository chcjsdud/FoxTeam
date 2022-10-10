#pragma once
#include <GameEngine/GameEnginePacketBase.h>

#include "MonsterInfo.h"

// �з� : ��Ŷ
// �뵵 : 
// ���� : Ŭ���̾�Ʈ���� ���ҽ� �ε� �� ���ͻ������� ����� ������ ��Ŷ
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
	int TotMonsterCount_;										// �����Ǵ� ���� �Ѱ���
	int CurMonsterCount_;										// ���� ���ŵ� ���� �ε���
	MonsterInfo MonsterInfo_;									// ��������
};

