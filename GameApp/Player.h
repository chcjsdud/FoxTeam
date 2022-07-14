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

#pragma region Init
private:
	//Init

	// 렌더러, 콜리전, 등등 Componenet 초기화
	void ComponenetInit();
	//State 초기화
	void StateInit();
	void KeyInit();
#pragma endregion

#pragma region State
private:
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
#pragma endregion

private:
	//Member Func
	void Attack();

	// update류 함수에 같이 들어가 카메라 업데이트, 바로 뒤에서 바라보는 함수
	void CameraUpdate_BackPosition(float _DeltaTime);
	void CameraUpdate_UpPosition(float _DeltaTime);

	// update류 함수에 같이 들어가 이동을 제어함
	void MoveUpdate(float _DeltaTime);

	//캐릭터가 바라보는 방향이 바로 바로 안변하고 천천히 변함(이동과는 무관하게)
	void MoveRotateUpdate(float _DeltaTime);

	void ChangeCamFunc(void (Player::* _Camfunc)(float))
	{
		CamFunc_ = std::bind(_Camfunc, this, std::placeholders::_1);
	}
private:
	//state
	GameEngineFSM PlayerState_;

	//component

	GameEngineCollision* PlayerGroundCollision_;
	GameEngineCollision* PlayerHitBoxCollision_;

	GameEngineFBXRenderer* FBXRenderer_;

	//GameEngineFBXMesh* mesh_; //필요한가?

	float Speed_;

	float4 KeyDir_;
	float4 CurFowordDir_;

	//Y축 회전 속도
	float YRotateSpeed_;

	std::function<void(float)> CamFunc_;

private:
};

