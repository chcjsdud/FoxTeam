#pragma once

#include <GameEngine/GameEngineActor.h>

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
};

