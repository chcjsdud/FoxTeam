#pragma once
#include <GameEngine/GameEngineActor.h>
// �з� : 
// �뵵 : 
// ���� : 

class GameEngineCollision;
class PJW_Mouse : public GameEngineActor
{
public:
	PJW_Mouse(); // default constructer ����Ʈ ������
	~PJW_Mouse(); // default destructer ����Ʈ �Ҹ���
	PJW_Mouse(const PJW_Mouse& _other) = delete; // default Copy constructer ����Ʈ ���������
	PJW_Mouse(PJW_Mouse&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	PJW_Mouse& operator=(const PJW_Mouse& _other) = delete; // default Copy operator ����Ʈ ���� ������
	PJW_Mouse& operator=(const PJW_Mouse&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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

