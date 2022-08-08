#pragma once
#include <GameEngine/GameEngineFSM.h>
#include <GameEngine/GameEngineComponent.h>


class AIController : public GameEngineComponent
{
public:
	AIController();
	~AIController();

public:
	void Start();
	void Update(float _DeltaTime);

	void AIController_Off();
	void AIController_On();

private:

	void ChangePlayerState(std::string _State);

private:
	AIController(const AIController& _other) = delete;
	AIController(AIController&& _other) = delete;
	AIController& operator=(const AIController& _other) = delete;
	AIController& operator=(const AIController&& _other) = delete;
};

