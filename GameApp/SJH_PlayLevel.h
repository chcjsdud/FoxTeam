#pragma once
#include <GameEngine/GameEngineLevel.h>

// 분류 : 
// 용도 : 
// 설명 : 
class SKySphereActor;
class SJH_FloorMap;
class SJH_Mouse;
class SJH_Yuki;
class SJH_PlayLevel : public GameEngineLevel
{
public:

protected:

private:
	void LevelStart() override;
	void LevelUpdate(float _DeltaTime) override;
	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

private:
	void CreateActorLevel();

public:
	SJH_PlayLevel();
	~SJH_PlayLevel();

protected:
	SJH_PlayLevel(const SJH_PlayLevel& _other) = delete;
	SJH_PlayLevel(SJH_PlayLevel&& _other) noexcept = delete;

private:
	SJH_PlayLevel& operator=(const SJH_PlayLevel& _other) = delete;
	SJH_PlayLevel& operator=(const SJH_PlayLevel&& _other) = delete;

public:

protected:

private:
	SKySphereActor* Background_;
	SJH_FloorMap* FloorMap_;
	SJH_Mouse* InGameMouse_;
	SJH_Yuki* Yuki_;
};

