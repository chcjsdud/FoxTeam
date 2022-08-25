#pragma once

#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

class GameEngineFBXRenderer;
class GHMap;
class GHNavMesh;
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
	void startIdle(float _deltaTime);
	void updateIdle(float _deltaTime);

	void startRun(float _deltaTime);
	void updateRun(float _deltaTime);

private:
	const float SPEED = 300.f;

private:
	GameEngineFBXRenderer* renderer_;
	GameEngineFSM state_;

	float4 destination_;
	float4 direction_;

	GHMap* currentMap_;
	GHNavMesh* currentNavMesh_;

	std::vector<float4> destinations_;
};