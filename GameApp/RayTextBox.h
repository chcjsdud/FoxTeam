#pragma once

#include <GameEngine/GameEngineActor.h>

class GameEngineCollision;
class RayTextBox : public GameEngineActor
{
public:
	RayTextBox(); 
	~RayTextBox();

	RayTextBox(const RayTextBox& _other) = delete; 
	RayTextBox(RayTextBox&& _other) = delete; 

	RayTextBox& operator=(const RayTextBox& _other) = delete;
	RayTextBox& operator=(const RayTextBox&& _other) = delete;

public:
	void Start() override;
	void Update(float _deltaTime) override;

private:
	GameEngineCollision* collision_;
};

