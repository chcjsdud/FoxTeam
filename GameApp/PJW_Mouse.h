#pragma once
#include <GameEngine/GameEngineActor.h>

// 내 구조에선 마우스가 곧 레이의 역할도 한다.
class GameEngineCollision;
class GameEngineFBXRenderer;
class PJW_Mouse : public GameEngineActor
{
public:
	// 광선 체크
	bool isPicked(const float4& _mousePos, float4& _pickedPos, GameEngineFBXRenderer* _mesh = nullptr);
	// 무엇이 피킹되었는지를 입력받아 밑의 충돌체 / 메시 중 하나의 함수가 실행된다.

	bool isCollPicked(const float4& _mousePos, float4& _pickedPos, GameEngineCollision* _collision = nullptr);
	bool isMeshPicked(GameEngineFBXRenderer* _mesh, const float4& _mousePos, float4& _pickedPos);

public:
	void Start() override;
	void Update(float _deltaTime) override;

public:
	PJW_Mouse(); // default constructer 디폴트 생성자
	~PJW_Mouse(); // default destructer 디폴트 소멸자
	PJW_Mouse(const PJW_Mouse& _other) = delete; // default Copy constructer 디폴트 복사생성자
	PJW_Mouse(PJW_Mouse&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	PJW_Mouse& operator=(const PJW_Mouse& _other) = delete; // default Copy operator 디폴트 대입 연산자
	PJW_Mouse& operator=(const PJW_Mouse&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

protected:
	bool Ray2Dto3D(float _mouseX, float _mouseY);
	// 뷰포트에서 잡힌 좌표를 3D 월드로 변환시킨다.
	bool Ray2Dto3D(float4 _mousePos);
	// 벡터값으로 받는 오버라이드

private:
	float4 originPos_; // 마우스 레이의 원점
	float4 direction_; // 마우스 레이의 방향 벡터
};

