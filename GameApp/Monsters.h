#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 
// 용도 : 
// 설명 : 
class Monsters : public GameEngineActor
{
private:

public:
	Monsters();
	~Monsters();

protected:		// delete constructer
	Monsters(const Monsters& _other) = delete;
	Monsters(Monsters&& _other) noexcept = delete;

private:		//delete operator
	Monsters& operator=(const Monsters& _other) = delete;
	Monsters& operator=(const Monsters&& _other) = delete;

public:
};

