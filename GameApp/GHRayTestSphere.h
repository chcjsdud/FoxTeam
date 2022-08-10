#pragma once

#include <GameEngine/GameEngineActor.h>

class GameEngineCollision;
class GHRayTestSphere : public GameEngineActor
{
public:
	GHRayTestSphere(); 
	~GHRayTestSphere();

	GHRayTestSphere(const GHRayTestSphere& _other) = delete; 
	GHRayTestSphere(GHRayTestSphere&& _other) = delete; 

	GHRayTestSphere& operator=(const GHRayTestSphere& _other) = delete;
	GHRayTestSphere& operator=(const GHRayTestSphere&& _other) = delete;

public:
	void Start() override;
	void Update(float _deltaTime) override;

private:
	GameEngineCollision* collision_;
	GameEngineRenderer* renderer_;
};

