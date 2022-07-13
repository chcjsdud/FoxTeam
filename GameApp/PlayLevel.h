#pragma once

#include <GameEngine/GameEngineLevel.h>
#include <GameEngine/GameEngineFSM.h>

class PlayLevel : public GameEngineLevel
{
public:
	PlayLevel();
	~PlayLevel();
	PlayLevel(const PlayLevel& _Other) = delete;
	PlayLevel(PlayLevel&& _Other) noexcept = delete;
	PlayLevel& operator=(const PlayLevel& _Other) = delete;
	PlayLevel& operator=(PlayLevel&& _Other) noexcept = delete;

public:
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _DeltaTime) override;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

protected:

private:


private:
	GameEngineFSM LoadState_;

private:
	void CreateActorLevel();

	//FSM으로 구현된 레벨 로딩 단계

	void Load_Start();
	void Load_Update(float _DeltaTime);
	void Load_End();

	void Play_Start();
	void Play_Update(float _DeltaTime);
	void Play_End();
};

