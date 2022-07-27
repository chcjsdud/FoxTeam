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
	// GameEngineLevel��(��) ���� ��ӵ�
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _DeltaTime) override;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

};

