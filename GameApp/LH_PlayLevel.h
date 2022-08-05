#pragma once

#include <GameEngine/GameEngineLevel.h>
#include <GameEngine/GameEngineFSM.h>


class PlayerController;
class PlayLevel : public GameEngineLevel
{
public:
	PlayLevel();
	~PlayLevel();


public:
	virtual void LevelStart() override;
	virtual void LevelUpdate(float _DeltaTime) override;
	virtual void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	virtual void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

protected:

private:
	PlayerController* PlayerController_;

	GameEngineFSM LoadState_;
public:
	PlayerController* GetPlayerController()
	{
		return PlayerController_;
	}

private:
	void CreateActorLevel();

	//FSM으로 구현된 레벨 로딩 단계

	void Load_Start();
	void Load_Update(float _DeltaTime);
	void Load_End();

	void Play_Start();
	void Play_Update(float _DeltaTime);
	void Play_End();

private:
	PlayLevel(const PlayLevel& _Other) = delete;
	PlayLevel(PlayLevel&& _Other) = delete;
	PlayLevel& operator=(const PlayLevel& _Other) = delete;
	PlayLevel& operator=(PlayLevel&& _Other) = delete;
};

