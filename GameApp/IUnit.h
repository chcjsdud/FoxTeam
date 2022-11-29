#pragma once
#include <GameEngine/GameEngineActor.h>

// �з� : ���ֺ��̽�
// �뵵 : 
// ���� : 

class IUnit : public GameEngineActor
{
public:
	virtual int GetIndex() = 0; // ĳ���͸� �������� ID �� ��´�.

	virtual void Damage(float _amount, IUnit* _subject) = 0; // �������� �޴´�.
	virtual void Knockback(float _knockbackTime, float4 _knockbackSpeed) = 0; // �˹�
	virtual void WallSlam(float _knockbackTime, float4 _knockbackSpeed, float _stunTime) = 0; // �˹�. ���� �ε��� ��� ����
	virtual void Slow(float _slowTime, float _slowRatio) = 0; // ������
	virtual void Stun(float _stunTime) = 0; // ����

public:
	UnitType UnitType_;
};

