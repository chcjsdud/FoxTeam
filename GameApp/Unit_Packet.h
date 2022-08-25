#pragma once
#include "GameEngine/GameEnginePacketBase.h"

class Unit_Packet : public GameEnginePacketBase
{
private:	// member Var

public:
	Unit_Packet(); // default constructer 디폴트 생성자
	~Unit_Packet(); // default destructer 디폴트 소멸자

public:
	// GameEnginePacketBase을(를) 통해 상속됨
	virtual void initPacketID() override;
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(bool _bServer, GameEngineSocketInterface* _network) override;

	void SetPaketInfo(unsigned int _Unit_ID, int _Order, float4 _TargetPos, unsigned int _Target_Unit_ID)
	{
		Unit_ID_ = _Unit_ID;
		ControlOrder_ = _Order;
		Target_Pos_ = _TargetPos;
		Target_Unit_ID_ = _Target_Unit_ID;
	}

protected:
	unsigned int Unit_ID_;
	unsigned int Target_Unit_ID_;
	int ControlOrder_;
	float4 Target_Pos_;

protected:		// delete constructer
	Unit_Packet(const Unit_Packet& _other) = delete; // default Copy constructer 디폴트 복사생성자
	Unit_Packet(Unit_Packet&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	Unit_Packet& operator=(const Unit_Packet& _other) = delete; // default Copy operator 디폴트 대입 연산자
	Unit_Packet& operator=(const Unit_Packet&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자
};
