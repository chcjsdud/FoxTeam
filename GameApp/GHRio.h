#pragma once

#include <GameEngine/GameEngineActor.h>
#include "Character.h"
#include <GameEngine/GameEngineFSM.h>

class GameEngineFBXRenderer;
class LumiaMap;
class Rio : public Character
{
public:
	bool TempFlag = false;

public:
	Rio();
	~Rio();
	Rio(const Rio& _other) = delete;
	Rio(Rio&& _other) = delete;
	Rio& operator=(const Rio& _other) = delete;
	Rio& operator=(const Rio&& _other) = delete;

public:
	void Start() override;
	void Update(float _deltaTime) override;

public:
	void InitSpawnPoint(const float4& _position);


private:
	void initRendererAnimation();
	void initInput();
	void initState();


#pragma region MainState
	void startNormal();
	void updateNormal(float _deltaTime);

	void startCrowdControl();
	void updateCrowdControl(float _deltaTime);
	void endCrowdControl();
#pragma endregion

#pragma region NormalState
	void startIdle();
	void updateIdle(float _deltaTime);

	void startRun();
	void updateRun(float _deltaTime);
#pragma endregion

#pragma region CrowdControlState
	void startStun();
	void updateStun(float _deltaTime);
#pragma endregion


private:
	const float SPEED = 300.f;

private:
	GameEngineFBXRenderer* renderer_;

	GameEngineFSM mainState_;
	GameEngineFSM normalState_;
	GameEngineFSM crowdControlState_;

	float4 destination_;
	float4 direction_;

	LumiaMap* currentMap_;

	std::vector<float4> destinations_;
};