#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 유닛베이스
// 용도 : 
// 설명 : 
class IUnit : public GameEngineActor
{
public:
	virtual int GetIndex() = 0;
	virtual void Damage(float _amount ) = 0;
	virtual void Damage(float _Amount, GameEngineActor* _Target) = 0;			// 221017 SJH ADD : 몬스터는 타겟필요

public:
	inline UnitType GetUnitType() const											// 221017 SJH ADD : 해당 유닛의 타입(공격타입 지정때 사용)
	{
		return UnitType_;
	}

protected:
	UnitType UnitType_;
};

