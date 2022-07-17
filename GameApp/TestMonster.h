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
	TestMonster(); // default constructer ����Ʈ ������
	~TestMonster(); // default destructer ����Ʈ �Ҹ���
	TestMonster(const TestMonster& _other) = delete; // default Copy constructer ����Ʈ ���������
	TestMonster(TestMonster&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	TestMonster& operator=(const TestMonster& _other) = delete; // default Copy operator ����Ʈ ���� ������
	TestMonster& operator=(const TestMonster&& _other) noexcept = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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

	void KeyDirUpdate(float _DeltaTime);
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
	float Speed_;

	float4 KeyDir_;
	float4 CurFowordDir_;

	//Y�� ȸ�� �ӵ�
	float YRotateSpeed_;


	//���� ��
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


	// ���ӸŴ��� ���� �� �÷��̾� ���� ����� �뵵
	void SetTarget(Player* _player)
	{
		targetPlayer_ = _player;
	}

	void CollisionSight(GameEngineCollision* _other);
};

