#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// 분류 : 
// 용도 : 
// 설명 : 
class Player;
class GameEngineLevel;
class GameEngineImageRenderer;
class TestMonster : public GameEngineActor
{
private:
	friend Player;

public:
	TestMonster(); // default constructer 디폴트 생성자
	~TestMonster(); // default destructer 디폴트 소멸자
	TestMonster(const TestMonster& _other) = delete; // default Copy constructer 디폴트 복사생성자
	TestMonster(TestMonster&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	TestMonster& operator=(const TestMonster& _other) = delete; // default Copy operator 디폴트 대입 연산자
	TestMonster& operator=(const TestMonster&& _other) noexcept = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

#pragma region Init
private:
	//Init

	// 렌더러, 콜리전, 등등 Componenet 초기화
	void ComponenetInit();
	//State 초기화
	void StateInit();

#pragma endregion

#pragma region State
private:
	void Idle_Start();
	void Idle_Update(float _DeltaTime);
	void Idle_End();

	void Pursuit_Start();
	void Pursuit_Update(float _DeltaTime);
	void Pursuit_End();

	void Attack_Start();
	void Attack_Update(float _DeltaTime);
	void Attack_End();
#pragma endregion

#pragma region MemberUpdateFunc
private:
	void DEBUGUpdate(float _DeltaTime);

	void KeyDirUpdate(float _DeltaTime);
	// update류 함수에 같이 들어가 이동을 제어함
	void MoveUpdate(float _DeltaTime);

	//캐릭터가 바라보는 방향이 바로 바로 안변하고 천천히 변함(이동과는 무관하게)
	void MoveRotateUpdate(float _DeltaTime);

#pragma endregion
private:
	//state
	GameEngineFSM monsterState_;

	//바닥 콜리전
	GameEngineCollision* monsterGroundCollision_;
	//피격 히트박스
	GameEngineCollision* monsterHitBoxCollision_;
	//공격 히트박스
	GameEngineCollision* monsterAttackHitBoxCollision_;

	GameEngineCollision* monsterSightBoxCollision_;

	GameEngineFBXRenderer* FBXRenderer_;

	bool IsMove_;
	float Speed_;

	float4 KeyDir_;
	float4 CurFowordDir_;

	//Y축 회전 속도
	float YRotateSpeed_;


	//공격 텀
	float AttackTurm_;

	//Status
	int Hp_;
	float Stamina_;

	Player* targetPlayer_;
	float4 prevmoveVector_;
	float4 moveVector_;
	float4 movePos_;

public:
	const int GetHP()
	{
		return Hp_;
	}

	void SetHP(int _Hp)
	{
		Hp_ = _Hp;
	}

	void SetDamage(int _damage)
	{
		Hp_ -= _damage;
	}


	// 게임매니저 생성 전 플레이어 추적 디버깅 용도
	void SetTarget(Player* _player)
	{
		targetPlayer_ = _player;
	}

	void CollisionSight(GameEngineCollision* _other);
};

