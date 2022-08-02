#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineRenderer;
class GameEngineCollision;
class SJH_TestActor : public GameEngineActor
{
public:

protected:

private:
	void Start() override;
	void Update(float _deltaTime) override;

public:
	SJH_TestActor();
	~SJH_TestActor();

protected:		// delete constructer
	SJH_TestActor(const SJH_TestActor& _other) = delete;
	SJH_TestActor(SJH_TestActor&& _other) noexcept = delete;

private:		//delete operator
	SJH_TestActor& operator=(const SJH_TestActor& _other) = delete;
	SJH_TestActor& operator=(const SJH_TestActor&& _other) = delete;

public:
	void SetResultColor(const float4& _Color);

protected:

private:
	GameEngineRenderer* BoxRenderer_;
	GameEngineCollision* BoxCollider_;
};

