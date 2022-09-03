#pragma once

#include <GameEngine\GameEngineLevel.h>

class MousePointer;
class LumiaMap;
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
	// GameEngineLevel��(��) ���� ��ӵ�
	void LevelStart() override;
	void LevelUpdate(float _DeltaTime) override;
	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

public:
	MousePointer* GetMousePointer() { return mouse_; }
	LumiaMap* GetMap() { return map_; }

private:
	void loadResource();
	void releaseResource();
	void createActor();
	void adjustCamera();

	void initRenderWindow();
	void releaseRenderWindow();


private:
	MousePointer* mouse_;
	LumiaMap* map_;
	GameEngineActor* player_;
};

