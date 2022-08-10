#pragma once

#include <GameEngine/GameEngineActor.h>

class GameEngineCollision;
class GHRayTestOBBBox : public GameEngineActor
{
public:
	GHRayTestOBBBox(); 
	~GHRayTestOBBBox();

	GHRayTestOBBBox(const GHRayTestOBBBox& _other) = delete; 
	GHRayTestOBBBox(GHRayTestOBBBox&& _other) = delete; 

	GHRayTestOBBBox& operator=(const GHRayTestOBBBox& _other) = delete;
	GHRayTestOBBBox& operator=(const GHRayTestOBBBox&& _other) = delete;

public:
	void Start() override;
	void Update(float _deltaTime) override;

private:
	GameEngineCollision* collision_;
	GameEngineRenderer* renderer_;
};

