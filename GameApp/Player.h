#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
/// <summary>
/// ���� ���ΰ� ����
/// </summary>
// ���� :
class GameEngineLevel;
class GameEngineImageRenderer;
class Player : public GameEngineActor
{
public:
	Player();
	~Player();
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
	GameEngineFBXMesh* mesh_;
};

