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
	TestMonster(); 
	~TestMonster(); 
	TestMonster(const TestMonster& _other) = delete;
	TestMonster(TestMonster&& _other) = delete; 
	TestMonster& operator=(const TestMonster& _other) = delete; 
	TestMonster& operator=(const TestMonster&& _other)  = delete; 

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

	// HP 공격력 이동 속도 등의 "몬스터의 특성" 을 Init 해주는 함수입니다.
	virtual void TraitInit();
	

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

	void PursuitDirUpdate(float _DeltaTime);
	// update류 함수에 같이 들어가 이동을 제어함
	void MoveUpdate(float _DeltaTime);

	//캐릭터가 바라보는 방향이 바로 바로 안변하고 천천히 변함(이동과는 무관하게)
	void MoveRotateUpdate(float _DeltaTime);

	void DamageUpdate(float _DeltaTime);
	// 업데이트에서 계속 실행되어 플레이어의 공격 충돌체와의 접촉 체크를 수행합니다.
	void KnockBackUpdate(float _DeltaTime);
	// 위의 DamageUpdate() 에서 실행되어 피격 시 넉백을 수행합니다.
	void DeathUpdate(float _DeltaTime);
	// 업데이트에서 계속 실행되어 남은 체력을 체크해 죽음 판정을 수행합니다.
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

	
	//Status
	int Hp_;

	// 공격받을 때 무적시간 관련 변수 (콜리젼이 한 번에 여러 번 파파팍 들어가지 않게)
	float InvincibleTermRate_; // 무적 기준 시간 (이 몬스터는 무적시간이 얼마나 가나...)
	float InvincibleTermTimer_; // 델타타임에 의해 깎여나가는 무적시간 타이머
	bool isDamaged_; // 방금 공격받았나? 변수. 무적시간 타이머가 끝나면 false 

	// 공격받을 때 넉백 관련 변수
	float KnockBackRate_; // 넉백 기준 시간 (이 몬스터는 넉백 시간이 얼마나 가나...)
	float KnockBackTimer_; // 델타타임에 의해 깎여나가는 넉백 타이머
	float KnockBackSpeed_; // 넉백시 뒤로 물러나는 스피드
	bool isKnockBack_; // 방금 넉백되었나?


	int AttackPower_;


	float AttackTurm_;
	float Speed_;

	float4 PursuitDir_; // 바라봐야 할 방향
	float4 CurFowordDir_; //현재 바라보는 방향
	float YRotateSpeed_; // 회전 속도

	Player* targetPlayer_;
	//float4 prevmoveVector_;
	//float4 moveVector_;
	//float4 movePos_;

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
	void CollisionGetDamaged(GameEngineCollision* _other);
};

