#pragma once

#include <GameEngine/GameEngineActor.h>

class GameEngineCollision;
class RayTestBox : public GameEngineActor
{
public:
	RayTestBox();
	~RayTestBox();

	RayTestBox(const RayTestBox& _other) = delete;
	RayTestBox(RayTestBox&& _other) = delete;

	RayTestBox& operator=(const RayTestBox& _other) = delete;
	RayTestBox& operator=(const RayTestBox&& _other) = delete;

public:
	void Start() override;
	void Update(float _deltaTime) override;

private:
	GameEngineCollision* collision_;
};

