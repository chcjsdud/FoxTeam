#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

// Ό³Έν :
class GameEngineLevel;
class GameEngineImageRenderer;
class Player : public GameEngineActor
{
public:
	static Player* MainPlayer;

public:
	// constrcuter destructer
	Player();
	~Player();

	// delete Function
	Player(const Player& _Other) = delete;
	Player(Player&& _Other) noexcept = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) noexcept = delete;

private:
	GameEngineCollision* PlayerCollision;
	class GameEngineFBXRenderer* FBXRenderer;

	GameEngineFSM StateManager_;

	float Speed;

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	void TestTimeEvent();

	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
};

