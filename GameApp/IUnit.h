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
};

