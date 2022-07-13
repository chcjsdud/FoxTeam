#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
/// <summary>
/// ���� ���ΰ� ����
/// </summary>
// ���� :
class GameEngineLevel;
class GameEngineImageRenderer;
class Player : public GameEngineActor
{
public:
	Player();
	~Player();
	Player(const Player& _Other) = delete;
	Player(Player&& _Other) noexcept = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) noexcept = delete;

private:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	//Init

	// ������, �ݸ���, ��� Componenet �ʱ�ȭ
	void ComponenetInit();
	//State �ʱ�ȭ
	void StateInit();
	void KeyInit();

private:
	//State
	void Idle_Start();
	void Idle_Update();
	void Idle_End();

	void Walk_Start();
	void Walk_Update();
	void Walk_End();

	void Run_Start();
	void Run_Update();
	void Run_End();

	void Attack_Start();
	void Attack_Update();
	void Attack_End();

private:
	//Member Func
	void Attack();
	void MoveUpdate();

private:
	//state
	GameEngineFSM PlayerState_;

	//component
	GameEngineCollision* PlayerCollision_;

	GameEngineCollision* PlayerGroundCollision_;
	GameEngineCollision* PlayerHitBoxCollision_;

	GameEngineFBXRenderer* FBXRenderer_;

	GameEngineFBXMesh* mesh_; //�ʿ��Ѱ�?

	float Speed_;
private:
};

