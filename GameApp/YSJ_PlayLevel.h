#pragma once
#include <GameEngine/GameEngineLevel.h>

// ���� : 
class YSJ_PlayLevel : public GameEngineLevel
{
public:
	YSJ_PlayLevel();
	~YSJ_PlayLevel();
	YSJ_PlayLevel(const YSJ_PlayLevel& _other) = delete; 
	YSJ_PlayLevel(YSJ_PlayLevel&& _other) noexcept = delete;
	YSJ_PlayLevel& operator=(const YSJ_PlayLevel& _other) = delete;
	YSJ_PlayLevel& operator=(const YSJ_PlayLevel&& _other) = delete;

protected:
	void LevelStart() override;
	void LevelUpdate(float _DeltaTime) override;
	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

private:
	void CreateActorLevel();
};
