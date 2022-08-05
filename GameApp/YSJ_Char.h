#pragma once
#include "LH_Player.h"

// 분류 : 캐릭터
// 용도 : 유키
// 설명 : 고유 스킬 및 패턴을 관리
class YSJ_Char : public Player
{
private:

public:
	YSJ_Char();
	~YSJ_Char();

private:
	//FBX Renderer
	GameEngineFBXRenderer* FBXRenderer_;
	GameEngineCollision* AttackHitBoxCollision_;

private:		//delete operator
	void Start() override;
	void Update(float _DeltaTime) override;

	// 렌더러, 콜리전, 등등 Componenet 초기화
	void ComponenetInit();
	//State 초기화
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
	YSJ_Char(const YSJ_Char& _other) = delete;
	YSJ_Char(YSJ_Char&& _other) = delete;
	YSJ_Char& operator=(const YSJ_Char& _other) = delete;
	YSJ_Char& operator=(const YSJ_Char&& _other) = delete;
};

