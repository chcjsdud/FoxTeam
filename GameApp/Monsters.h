#pragma once
#include <GameEngine/GameEngineFSM.h>

#include "IUnit.h"
#include "MonsterStateInfo.h"

// 분류 : 몬스터
// 용도 : 기본(부모)클래스
// 설명 : 현재 맵에 배치되는 몬스터들의 공통정보
class GameEngineFBXRenderer;
class GameEngineCollision;
class NavFace;
class NavMesh;
class LumiaMap;
class Character;
class Monsters : public IUnit
{
public: // Static Value
	
public: // inline Get Function
	inline MonsterStateBasicType GetMonsterBasicStateType()
	{
		return CurStateBasicType_;
	}

	inline MonsterStateType GetMonsterDetailStateType()
	{
		return CurStateType_;
	}

	inline MonsterStateInfo GetMonsterStateInfo()
	{
		return StateInfo_;
	}

	inline Location GetMonsterAreaType()
	{
		return AreaType_;
	}

	inline MonsterType GetMonsterType()
	{
		return Type_;
	}

public: // inline Set Function
	inline void SetMonsterIndex(int _Index)
	{
		Index_ = _Index;
	}

	inline void SetMonsterAreaType(Location _AreaType)
	{
		AreaType_ = _AreaType;
	}

public:
	int GetIndex() override;
	void Damage(float _amount) override {}

public: // Initalize Function
	void InitalizeSpawnPosition(const float4& _SpawnPosition);				// 인자로 수신한 _SpawnPosition을 해당 몬스터의 스폰위치로 지정(클라이언트전용)

public: // public ChangeAnimation Function
	void ChangeAnimationAndState(MonsterStateType _StateType);				// 몬스터상태타입으로 애니메이션 및 상태 전환

public:
protected: // Protected Pure Virtual Function
	virtual void InitalizeStateInfo() = 0;									// 몬스터 상태정보 초기화
	virtual void InitalizeResourceLoad() = 0;								// 최초 리소스 로드
	virtual void InitalizeRenderAndAnimation() = 0;							// 렌더러 & 애니메이션 생성
	virtual void InitalizeCollider() = 0;									// 충돌체 생성

private: // Private Initalize Function
	void InitalizeFSMState();												// FSM 상태 생성(공통)

private: // Private Update Function
	void CheckMonsterStateInfo(float _DeltaTime);							// 상태정보 체크(상태별 강제상태전환 체크)
	void CheckAllCollision(float _DeltaTime);								// 충돌체 충돌체크

private: // FSM State Function
	//==================================== Main State
	void StartNormalState();												// 일반상태 시작
	void UpdateNormalState(float _DeltaTime);								// 일반상태 갱신
	void EndNormalState();													// 일반상태 종료

	void StartCrowdControlState();											// 군중제어상태 시작
	void UpdateCrowdControlState(float _DeltaTime);							// 군중제어상태 갱신
	void EndCrowdControlState();											// 군중제어상태 종료

	void StartAttackState();												// 공격상태 시작
	void UpdateAttackState(float _DeltaTime);								// 공격상태 갱신
	void EndAttackState();													// 공격상태 종료

	//==================================== Normal State
	void StartAppearState();												// 첫등장상태 시작
	void UpdateAppearState(float _DeltaTime);								// 첫등장상태 갱신
	void EndAppearState();													// 첫등장상태 종료

	void StartRegenState();													// 리젠상태 시작
	void UpdateRegenState(float _DeltaTime);								// 리젠상태 갱신
	void EndRegenState();													// 리젠상태 종료

	void StartIdleState();													// 대기상태 시작
	void UpdateIdleState(float _DeltaTime);									// 대기상태 갱신
	void EndIdleState();													// 대기상태 종료

	void StartRunState();													// 이동상태 시작
	void UpdateRunState(float _DeltaTime);									// 이동상태 갱신
	void EndRunState();														// 이동상태 종료

	void StartHomingInstinctState();										// 귀소본능상태 시작
	void UpdateHomingInstinctState(float _DeltaTime);						// 귀소본능상태 갱신
	void EndHomingInstinctState();											// 귀소본능상태 종료

	void StartChaseState();													// 타겟추적상태 시작
	void UpdateChaseState(float _DeltaTime);								// 타겟추적상태 갱신
	void EndChaseState();													// 타겟추적상태 종료

	//==================================== CrowdControl State
	void StartHitState();													// 피격상태 시작
	void UpdateHitState(float _DeltaTime);									// 피격상태 갱신
	void EndHitState();														// 피격상태 종료

	void StartDeathState();													// 사망중상태 시작
	void UpdateDeathState(float _DeltaTime);								// 사망중상태 갱신
	void EndDeathState();													// 사망중상태 종료

	void StartDeadState();													// 사망(리젠대기)상태 시작
	void UpdateDeadState(float _DeltaTime);									// 사망(리젠대기)상태 갱신
	void EndDeadState();													// 사망(리젠대기)상태 종료

	//==================================== Attack State(조건 : 타겟지정이 되어있을때)
	void StartAttack01State();												// 일반공격1상태 시작
	void UpdateAttack01State(float _DeltaTime);								// 일반공격1상태 갱신
	void EndAttack01State();												// 일반공격1상태 종료

	void StartAttack02State();												// 일반공격2상태 시작
	void UpdateAttack02State(float _DeltaTime);								// 일반공격2상태 갱신
	void EndAttack02State();												// 일반공격2상태 종료

	void StartSkillAttackState();											// 스킬공격상태 시작
	void UpdateSkillAttackState(float _DeltaTime);							// 스킬공격상태 갱신
	void EndSkillAttackState();												// 스킬공격상태 종료

private:
	void Start() override;
	void Update(float _deltaTime) override;

public:
	Monsters();
	virtual ~Monsters() = 0;												// 해당 클래스는 모든 몬스터의 상위(부모) 클래스로 
																			// 반드시 상속받아서 객체화해야하기때문에 순수가상소멸자를 사용

protected:
	Monsters(const Monsters& _other) = delete;
	Monsters(Monsters&& _other) = delete;

private:
	Monsters& operator=(const Monsters& _other) = delete;
	Monsters& operator=(const Monsters&& _other) = delete;

public:
protected: // 컴포넌트
	GameEngineFBXRenderer* MainRenderer_;									// 메인 렌더러
	GameEngineFBXRenderer* EffectRenderer_;									// 이펙트 렌더러
	GameEngineCollision* BodyCollider_;										// 몸체 충돌체(메인 -> 보통 몬스터는 해당 충돌체로 공격박스, 히트박스를 모두 사용)
	GameEngineCollision* AtkCollider_;										// 공격 충돌체(서브 -> 공격 충돌체가 분리되어 필요한 몬스터가 생성)
	GameEngineCollision* TargetCollider_;									// 몬스터를 공격하여 타겟으로 지정된 액터의 충돌체

protected: // 생성정보(탐색용)
	int Index_;																// 생성 인덱스
	MonsterType Type_;														// 몬스터의 타입
	Location AreaType_;														// 최초 스폰 지역 타입(StateInfo_.AppearAreaList_ 중 랜덤으로 선택된 지역)

protected: // 맵 및 길찾기
	NavFace* CurrentNavFace_;												// 현재 면정보
	NavMesh* CurrentNavMesh_;												// 네비메쉬
	LumiaMap* CurrentMap_;													// 현재 몬스터가 배치된 맵
	
protected: // 상태정보(갱신용)
	bool DetectTarget_;														// 타겟지정완료 Flag(true: 타겟지정) -> 공격받았을때 지정
	MonsterStateInfo StateInfo_;											// 몬스터의 상태정보
	float4 Destination_;													// 이동목적지

protected: // FSM State
	GameEngineFSM MainState_;												// 메인상태(기본: NormalState 실행)
	GameEngineFSM NormalState_;												// 기본상태(CC, ATK 후 돌아오는 상태)
	GameEngineFSM CrowdControlState_;										// 군중제어상태(외부 Callback에의해 실행되는 상태)
	GameEngineFSM AttackState_;												// 공격상태(일반공격, 스킬공격 상태를 관리 및 실행)
	
protected: // 타겟
	Character* CurTarget_;													// 현재 몬스터의 타겟(피격시 타겟지정, 귀환상태(귀소본능 수치가 0.0f일때)에서 초기화)

private: // 상태정보
	MonsterStateBasicType CurStateBasicType_;								// 몬스터의 현재상태 기본타입(Normal, CrowdControl, Attack)
	MonsterStateType CurStateType_;											// 몬스터의 현재상태 상세타입(Run, Idle, Hit, ...)
};

