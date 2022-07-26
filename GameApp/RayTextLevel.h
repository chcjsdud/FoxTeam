#pragma once

#include <GameEngine\GameEngineLevel.h>

class RayTextLevel : public GameEngineLevel
{
public:
	RayTextLevel(); 
	~RayTextLevel();

	RayTextLevel(const RayTextLevel& _other) = delete; 
	RayTextLevel(RayTextLevel&& _other) = delete; 

	RayTextLevel& operator=(const RayTextLevel& _other) = delete;
	RayTextLevel& operator=(const RayTextLevel&& _other) = delete;

public:
	// GameEngineLevel을(를) 통해 상속됨
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _DeltaTime) override;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

};

