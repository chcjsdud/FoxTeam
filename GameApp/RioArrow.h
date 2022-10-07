#pragma once

#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

class GameEngineFBXRenderer;
class GameEngineCollision;
class RioArrow : public GameEngineActor
{
public:
	RioArrow(); 
	~RioArrow();

	RioArrow(const RioArrow& _other) = delete; 
	RioArrow(RioArrow&& _other) = delete; 

	RioArrow& operator=(const RioArrow& _other) = delete;
	RioArrow& operator=(const RioArrow&& _other) = delete;

public:
	virtual void Start();
	virtual void Update(float _deltaTime);

private:
	GameEngineFSM state_;
	GameEngineFBXRenderer* renderer_;
	GameEngineCollision* collision_;
};

