#pragma once
#include <GameEngine/GameEngineActor.h>
// 분류 : 
// 용도 : 
// 설명 : 

class GameEngineCollision;
class PJW_Mouse : public GameEngineActor
{
public:
	PJW_Mouse(); // default constructer 디폴트 생성자
	~PJW_Mouse(); // default destructer 디폴트 소멸자
	PJW_Mouse(const PJW_Mouse& _other) = delete; // default Copy constructer 디폴트 복사생성자
	PJW_Mouse(PJW_Mouse&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	PJW_Mouse& operator=(const PJW_Mouse& _other) = delete; // default Copy operator 디폴트 대입 연산자
	PJW_Mouse& operator=(const PJW_Mouse&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void Start() override;
	void Update(float _deltaTime) override;

public:
	float4 GetRayDirection() { return rayDirection_; }
	float4 GetRayOrigin() { return rayOrigin_; }

	float4 GetIntersectionYAxisPlane(float _height, float _rayLength);

private:
	void updateMouseRay();

private:
	float4 rayDirection_;
	float4 rayOrigin_;

	GameEngineCollision* rayCollision_;

};

