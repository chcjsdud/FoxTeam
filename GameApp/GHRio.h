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

	void Move(const float4& _position);
	void MoveWithPathFind(const float4& _position);

private:
	// ó�� �������� �� �ؾ��� ���� �Լ��� �и� ����
	void initRendererAnimation();
	void initInput();
	void initState();

#pragma region MainState
	void startNormalState();
	void updateNormalState(float _deltaTime);

	void startCrowdControlState();
	void updateCrowdControlState(float _deltaTime);
	void endCrowdControlState();

#pragma endregion

	// �Ϲ����� ����
#pragma region NormalState
	void startWait();
	void updateWait(float _deltaTime);

	void startRun();
	void updateRun(float _deltaTime);
#pragma endregion

	// CC, �����̻���� �޴� ����
#pragma region CrowdControlState
	void startStun();
	void updateStun(float _deltaTime);
#pragma endregion


private:
	const float SPEED = 300.f;

private:
	GameEngineFBXRenderer* renderer_;

	GameEngineFSM state_;
	GameEngineFSM normalState_;
	GameEngineFSM crowdControlState_;

	float4 destination_;
	float4 direction_;

	std::vector<float4> destinations_;

	float mousePressDelay_;
};