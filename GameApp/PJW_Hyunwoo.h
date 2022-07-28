#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include <GameEngine/GameEngineFBXRenderer.h>

class GameEngineCollision;
class PJW_Hyunwoo : public GameEngineActor
{

public:
	PJW_Hyunwoo(); 
	~PJW_Hyunwoo(); 
	PJW_Hyunwoo(const PJW_Hyunwoo& _other) = delete;
	PJW_Hyunwoo(PJW_Hyunwoo&& _other) noexcept;
	PJW_Hyunwoo& operator=(const PJW_Hyunwoo& _other) = delete;
	PJW_Hyunwoo& operator=(const PJW_Hyunwoo&& _other) = delete;

public:
	// 공격 타겟을 선정하는 함수
	void SetTarget(PJW_Hyunwoo* _target)
	{
		target_ = _target;
	}

	// 현재 타겟을 리턴하는 함수
	PJW_Hyunwoo* GetTarget()
	{
		return target_;
	}

	void GetDamage(float _damage)
	{
		curHP_ -= _damage;
	}

	


protected: 
	virtual void Start();
	void Init_FBX();
	void Init_FSM();
	void Init_Collision();

	virtual void Update(float _DeltaTime);
	void Check_Collision(float _DeltaTime);
	void Check_HP();
	void Check_DebugRect(float _DeltaTime);

// FSM
private:
	GameEngineFSM hyunwooState_;

// States 함수
	void Idle_Start();
	void Idle_Update(float _DeltaTime);
	void Idle_End();

	void Move_Start();
	void Move_Update(float _DeltaTime);
	void Move_End();

	void Attack_Start();
	void Attack_Update(float _DeltaTime);
	void Attack_End();

	void Skill_Q_Start();
	void Skill_Q_Update(float _DeltaTime);
	void Skill_Q_End();

	void Death_Start();
	void Death_Update(float _DeltaTime);
	void Death_End();



// FBX 렌더러
private:
	GameEngineFBXRenderer* FBXRenderer_;


// 충돌체
private:
	GameEngineCollision* collision_Body_;
	// 몸통 콜리젼

	GameEngineCollision* collision_Attack_; 
	// 공격 범위 콜리젼 : 이 내부에 상대의 body 가 접촉되면 공격 수행


private:
	PJW_Hyunwoo* target_;

// 스탯
private:
	float status_HP_;
	float status_ATK_;
	float status_MoveSpeed_;

// 현재 체력 마력
private:
	float curHP_;

// 기타 변수
private:
	float deltaTime_;
	bool isMoving_;
	float4 aimDir_;
	float4 curDir_;
};

