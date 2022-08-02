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

protected:		// delete constructer
	Player_Yuki(const Player_Yuki& _other) = delete;
	Player_Yuki(Player_Yuki&& _other) noexcept = delete;

private:		//delete operator
	Player_Yuki& operator=(const Player_Yuki& _other) = delete;
	Player_Yuki& operator=(const Player_Yuki&& _other) = delete;

private:
#pragma region Player component
	//�ٴ� �ݸ���
	GameEngineCollision* PlayerGroundCollision_;
	// RockOn �浹ü
	GameEngineCollision* PlayerLockOnCollision_;
	//�ǰ� ��Ʈ�ڽ�
	GameEngineCollision* PlayerHitBoxCollision_;
	//���� ��Ʈ�ڽ�
	GameEngineCollision* PlayerAttackHitBoxCollision_;
	//FBX Renderer
	GameEngineFBXRenderer* FBXRenderer_;
#pragma endregion

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	// ������, �ݸ���, ��� Componenet �ʱ�ȭ
	void ComponenetInit()override;
	//State �ʱ�ȭ
	void StateInit() override;
	void KeyInit() override;
	void UIInit() override;

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

private:
	void BaseInfoSetting();
	void SkillInfoSetting();

public: // ��Ű �������� �� ��ų���� ����
	//void Initialize() override;

};

