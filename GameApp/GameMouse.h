#pragma once
#include <GameEngine/MouseActor.h>

class PlayController;
class Player;
class GameEngineCollision;
class GameMouse : public MouseActor
{
	friend PlayController;
	friend Player;

public:
	GameMouse(); // default constructer 디폴트 생성자
	~GameMouse(); // default destructer 디폴트 소멸자
	GameMouse(const GameMouse& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameMouse(GameMouse&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	GameMouse& operator=(const GameMouse& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameMouse& operator=(const GameMouse&& _other) noexcept = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

protected:
	// Start() 에 들어갈 초기화 함수들
	void ImageInit();
	void CollisionInit();

	// Update() 에 들어가서 주기적으로 체크하는 함수들
	void InputCheck(float _DeltaTime); // 마우스 좌 우 클릭, 스크롤링을 체크합니다.
	void DebugUpdate(float _DeltaTime);


	GameEngineCollision* mouseCollision_;
	float4 XYBuffer_;
	void UpdateMove(float _DeltaTime);
	// 마우스 충돌체가 인 게임 내 바닥을 타고 다닐 수 있게 하는 함수(마우스의 XY 값을 인게임의 XZ 값으로 변환)

private:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	float4 savedPos_;
	// 마우스 이동 시 마우스의 좌표를 저장하는 버퍼

	void SaveCurPos()
	{
		savedPos_ = GameEngineInput::GetInst().GetMouse3DPos();
			//UIRenderer->GetTransform()->GetWorldPosition();
	}

public:
	const float4 GetSavedPos()
	{
		return savedPos_;
	}

};

