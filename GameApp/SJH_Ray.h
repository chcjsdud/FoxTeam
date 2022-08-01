#pragma once
#include <GameEngine/GameEngineActor.h>

// �з� : ����
// �뵵 : ī�޶� �ٶ󺸴� �������� �߻��ϴ� ���⺤��
// ���� : 
class GameEngineCollision;
class SJH_Ray : public GameEngineActor
{
public: // �ش� ���콺 Ŭ�������� �浹�ϴ� ��ü�� ã�Ƴ� �� ��ü�� ��ġ���� ��ȯ
	float4 RayAtViewSpace(float _MousePosX, float _MousePosY);
	float4 RayAtViewSpace(float4 _MousePos);

public: // 
	bool IsPicked(GameEngineCollision* _Collider);

protected:

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	SJH_Ray();
	~SJH_Ray();

protected:		// delete constructer
	SJH_Ray(const SJH_Ray& _other) = delete;
	SJH_Ray(SJH_Ray&& _other) noexcept = delete;

private:		//delete operator
	SJH_Ray& operator=(const SJH_Ray& _other) = delete;
	SJH_Ray& operator=(const SJH_Ray&& _other) = delete;

public:
	
protected:

private:
	float4 OriginPos_;				// ���콺 Ŭ����ǥ
	float4 Direction_;				// ī�޶� �ٶ󺸴� ����
};

