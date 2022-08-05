#pragma once

#include <GameEngine/GameEngineActor.h>

class GameEngineFBXRenderer;
class GHMap : public GameEngineActor
{
public:
	GHMap(); 
	~GHMap();

	GHMap(const GHMap& _other) = delete; 
	GHMap(GHMap&& _other) = delete; 

	GHMap& operator=(const GHMap& _other) = delete;
	GHMap& operator=(const GHMap&& _other) = delete;

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime) override;

public:
	GameEngineFBXRenderer* renderer_;
};

