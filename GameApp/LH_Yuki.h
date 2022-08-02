#pragma once
#include "LH_Player.h"

// 분류 : 캐릭터
// 용도 : 유키
// 설명 : 고유 스킬 및 패턴을 관리
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
	//바닥 콜리전
	GameEngineCollision* PlayerGroundCollision_;
	// RockOn 충돌체
	GameEngineCollision* PlayerLockOnCollision_;
	//피격 히트박스
	GameEngineCollision* PlayerHitBoxCollision_;
	//공격 히트박스
	GameEngineCollision* PlayerAttackHitBoxCollision_;
	//FBX Renderer
	GameEngineFBXRenderer* FBXRenderer_;
#pragma endregion

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	// 렌더러, 콜리전, 등등 Componenet 초기화
	void ComponenetInit()override;
	//State 초기화
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

public: // 유키 고유정보 및 스킬정보 셋팅
	//void Initialize() override;

};

