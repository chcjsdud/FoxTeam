#pragma once
#include <GameEngine/GameEngineLevel.h>
#include <GameEngine/GameEngineFSM.h>

// Ό³Έν :
class MouseActor;
class PlayLevel : public GameEngineLevel
{
public:
	// constrcuter destructer
	PlayLevel();
	~PlayLevel();

	// delete Function
	PlayLevel(const PlayLevel& _Other) = delete;
	PlayLevel(PlayLevel&& _Other) noexcept = delete;
	PlayLevel& operator=(const PlayLevel& _Other) = delete;
	PlayLevel& operator=(PlayLevel&& _Other) noexcept = delete;

	MouseActor* MActor; 
	class PostFade* FadeEffect;

protected:

	class GameEngineFBXWindow* FBXWindow;

private:
	GameEngineFSM LoadState_;

private:
	void CreateActorLevel();

	void LevelStart() override;
	void LevelUpdate(float _DeltaTime) override;
	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;


	void Load_Start();
	void Load_Update();
	void Load_End();

	void Play_Start();
	void Play_Update();
	void Play_End();
};

