#pragma once
#include <GameEngine/MouseActor.h>

class GameEngineCollision;
class GameMouse : public MouseActor
{

public:
	GameMouse(); // default constructer ����Ʈ ������
	~GameMouse(); // default destructer ����Ʈ �Ҹ���
	GameMouse(const GameMouse& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameMouse(GameMouse&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	GameMouse& operator=(const GameMouse& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameMouse& operator=(const GameMouse&& _other) noexcept = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

protected:
	void ImageInit();
	void CollisionInit();
	GameEngineCollision* mouseCollision_;

private:
	void Start() override;
	void Update(float _DeltaTime) override;

};

