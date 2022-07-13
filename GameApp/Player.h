#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
/// <summary>
/// 여우 주인공 액터
/// </summary>
// 설명 :
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

	// 렌더러, 콜리전, 등등 Componenet 초기화
	void ComponenetInit();
	//State 초기화
	void StateInit();
	void KeyInit();

private:
	//State
	void Idle_Start();
	void Idle_Update(float _DeltaTime);
	void Idle_End();

	void Walk_Start();
	void Walk_Update(float _DeltaTime);
	void Walk_End();

	void Run_Start();
	void Run_Update(float _DeltaTime);
	void Run_End();

	void Attack_Start();
	void Attack_Update(float _DeltaTime);
	void Attack_End();

private:
	//Member Func
	void Attack();


	void CameraUpdate(float _DeltaTime);
	void MoveUpdate(float _DeltaTime);

	void RotateFunc(float _DeltaTime);


private:
	//state
	GameEngineFSM PlayerState_;

	//component
	GameEngineCollision* PlayerCollision_;

	GameEngineCollision* PlayerGroundCollision_;
	GameEngineCollision* PlayerHitBoxCollision_;

	GameEngineFBXRenderer* FBXRenderer_;

	GameEngineFBXMesh* mesh_; //필요한가?

	float Speed_;

	float4 CurFowordDir_;
	float4 KeyDir_;

private:
};

