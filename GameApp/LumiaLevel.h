#pragma once

#include <GameEngine/GameEngineLevel.h>

#include <GameApp/MousePointer.h>
#include <GameApp/YSJ_LumiaMap.h>
#include <GameApp/ItemBoxManager.h>
#include <GameApp/GHRio.h>

class LumiaMap;
class LumiaLevel : public GameEngineLevel
{
public:
	LumiaLevel();
	~LumiaLevel();
	LumiaLevel(const LumiaLevel& _other) = delete;
	LumiaLevel(LumiaLevel&& _other) = delete;
	LumiaLevel& operator=(const LumiaLevel& _other) = delete;
	LumiaLevel& operator=(const LumiaLevel&& _other) = delete;

public:
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _DeltaTime) override;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

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
	GHRio* player_;

	MousePointer* mouse_;
	LumiaMap* map_;

	ItemBoxManager* ItemBox_;
};

