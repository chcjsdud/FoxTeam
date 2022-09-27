#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 마우스
// 용도 : UI 상호 및 플레이어 이동
// 설명 : 
class GameEngineUIRenderer;
class GameEngineCollision;
class MousePointer : public GameEngineActor
{
public: // Static Value
	static MousePointer* InGameMouse;

public: // Inline Get Function
	inline float4 GetRayDirection()
	{
		return rayDirection_;
	}

	inline float4 GetRayOrigin()
	{
		return rayOrigin_;
	}

	inline GameEngineCollision* GetRayCollision()
	{
		return rayCollision_;
	}

public: // Public Function
	float4 GetIntersectionYAxisPlane(float _height, float _rayLength);
	GameEngineCollision* GetPickCollision(int _Order);

protected:
private:
	void updateMouseRay();

private:
	void Start() override;
	void Update(float _deltaTime) override;
	void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;
	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;

public:
	MousePointer();
	~MousePointer();

protected:
	MousePointer(const MousePointer& _other) = delete;
	MousePointer(MousePointer&& _other) = delete;

private:
	MousePointer& operator=(const MousePointer& _other) = delete;
	MousePointer& operator=(const MousePointer&& _other) = delete;

private: // Ray(InGame)
	float4 rayDirection_;
	float4 rayOrigin_;
	GameEngineCollision* rayCollision_;

private: // Mouse(UI)
	GameEngineUIRenderer* MouseRenderer_;
	GameEngineCollision* MouseCollider_;
};

