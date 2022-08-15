#pragma once
#include <GameEngine/GameEngineActor.h>

// �� �������� ���콺�� �� ������ ���ҵ� �Ѵ�.
class GameEngineCollision;
class GameEngineFBXRenderer;
class PJW_Mouse : public GameEngineActor
{
public:
	// ���� üũ
	bool isPicked(const float4& _mousePos, float4& _pickedPos, GameEngineFBXRenderer* _mesh = nullptr);
	// ������ ��ŷ�Ǿ������� �Է¹޾� ���� �浹ü / �޽� �� �ϳ��� �Լ��� ����ȴ�.

	bool isCollPicked(const float4& _mousePos, float4& _pickedPos, GameEngineCollision* _collision = nullptr);
	bool isMeshPicked(GameEngineFBXRenderer* _mesh, const float4& _mousePos, float4& _pickedPos);

public:
	void Start() override;
	void Update(float _deltaTime) override;

public:
	PJW_Mouse(); // default constructer ����Ʈ ������
	~PJW_Mouse(); // default destructer ����Ʈ �Ҹ���
	PJW_Mouse(const PJW_Mouse& _other) = delete; // default Copy constructer ����Ʈ ���������
	PJW_Mouse(PJW_Mouse&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	PJW_Mouse& operator=(const PJW_Mouse& _other) = delete; // default Copy operator ����Ʈ ���� ������
	PJW_Mouse& operator=(const PJW_Mouse&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

protected:
	bool Ray2Dto3D(float _mouseX, float _mouseY);
	// ����Ʈ���� ���� ��ǥ�� 3D ����� ��ȯ��Ų��.
	bool Ray2Dto3D(float4 _mousePos);
	// ���Ͱ����� �޴� �������̵�

private:
	float4 originPos_; // ���콺 ������ ����
	float4 direction_; // ���콺 ������ ���� ����
};

