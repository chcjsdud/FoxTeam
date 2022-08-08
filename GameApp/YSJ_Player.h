#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

// Ό³Έν :
class GameEngineLevel;
class GameEngineImageRenderer;
class YSJ_Player : public GameEngineActor
{
public:
	static YSJ_Player* MainPlayer;

public:
	// constrcuter destructer
	YSJ_Player();
	~YSJ_Player();

	// delete Function
	YSJ_Player(const YSJ_Player& _Other) = delete;
	YSJ_Player(YSJ_Player&& _Other) noexcept = delete;
	YSJ_Player& operator=(const YSJ_Player& _Other) = delete;
	YSJ_Player& operator=(YSJ_Player&& _Other) noexcept = delete;

	void SetColor(float4 _Color)
	{
		Color_ = _Color;
	}

protected:

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;



private:
	GameEngineCollision* PlayerCollision;

	GameEngineFSM StateManager_;

	float Speed;
	float4 Color_;

};

