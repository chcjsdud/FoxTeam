#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include <GameEngine/GameEngineFBXRenderer.h>
#include "PJW_Player.h"

class GameEngineCollision;
class PJW_Hyunwoo : public PJW_Player
{

public:
	PJW_Hyunwoo(); 
	~PJW_Hyunwoo(); 
	PJW_Hyunwoo(const PJW_Hyunwoo& _other) = delete;
	PJW_Hyunwoo(PJW_Hyunwoo&& _other) noexcept;
	PJW_Hyunwoo& operator=(const PJW_Hyunwoo& _other) = delete;
	PJW_Hyunwoo& operator=(const PJW_Hyunwoo&& _other) = delete;


protected: 
	virtual void Start();
	void Init_FBX();
	void Init_FSM();
	void Init_Collision();

	virtual void Update(float _DeltaTime);
	void Check_Collision(float _DeltaTime);
	void Check_HP();
	void Check_DebugRect(float _DeltaTime);

// FSM
private:
	GameEngineFSM hyunwooState_;

// States 함수
	void Idle_Start();
	void Idle_Update(float _DeltaTime);
	void Idle_End();

	void Move_Start();
	void Move_Update(float _DeltaTime);
	void Move_End();

	void Attack_Start();
	void Attack_Update(float _DeltaTime);
	void Attack_End();

	void Skill_Q_Start();
	void Skill_Q_Update(float _DeltaTime);
	void Skill_Q_End();

	void Death_Start();
	void Death_Update(float _DeltaTime);
	void Death_End();


// FBX 렌더러
private:
	GameEngineFBXRenderer* FBXRenderer_;
	float4 destination_;
	float4 direction_;

// 충돌체



};

