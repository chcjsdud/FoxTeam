#pragma once

#include <GameEngine/GameEngineLevel.h>

class LumiaLevel : public GameEngineLevel
{
public:
	LumiaLevel();
	~LumiaLevel();

public:
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _DeltaTime) override;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;


};

