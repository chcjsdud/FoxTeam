#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

#include "Enums.h"
#include "LH_Status.h"

//#include "AIController.h"

class GameEngineCollision;
class GameEngineFBXRenderer;
class Unit : public GameEngineActor
{
public:
	Unit();
	~Unit();

protected: // 기본정보

	bool Iscontrolled_;

	//AIController AIController_;
	GameEngineFSM AIController_;

	CharacterType CharacterType_;		// (0: AI, 1: MAIN) : default(AI)

	GameEngineFSM State_;


	//버프 리스트, 
	std::map <std::string, Buff*> BufferList_;
	// 캐릭터 기본 스텟
	Status Status_Base_;
	// 캐릭터 추가 스텟 / 덧
	Status Status_Add_;
	// 캐릭터 추가 스텟 / 곱
	Status Status_Mult_;
	// 캐릭터 최종 스텟
	Status Status_Final_;



#pragma region 조작 변수
	//키 방향 07.26, 조작으로 인해 움직여야할 방향, 
	float4 KeyDir_;

	//락온된 대상의 방향, 움직일 방향보단 바라봐야할 방향에 가까움
	float4 TargetDir_;
	float4 TargetPos_;
	bool IsAttack_;

	bool EnemyInSight_; //시야에 적이 있음

#pragma endregion

	// 유닛의 이동 방향, 유닛이 바라봐야 할 방향
	float4 FowordDir_;
	// 유닛이 현재 바라보는 방향 (이동과 무관함)
	float4 CurFowordDir_;
	//Y축 회전 속도
	float YRotateSpeed_;

	// 사거리
	float AttackDist_;
	//공격 텀, 공격 속도
	float AttackTurm_;
	// 히트박스 활성화 전에 뜸들이는 시간 (에니메이션)
	float AttackBuffer_;
	//Attack Hit Box 활성화 시간
	float AttackHitTime_;

	GameEngineActor* Target_;

	//바닥 콜리전
	GameEngineCollision* UnitGroundCollision_;
	//시야 충돌체
	GameEngineCollision* UnitSightCollision_;
	//피격 히트박스
	GameEngineCollision* UnitHitBoxCollision_;

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

protected:
	void SyncStatus();

	const float4 CalculateTargetDir(float4 TargetPos)
	{
		float4 Dir = TargetPos - GetTransform()->GetWorldPosition();
		Dir.Normalize3D();

		return Dir;
	}

protected:
	void UpdateBuff(float _DeltaTime);

	void CurDirUpdate(float _DeltaTime);
	void TargetDirUpdate(float _DeltaTime);
	//RockOn 중일때 방향 업데이트
	//void RockOnDirUpdate(float _DeltaTime);
	//void KeyDirUpdate(float _DeltaTime);
	void MoveUpdate(float _DeltaTime);

	//캐릭터가 바라보는 방향이 바로 바로 안변하고 천천히 변함(이동과는 무관하게), 
//07.18 바라보는 방향을 넣어줘야함
	void MoveRotateUpdate(float _DeltaTime);

protected:
	// 캐릭터 State, 가상함수를 통해 캐릭터 클레스가 기본적으로 가지고 있어야 할 state를 지정
	virtual void Idle_Start() = 0;
	virtual void Idle_Update(float _DeltaTime) = 0;
	virtual void Idle_End() = 0;

	virtual void Walk_Start() = 0;
	virtual void Walk_Update(float _DeltaTime) = 0;
	virtual void Walk_End() = 0;

	//virtual void Run_Start() = 0;
	//virtual void Run_Update(float _DeltaTime) = 0;
	//virtual void Run_End() = 0;

	virtual void Attack_Start() = 0;
	virtual void Attack_Update(float _DeltaTime) = 0;
	virtual void Attack_End() = 0;

	// 공격과 공격 사이 에니메이션 잠깐 재생하는 구간, 공격이 완료된 직후이며, 이 단계에서 Attack를 끝낼지 게속할지 판별한다.
	virtual void Attack_Ready_Start() = 0;
	virtual void Attack_Ready_Update(float _DeltaTime) = 0;
	virtual void Attack_Ready_End() = 0;

public:
	void AddBaseStat(Status _Status);
	void SetBaseStat(Status _Status);
	void AddBuff(std::string _Name, Status _Status, float _Time);
	void RemoveBuff(std::string _Name);
	void RemoveAllBuff(std::string _Name);


public:

	void Unit_AIController_Off()
	{
		Iscontrolled_ = false;
		//AIController_->AIController_Off();
	}
	void Unit_AIController_On()
	{
		Iscontrolled_ = true;
		//AIController_->AIController_On();
	}

	void Unit_SetTargetDir(float4 _Dir)
	{
		TargetDir_ = _Dir;
	}

	void Unit_SetTargetPos(float4 _Pos)
	{
		TargetPos_ = _Pos;
	}

	void Unit_ChangeState(std::string _StateName)
	{
		State_.ChangeState(_StateName);
	}

	//void Unit_SetMove(bool _IsMove)
	//{
	//	IsMove_ = _IsMove;
	//}

	std::string Unit_GetStateName()
	{
		return State_.GetCurrentState()->Name_;
	}

	inline void Unit_SetCharacterType(CharacterType _Type)
	{
		// 잘못된 타입 수신시 실패
		if (_Type == CharacterType::NONE || _Type == CharacterType::MAX)
		{
			return;
		}

		CharacterType_ = _Type;
	}

	std::map < std::string, Buff*> PlayerGetBuffList()
	{
		return BufferList_;
	}

	const Status Unit_GetBaseStat()
	{
		return Status_Base_;
	}

	const Status Unit_GetAddStat()
	{
		return Status_Add_;
	}

	const Status Unit_GetMultStat()
	{
		return Status_Mult_;
	}

	const Status Unit_GetFinalStat()
	{
		return Status_Final_;
	}

	const int Unit_GetCurHP()
	{
		return Status_Final_.Stat_Health_;
	}

	const float Unit_GetCurStamina()
	{
		return Status_Final_.Stat_Stamina_;
	}

	GameEngineActor* Unit_GetTarget()
	{
		return Target_;
	}

	const int Unit_GetAttackPower()
	{
		return Status_Final_.Stat_AttackPower_;
	}

	void Unit_SetTarget(GameEngineActor* _Target)
	{
		Target_ = _Target;
	}

protected:		// delete constructer
	Unit(const Unit& _other) = delete;
	Unit(Unit&& _other) = delete;
	Unit& operator=(const Unit& _other) = delete;
	Unit& operator=(const Unit&& _other) = delete;
};
