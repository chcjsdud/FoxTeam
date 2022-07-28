#pragma once
#include <GameEngine/GameEngineActor.h>
#include "PJW_Enum.h"
#include <GameEngine/GameEngineCollision.h>

// 상대 타겟팅 등의 복잡한 상호작용을 GameController 에서 처리해주기 위해 만든 클래스입니다.
// 타겟 설정, 공격, 추적 등의 상호작용을 할 때 벡터의 인덱스값으로만 불러 올 수 있게


class PJW_GameController;
class PJW_Player : public GameEngineActor
{
	friend PJW_GameController;

public:
	PJW_Player(); // default constructer 디폴트 생성자
	~PJW_Player(); // default destructer 디폴트 소멸자
	PJW_Player(const PJW_Player& _other) = delete; // default Copy constructer 디폴트 복사생성자
	PJW_Player(PJW_Player&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	PJW_Player& operator=(const PJW_Player& _other) = delete; // default Copy operator 디폴트 대입 연산자
	PJW_Player& operator=(const PJW_Player&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	inline void SetCollisionBody(InGameCollisionType _inGameCollisionType)
	{
		// 콜리젼 타입(플레이어 몇의 콜리젼인지...) 를 세팅해주는 함수.
		// 컨트롤러에서 캐릭터가 생성될 때 쓰입니다.
		collision_Body_->SetCollisionGroup(_inGameCollisionType);
	}

	inline void SetCollisionAttackRange(InGameCollisionType _inGameCollisionType)
	{
		// 콜리젼 타입(플레이어 몇의 콜리젼인지...) 를 세팅해주는 함수.
		// 컨트롤러에서 캐릭터가 생성될 때 쓰입니다.
		collision_Attack_->SetCollisionGroup(_inGameCollisionType);
	}

	// 공격 타겟을 선정하는 함수
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

	// 스탯
protected:
	float status_HP_;
	float status_ATK_;
	float status_MoveSpeed_;

	// 현재 체력 마력
protected:
	float curHP_;

	// 기타 변수
protected:
	float deltaTime_;
	bool isMoving_;
	bool isAttacking_;
	float4 aimDir_;
	float4 curDir_;

protected:
	GameEngineCollision* collision_Body_;
	// 몸통 콜리젼
	GameEngineCollision* collision_Attack_;
	// 공격 범위 콜리젼 : 이 내부에 상대의 body 가 접촉되면 공격 수행
};

