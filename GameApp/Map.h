#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

class GameEngineLevel;
class GameEngineImageRenderer;
class Map : public GameEngineActor
{
private:	// member Var

public:
	Map(); // default constructer 디폴트 생성자
	~Map(); // default destructer 디폴트 소멸자

private:
	Map(const Map& _other) = delete; 
	Map(Map&& _other) = delete;
	Map& operator=(const Map& _other) = delete; 
	Map& operator=(const Map&& _other) = delete;

private:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	GameEngineCollision* GroundCollision_;

public:
};

