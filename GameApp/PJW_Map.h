#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngine/GameEngineRenderer.h>

class GameEngineImageRenderer;
class PJW_Map : public GameEngineActor
{
public:
	PJW_Map();
	~PJW_Map();
	PJW_Map(const PJW_Map& _other) = delete;
	PJW_Map(PJW_Map&& _other) noexcept; 
	PJW_Map& operator=(const PJW_Map& _other) = delete; 
	PJW_Map& operator=(const PJW_Map&& _other) = delete; 

public:

protected:

	void Start() override;

private:
	GameEngineRenderer* renderer_;
	GameEngineCollision* mapCollision_;
};

