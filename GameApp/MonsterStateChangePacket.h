#pragma once
#include <GameEngine/GameEnginePacketBase.h>

// 분류 : 패킷
// 용도 : 
// 설명 : GameTimeController에 의하여 서버(호스트)가 클라이언트(게스트)에게 일괄처리 목적으로 상태전환을 명령하는 패킷
class MonsterStateChangePacket : public GameEnginePacketBase
{
public: // Inline Get Function
public: // Inline Set Function
	inline void SetMonsterType(MonsterType _Type)
	{
		Type_ = _Type;
	}

	inline void SetMonsterStateType(MonsterStateType _StateType)
	{
		StateType_ = _StateType;
	}

protected:
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual void initPacketID() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _sender, GameEngineSocketInterface* _network, bool _bServer) override;

private:

public:
	MonsterStateChangePacket();
	~MonsterStateChangePacket();

protected:
	MonsterStateChangePacket(const MonsterStateChangePacket& _other) = delete;
	MonsterStateChangePacket(MonsterStateChangePacket&& _other) noexcept = delete;

private:
	MonsterStateChangePacket& operator=(const MonsterStateChangePacket& _other) = delete;
	MonsterStateChangePacket& operator=(const MonsterStateChangePacket&& _other) = delete;

public:
protected:
private:
	MonsterType Type_;													// 상태를 전환하려는 몬스터타입
	MonsterStateType StateType_;										// 전환하려는 상태
};

