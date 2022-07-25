#pragma once
#include <GameEngine/MouseActor.h>

class GameEngineCollision;
class GameMouse : public MouseActor
{

public:
	GameMouse(); // default constructer 디폴트 생성자
	~GameMouse(); // default destructer 디폴트 소멸자
	GameMouse(const GameMouse& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameMouse(GameMouse&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	GameMouse& operator=(const GameMouse& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameMouse& operator=(const GameMouse&& _other) noexcept = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

protected:
	void ImageInit();
	void CollisionInit();
	GameEngineCollision* mouseCollision_;

private:
	void Start() override;
	void Update(float _DeltaTime) override;

};

