#pragma once
#include <GameEngine/GameEngineActor.h>

// Ό³Έν : 
class HyperLoop : public GameEngineActor
{
public:
	HyperLoop();
	~HyperLoop();
	HyperLoop(const HyperLoop& _other) = delete; 
	HyperLoop(HyperLoop&& _other) noexcept = delete;
	HyperLoop& operator=(const HyperLoop& _other) = delete;
	HyperLoop& operator=(const HyperLoop&& _other) = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	GameEngineFBXRenderer* renderer_;
};

