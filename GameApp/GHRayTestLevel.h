#pragma once

#include <GameEngine\GameEngineLevel.h>

class GHMousePointer;
class GHMap;
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

public:
	GHMousePointer* GetMousePointer() { return mouse_; }
	GHMap* GetMap() { return map_; }

private:
	void loadResource();
	void releaseResource();
	void createActor();
	void adjustCamera();

	void initRenderWindow();
	void releaseRenderWindow();


private:
	GHMousePointer* mouse_;
	GHMap* map_;
	GameEngineActor* player_;
};

