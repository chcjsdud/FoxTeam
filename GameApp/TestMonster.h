#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
// �з� : 
// �뵵 : 
// ���� : 
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

	// ������, �ݸ���, ��� Componenet �ʱ�ȭ
	void ComponenetInit();
	//State �ʱ�ȭ
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

	void PursuitDirUpdate(float _DeltaTime);
	// update�� �Լ��� ���� �� �̵��� ������
	void MoveUpdate(float _DeltaTime);

	//ĳ���Ͱ� �ٶ󺸴� ������ �ٷ� �ٷ� �Ⱥ��ϰ� õõ�� ����(�̵����� �����ϰ�)
	void MoveRotateUpdate(float _DeltaTime);

#pragma endregion
private:
	//state
	GameEngineFSM monsterState_;

	//�ٴ� �ݸ���
	GameEngineCollision* monsterGroundCollision_;
	//�ǰ� ��Ʈ�ڽ�
	GameEngineCollision* monsterHitBoxCollision_;
	//���� ��Ʈ�ڽ�
	GameEngineCollision* monsterAttackHitBoxCollision_;

	GameEngineCollision* monsterSightBoxCollision_;

	GameEngineFBXRenderer* FBXRenderer_;

	bool IsMove_;

	float Speed_; //�̵� �ӵ�

	//���� ��
	float AttackTurm_;

	//Status
	int Hp_;
	float Stamina_;

	float4 PursuitDir_; // �ٶ���� �� ����
	float4 CurFowordDir_; //���� �ٶ󺸴� ����
	float YRotateSpeed_; // ȸ�� �ӵ�

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


	// ���ӸŴ��� ���� �� �÷��̾� ���� ����� �뵵
	void SetTarget(Player* _player)
	{
		targetPlayer_ = _player;
	}

	void CollisionSight(GameEngineCollision* _other);
};

