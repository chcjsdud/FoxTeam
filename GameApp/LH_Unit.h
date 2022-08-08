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

	bool Iscontrolled_; //유닛(플레이어)가 플레이어 컨트롤러에 의해 컨트롤 되는지 판단하여 AI를 굴림
	/*
	각각의 명령들은 Unit class 안에서 가장 기본적인 틀로 미리 정의되어 있고, 고유 정보를 넣고 싶을땐 override 해서 사용(스킬은 무적권 순수 가상함수로)


	1. 원초 명령 : PlayerControl, AIControll 에서 UI, 단축키, AI알고리즘을 통해 내리는 명령, 일종의 지침

				1. Idle(None)
				2. 우클릭
				3. 좌클릭
				4. A(Attack)
				5. S(Stop(멈춰!, 명령 취소))
				6. H(Hold)
				7. P(Patrrol)

				8~ . Skill 1~
					스킬은 스킬 사거리, 타겟, 논타겟 여부를 구분함

	2. 단계 명령 : 원초 명령을 받아 그것을 실행하기 위한 단계, 원초 명령을 스스로 해석하고 엑션을 취하기 위한 계산(번역)을 함
					단계 명령에서 원초 명령에 걸맞는 동작 명령을 내림
					원초 명령을 동작 명령으로 전달하는 해석 단계

					각자 플레이어 캐릭터마다 고유의 스킬 단계 명령을 구현할 수 있게 해야함
					각각의 플레이어 캐릭터마다 고유의 공격 알고리즘이나 그런걸 지닐 수도 있는걸 감안해야함
					때려죽여도 원초 명령을 어차피 동일할것으로, 단계 명령은 각 캐릭터마다 오버라이딩 해서 사용할 수 있게함(오버라이드 안하면 그냥 기존꺼 사용)

					0. 단계 명령을 업데이트 하는중, CC기를 맞으면 기존 명령을 취소(Stop)함 

					1. Idle(None)
					2. Detect (적이 있는가 탐지함)
					3. Chase (공격 원초 명령이 내려 왔을때, 적을 탐지하면 추적함 (사거리 안까지 Walk 동작 명령 수행))
								(스킬 원초 명령이 내려왔을때, 타겟 지정인지 논 타겟인지 구분)
								(타겟 지정일 경우 적을 탐지하면 추적함 (사거리 안까지 Walk 동작 명령 수행))
								(논타겟이면 바로 스킬 단계로 넘어감)

					4-1. Attack (추적이 완료되었으면 공격함(Attack 동작 명령 수행)
								(Attack 동작 명령 수행 후, 원초 명령이 Attack 상태면 Chase 상태로 돌아감(적이 움직이지 않았으면 다시 Attack으로 돌아올것))
								(추적 완료후, Attack 을 수행할 시점에 적이 사망해 있거나 거리를 벗어나면 Chase or Idle 로 돌아감)
								(Stop이나 다른 원초 명령이 내려지면 그에 걸맞는 단계 명령 수행)

					4-2. Skill (추적이 완료되었으면(논 타겟이거나) 그에 걸맞는 스킬 동작 명령을 수행, 스킬이 끝났는지 판단하여 걸맞는 명령 시행) 

					5. Sturn (CC기에 의해 Sturn에 걸린 상태, Sturn 동작 명령 시행후, 시간이 지나면 None 상태로 되돌림)

					예) 원초 명령에서 어떤 Actor를 우클릭 하면 그게 적인지 아군인지 뭔지 판단해서 그것에 걸맞는 동작 명령을 내림

					예) 공격 원초 명령중에 대상이 사라졌거나 대상이 죽었을경우, 동작 명령을 Idle로 전환하고, 원초 명령을 취소(Stop)함

					예) 캐릭터마다 다른 스킬 알고리즘을 원할경우, 단계명령에서 동작 명령을 결정하는 함수를 오버라이딩 해서 사용

	3. 동작 명령 : Idle, Walk, Attack, Sturn 같은 세부 동작을 수행하는 단계,
					각각의 동작들은 서로 독립적임 
					각기 캐릭터마다 고유한 함수를 원할경우, 오버라이드 해서 각기 구현 가능





		모든 유닛은 같은 명령 단계를 공유함
		AI는 AI 컨트롤러가, 플레이어는 플레이어 컨트롤러가 명령을 내림

		각기 플레이어마다 스킬의 종류와 알고리즘이 다르기 때문에, 그 경우 각자 따로 추가하여 사용할 수 있게 해야함

		AI 컨트롤러도 부모 클레스로 설계해서 각자 상속 받아서 자기만의 컨트롤러를 지니도록 해야함

		플레이어 컨트롤러는 그냥 단축키 같은것에 의한 명령만 내려줄 뿐이니 중요치 않음


		구태여 상속을 이용하고 통일된 인터페이스를 만들고자 하는 이유는 다수의 독립된 플레이어 클레스가 존재할때 알고리즘을 통일하여 서로 충돌, ㅂ그를 방지하기 위함




	설계 의도 : 각각의 행동, 동작들에 독립성을 부여하여 다른곳에서 가져다 쓰기 용이하게 만듦
				

	디버깅 단계에서 현재 내려진 명령, 명령을 수행하기 위해 취하는 단계, 실제 엑션 등등을 가시적으로 보여줌
	
	
	
	*/


	//AIController AIController_;
	GameEngineFSM Primal_Order_; // 플레이어, AI등이 내리는 가장 원초적 명령단계, UI, 단축키 등을 통해 Attack, Skill, Stop, Hold, Move, Patrrol 등의 명령
	GameEngineFSM Process_Order_; // 원초 명령을 수행하기 위한 단계적 명령, 공격을 위한 추적, 탐지, 
	GameEngineFSM Action_Order_; // 원초 명령을 수행하기 위한 동작 단계, Attack 동작, Move 동작, Idle 동작 

	//GameEngineFSM AIController_;

	GameEngineFSM State_;


	CharacterType CharacterType_;		// (0: AI, 1: MAIN) : default(AI)

#pragma region 조작 변수
	//키 방향 07.26, 조작으로 인해 움직여야할 방향, 
	float4 KeyDir_;

	//락온된 대상의 방향, 움직일 방향보단 바라봐야할 방향에 가까움
	float4 TargetDir_;
	float4 TargetPos_;
	bool IsAttack_;

	bool EnemyInSight_; //시야에 적이 있음

#pragma endregion

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

	bool IsSturn_;
	float SturnTime_;

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

	Unit* Target_;

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

	Unit* Unit_GetTarget()
	{
		return Target_;
	}

	const int Unit_GetAttackPower()
	{
		return Status_Final_.Stat_AttackPower_;
	}

	void Unit_SetTarget(Unit* _Target)
	{
		Target_ = _Target;
	}

protected:		// delete constructer
	Unit(const Unit& _other) = delete;
	Unit(Unit&& _other) = delete;
	Unit& operator=(const Unit& _other) = delete;
	Unit& operator=(const Unit&& _other) = delete;
};
