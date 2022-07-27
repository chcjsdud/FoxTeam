#pragma once

#include <GameEngine/GameEngineActor.h>

class GameEngineFBXRenderer;
class GHRio : public GameEngineActor
{
public:
	GHRio(); 
	~GHRio();
	GHRio(const GHRio& _other) = delete; 
	GHRio(GHRio&& _other) = delete; 
	GHRio& operator=(const GHRio& _other) = delete;
	GHRio& operator=(const GHRio&& _other) = delete;
	
public:
	void Start() override;
	void Update(float _deltaTime) override;

private:
	GameEngineFBXRenderer* renderer_;
};

