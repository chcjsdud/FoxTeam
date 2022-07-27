#pragma once

#include <GameEngine/GameEngineActor.h>

class GameEngineCollision;
class GHRayTestBox : public GameEngineActor
{
public:
	GHRayTestBox();
	~GHRayTestBox();

	GHRayTestBox(const GHRayTestBox& _other) = delete;
	GHRayTestBox(GHRayTestBox&& _other) = delete;

	GHRayTestBox& operator=(const GHRayTestBox& _other) = delete;
	GHRayTestBox& operator=(const GHRayTestBox&& _other) = delete;

public:
	void Start() override;
	void Update(float _deltaTime) override;

private:
	GameEngineCollision* collision_;
	GameEngineRenderer* renderer_;
};

