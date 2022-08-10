#pragma once

#include <GameEngine/GameEngineActor.h>

class GameEngineCollision;
class GHMousePointer : public GameEngineActor
{
public:
	GHMousePointer(); 
	~GHMousePointer();
	
	GHMousePointer(const GHMousePointer& _other) = delete; 
	GHMousePointer(GHMousePointer&& _other) = delete; 

	GHMousePointer& operator=(const GHMousePointer& _other) = delete;
	GHMousePointer& operator=(const GHMousePointer&& _other) = delete;

public:
	void Start() override;
	void Update(float _deltaTime) override;

private:
	void updateMouseRay();

private:
	float4 rayDirection_;
	float4 rayOrigin_;

	GameEngineCollision* rayCollision_;
};

