#pragma once
#include "LH_Player.h"

// �з� : ĳ����
// �뵵 : ��Ű
// ���� : ���� ��ų �� ������ ����
class Player_Yuki : public Player
{
private:

public:
	Player_Yuki();
	~Player_Yuki();

private:
	//FBX Renderer
	GameEngineFBXRenderer* FBXRenderer_;
	GameEngineCollision* AttackHitBoxCollision_;

private:		//delete operator
	void Start() override;
	void Update(float _DeltaTime) override;

	// ������, �ݸ���, ��� Componenet �ʱ�ȭ
	void ComponenetInit();
	//State �ʱ�ȭ
	void StateInit();
	void UIInit();

	void DEBUGUpdate(float _DeltaTime);

private:
	void Idle_Start() override;
	void Idle_Update(float _DeltaTime) override;
	void Idle_End() override;

	void Walk_Start() override;
	void Walk_Update(float _DeltaTime) override;
	void Walk_End() override;

	void Run_Start() override;
	void Run_Update(float _DeltaTime) override;
	void Run_End() override;

	void Attack_Start() override;
	void Attack_Update(float _DeltaTime) override;
	void Attack_End() override;

private:		// delete constructer
	Player_Yuki(const Player_Yuki& _other) = delete;
	Player_Yuki(Player_Yuki&& _other) = delete;
	Player_Yuki& operator=(const Player_Yuki& _other) = delete;
	Player_Yuki& operator=(const Player_Yuki&& _other) = delete;
};

