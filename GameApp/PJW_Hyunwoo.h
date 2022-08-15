#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include <GameEngine/GameEngineFBXRenderer.h>
#include "PJW_Player.h"

class GameEngineCollision;
class PJW_Hyunwoo : public PJW_Player
{

public:
	PJW_Hyunwoo();
	~PJW_Hyunwoo();
	PJW_Hyunwoo(const PJW_Hyunwoo& _other) = delete;
	PJW_Hyunwoo(PJW_Hyunwoo&& _other) noexcept;
	PJW_Hyunwoo& operator=(const PJW_Hyunwoo& _other) = delete;
	PJW_Hyunwoo& operator=(const PJW_Hyunwoo&& _other) = delete;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	GameEngineFBXRenderer* FBXRenderer_;
};

