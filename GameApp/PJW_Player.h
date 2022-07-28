#pragma once
#include <GameEngine/GameEngineActor.h>
#include "PJW_Enum.h"
#include <GameEngine/GameEngineCollision.h>

// ��� Ÿ���� ���� ������ ��ȣ�ۿ��� GameController ���� ó�����ֱ� ���� ���� Ŭ�����Դϴ�.
// Ÿ�� ����, ����, ���� ���� ��ȣ�ۿ��� �� �� ������ �ε��������θ� �ҷ� �� �� �ְ�


class PJW_GameController;
class PJW_Player : public GameEngineActor
{
	friend PJW_GameController;

public:
	PJW_Player(); // default constructer ����Ʈ ������
	~PJW_Player(); // default destructer ����Ʈ �Ҹ���
	PJW_Player(const PJW_Player& _other) = delete; // default Copy constructer ����Ʈ ���������
	PJW_Player(PJW_Player&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	PJW_Player& operator=(const PJW_Player& _other) = delete; // default Copy operator ����Ʈ ���� ������
	PJW_Player& operator=(const PJW_Player&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	inline void SetCollisionBody(InGameCollisionType _inGameCollisionType)
	{
		// �ݸ��� Ÿ��(�÷��̾� ���� �ݸ�������...) �� �������ִ� �Լ�.
		// ��Ʈ�ѷ����� ĳ���Ͱ� ������ �� ���Դϴ�.
		collision_Body_->SetCollisionGroup(_inGameCollisionType);
	}

	inline void SetCollisionAttackRange(InGameCollisionType _inGameCollisionType)
	{
		// �ݸ��� Ÿ��(�÷��̾� ���� �ݸ�������...) �� �������ִ� �Լ�.
		// ��Ʈ�ѷ����� ĳ���Ͱ� ������ �� ���Դϴ�.
		collision_Attack_->SetCollisionGroup(_inGameCollisionType);
	}

	// ���� Ÿ���� �����ϴ� �Լ�
	void SetTarget(PJW_Player* _target)
	{
		target_ = _target;
	}

	PJW_Player* GetTarget()
	{
		return target_;
	}

	void GetDamage(float _damage)
	{
		curHP_ -= _damage;
	}

	void SetMove(bool _true)
	{
		isMoving_ = _true;
	}

protected:
	PJW_Player* target_;

	// ����
protected:
	float status_HP_;
	float status_ATK_;
	float status_MoveSpeed_;

	// ���� ü�� ����
protected:
	float curHP_;

	// ��Ÿ ����
protected:
	float deltaTime_;
	bool isMoving_;
	bool isAttacking_;
	float4 aimDir_;
	float4 curDir_;

protected:
	GameEngineCollision* collision_Body_;
	// ���� �ݸ���
	GameEngineCollision* collision_Attack_;
	// ���� ���� �ݸ��� : �� ���ο� ����� body �� ���˵Ǹ� ���� ����
};

