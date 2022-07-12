#pragma once

#include <GameEngine\GameEngineActor.h>

/// <summary>
/// ���� ���ΰ� ����
/// </summary>

class GameEngineFBXMesh;
class Player : public GameEngineActor
{
public:
	Player();
	~Player();
	Player(const Player& _Other) = delete;
	Player(Player&& _Other) noexcept = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) noexcept = delete;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	GameEngineFBXMesh* mesh_;
};

