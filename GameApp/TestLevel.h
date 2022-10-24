#pragma once

#include <GameEngine/GameEngineLevel.h>

class Rio;
class Jackie;
class MousePointer;
class LumiaMap;
class TestLevel : public GameEngineLevel
{
public:
	TestLevel(); 
	~TestLevel();

	TestLevel(const TestLevel& _other) = delete; 
	TestLevel(TestLevel&& _other) = delete; 

	TestLevel& operator=(const TestLevel& _other) = delete;
	TestLevel& operator=(const TestLevel&& _other) = delete;

public:
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _DeltaTime) override;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

private:
	Rio* rio_;
	Jackie* jackie_;
	MousePointer* mouse_;
	LumiaMap* map_;
};

