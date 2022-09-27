#pragma once

#include <GameEngine/GameEngineActor.h>

class GameEngineCollision;
class MousePointer : public GameEngineActor
{
public:
	MousePointer(); 
	~MousePointer();
	
	MousePointer(const MousePointer& _other) = delete; 
	MousePointer(MousePointer&& _other) = delete; 

	MousePointer& operator=(const MousePointer& _other) = delete;
	MousePointer& operator=(const MousePointer&& _other) = delete;

public:
	void Start() override;
	void Update(float _deltaTime) override;

public:
	float4 GetRayDirection() { return rayDirection_; }
	float4 GetRayOrigin() { return rayOrigin_; }

	float4 GetIntersectionYAxisPlane(float _height, float _rayLength);

	GameEngineCollision* GetPickCollision(int _Order);
	GameEngineCollision* GetRayCollision() { return rayCollision_; }

private:
	void updateMouseRay();

private:
	float4 rayDirection_;
	float4 rayOrigin_;

	GameEngineCollision* rayCollision_;
};

