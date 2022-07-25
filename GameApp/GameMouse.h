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
	GameMouse(); // default constructer ����Ʈ ������
	~GameMouse(); // default destructer ����Ʈ �Ҹ���
	GameMouse(const GameMouse& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameMouse(GameMouse&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	GameMouse& operator=(const GameMouse& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameMouse& operator=(const GameMouse&& _other) noexcept = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

protected:
	// Start() �� �� �ʱ�ȭ �Լ���
	void ImageInit();
	void CollisionInit();

	// Update() �� ���� �ֱ������� üũ�ϴ� �Լ���
	void InputCheck(float _DeltaTime); // ���콺 �� �� Ŭ��, ��ũ�Ѹ��� üũ�մϴ�.
	void DebugUpdate(float _DeltaTime);


	GameEngineCollision* mouseCollision_;
	float4 XYBuffer_;
	void UpdateMove(float _DeltaTime);
	// ���콺 �浹ü�� �� ���� �� �ٴ��� Ÿ�� �ٴ� �� �ְ� �ϴ� �Լ�(���콺�� XY ���� �ΰ����� XZ ������ ��ȯ)

private:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	float4 savedPos_;
	// ���콺 �̵� �� ���콺�� ��ǥ�� �����ϴ� ����

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

