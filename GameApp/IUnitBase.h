#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 유닛베이스
// 용도 : 
// 설명 : 
class IUnitBase : public GameEngineActor
{
public:
	virtual int GetIndex() = 0;
};

