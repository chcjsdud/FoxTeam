#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 마우스
// 용도 : UI 상호 및 플레이어 이동
// 설명 : 
class GameEngineUIRenderer;
class GameEngineCollision;
class SJH_Mouse : public GameEngineActor
{
public:
	inline float4 GetMouseClickPos()
	{
		return MouseClickPos_;
	}

protected:

private:
	bool IsPicked();

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	SJH_Mouse();
	~SJH_Mouse();

protected:
	SJH_Mouse(const SJH_Mouse& _other) = delete;
	SJH_Mouse(SJH_Mouse&& _other) noexcept = delete;

private:
	SJH_Mouse& operator=(const SJH_Mouse& _other) = delete;
	SJH_Mouse& operator=(const SJH_Mouse&& _other) = delete;

public:

protected:

private:
	GameEngineUIRenderer* Renderer_;
	GameEngineCollision* Collider_;

private:
	GameEngineActor* Target_;

private:
	float4 MouseClickPos_;
};

