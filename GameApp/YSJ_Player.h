#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

// ���� :
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

protected:
	GameEngineCollision* PlayerCollision;

	GameEngineFSM StateManager_;

	float Speed;

private:
	void Start() override;
	void Update(float _DeltaTime) override;

	void TestTimeEvent();

	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
};
