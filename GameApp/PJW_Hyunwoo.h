#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include <GameEngine/GameEngineFBXRenderer.h>

class GameEngineCollision;
class PJW_Hyunwoo : public GameEngineActor
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
	virtual void Update(float _DeltaTime);

	void Init_FBX();
	void Init_FSM();
	void Init_Collision();

// FSM
private:
	GameEngineFSM hyunwooState_;

// States 함수
	void Idle_Start();
	void Idle_Update(float _DeltaTime);
	void Idle_End();

	//void Move_Start();
	//void Move_Update(float _DeltaTime);
	//void Move_End();

	void Skill_Q_Start();
	void Skill_Q_Update(float _DeltaTime);
	void Skill_Q_End();

// FBX 렌더러
private:
	GameEngineFBXRenderer* FBXRenderer_;


// 충돌체
private:
	GameEngineCollision* collision_Body_;
	// 몸통 콜리젼

	GameEngineCollision* collision_Qskill_; 
	// Q 스킬 시 피격 거리 콜리젼

	GameEngineCollision* collision_Rskill;
	// R 궁극기 피격 거리 콜리젼
};

