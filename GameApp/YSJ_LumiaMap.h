#pragma once
#include <GameEngine/GameEngineActor.h>

// Ό³Έν : 
class GameEngineFBXRenderer;
class YSJ_LumiaMap : public GameEngineActor
{
public:
	static YSJ_LumiaMap* MainMap;

public:
	YSJ_LumiaMap();
	~YSJ_LumiaMap();
	YSJ_LumiaMap(const YSJ_LumiaMap& _other) = delete; 
	YSJ_LumiaMap(YSJ_LumiaMap&& _other) noexcept = delete;
	YSJ_LumiaMap& operator=(const YSJ_LumiaMap& _other) = delete;
	YSJ_LumiaMap& operator=(const YSJ_LumiaMap&& _other) = delete;

	GameEngineFBXRenderer* GetFBXNaviRenderer()
	{
		return FBXNaviRenderer;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void ReleaseEvent() override;
	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

private:

	GameEngineFBXRenderer* FBXNaviRenderer;
	GameEngineFBXRenderer* FBXWallRenderer;
	GameEngineFBXRenderer* FBXSchoolMap;
};

