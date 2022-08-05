#pragma once

#include <GameEngine\GameEngineLevel.h>

class GHRayTestLevel : public GameEngineLevel
{
public:
	GHRayTestLevel(); 
	~GHRayTestLevel();

	GHRayTestLevel(const GHRayTestLevel& _other) = delete; 
	GHRayTestLevel(GHRayTestLevel&& _other) = delete; 

	GHRayTestLevel& operator=(const GHRayTestLevel& _other) = delete;
	GHRayTestLevel& operator=(const GHRayTestLevel&& _other) = delete;

public:
	// GameEngineLevel을(를) 통해 상속됨
	void LevelStart() override;
	void LevelUpdate(float _DeltaTime) override;
	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;


private:
	void loadResource();
	void releaseResource();
	void createActor();
	void adjustCamera();

	void initRenderWindow();
	void releaseRenderWindow();

};

