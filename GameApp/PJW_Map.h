#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngine/GameEngineRenderer.h>

class PJW_Hyunwoo;
class GameEngineFBXRenderer;
class PJW_Map : public GameEngineActor
{
	friend PJW_Hyunwoo;

public:
	PJW_Map();
	~PJW_Map();
	PJW_Map(const PJW_Map& _other) = delete;
	PJW_Map(PJW_Map&& _other) noexcept; 
	PJW_Map& operator=(const PJW_Map& _other) = delete; 
	PJW_Map& operator=(const PJW_Map&& _other) = delete; 

public:
	void SetMainPlayer(PJW_Hyunwoo* _player)
	{
		mainPlayer_ = _player;
	}

protected:

	void Start() override;

private:
	GameEngineFBXRenderer* renderer_;
	GameEngineCollision* mapCollision_;
	PJW_Hyunwoo* mainPlayer_;


};

