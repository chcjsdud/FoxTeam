#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include "PlayerController.h"

#include "Enums.h"
#include "LH_Status.h"

// 플레이어 컨트롤러로 부터 키 조작에 의한 여러 값들을 전달받아 그대로 행동함
// 플레이어로 옮겨야 할 부분이 많은데 일단 완성후 할것

// 함수 명명
//
// 1. 설정 == Set
// 2. 추가 == Add
//
// 3. (Set||Add||Get)(대상)(행동)
//
// 4. (이름)(Update)
//


class GameActorUpdatePaket
{
	//각자 계산하고 최종적으로 반영되어야 할 결과값만 패킷으로 보내자

	unsigned int ObjectID_;
	//std::string Name_;
	//float4 Pos_;
	//TransformData TransformData_;

	float4 vWorldPosition_;
	float4 vWorldRotation_;
	float4 vWorldScaling_;

	float4 vLocalPosition_;
	float4 vLocalRotation_;
	float4 vLocalScaling_;


	int Size_;

	const int GetPaketSize()
	{
		Size_ = sizeof(GameActorUpdatePaket);
	}


};



	// * 계획
	// 1. 플레이어 컨트롤러는 서버와 무관하게 각자 컴퓨터가 생성 (문제 없을듯, 되려 키 인풋 때문에 편함)
	// 2. 서버가 플레이어 캐릭터를 생성후, 클라이언트로 부터 패킷을 받아 클라이언트의 플레이어 컨트롤러에 캐릭터 배분
	// 3. 클라이언트가 서버로 동기화 데이터 패킷을 보냄
	// 4. 서버가 패킷을 받고 게산후, 다시 클라이언트로 쏴줌
	// 5. 반복
	// 
	// 서버에 패킷을 보내야 하는 이유
	// 1. 동기화 시키기 위해,
	// 2. 모든 컴퓨터들의 델타 타임이 다르기 때문에 모두 맞춰줄 필요가 있음
	// 3. 
	// 
	// 서버 패킷으로 보낼것
	//
	//	0. 델타타임의 영향을 받을 모든것
	//	1. 현재 유닛의 아이디
	//	2. 서버에게 나 이만큼 이동해줘! 라고 보내기 (서버가 _deltaTime 계산후, 이동할 값 다시 보내줄것)
	//	2. 트랜스폼 데이터
	//	3. IsDeath
	//	4. 에니메이션 String
	//	5. 캐릭터 스탯 뭉텅이들
	//
	// 서버에게 받아야 할것
	//	
	//	1. 유닛 아이디(매칭시켜야 하니까) 
	//	2. 서버가 계산한 이동 데이터
	//	3. 서버가 계산한 트렌스폼 데이터
	//	4. 
	//
	//	서버가 해줘야 할것
	//	1. 서버의 델타타임에 맞게 계산해서 다시 쏴주기
	//
	// 엔진이 가져야 할 기능
	//
	//	1. 모든 엑터들이 고유의 ID를 가지게 할것
	//	2. 함수를 통해 모든 엑터들의 고유 ID를 알 수 있게 할것
	// 
	//	3. ID로 부터 엑터 포인터들을 가져와 사용할 수 있게 할것
	// 
	//	4. static unsigned int IDs_ 같은걸 만들고, 엑터들이 생성 될때마다 ++ 하면서 각기 고유ID를 매겨줄것 (중복 생성시 오류)
	//	5. 충돌 함수가 충돌한 대상의 ID 가져오기
	//	6. ID는 1부터 시작하게끔, (0은 null 취급)
	//

enum class Team
{
	None,
	Team1,
	Team2,
	Team3,
	Team4,
	Team5,
	Team6,
	Monster
};

class GameEngineCollision;
class GameEngineFBXRenderer;
class Unit : public GameEngineActor
{
	friend class PlayerController;

public:
	Unit();
	~Unit();


#pragma region Public
public:
	void Unit_AddBaseStat(Status _Status);
	void Unit_SetBaseStat(Status _Status);
	void Unit_AddBuff(std::string _Name, Status _Status, float _Time = -1.f, bool _IsSturn = false, std::function<void()> _BuffFunc = nullptr); //_Time(-1.f) == 영구지속
	void Unit_RemoveBuff(std::string _Name);
	void Unit_RemoveAllBuff(std::string _Name);

public:
	const Team Unit_GetTeam()
	{
		return Team_;
	}

	void Unit_SetTeam(Team _Team)
	{
		Team_ = _Team;
	}

	std::map < std::string, Buff*> Unit_GetPlayerBuffList()
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
		return Target_Unit_;
	}

	const int Unit_GetAttackPower()
	{
		return Status_Final_.Stat_AttackPower_;
	}

	void Unit_SetTarget(Unit* _Target)
	{
		Target_Unit_ = _Target;
	}

	//const unsigned int GetUnitID()
	//{
	//	return UnitID_;
	//}
	//void Unit_ChangeState(std::string _StateName)
	//{
	//	State_.ChangeState(_StateName);
	//}

	//std::string Unit_GetStateName()
	//{
	//	return State_.GetCurrentState()->Name_;
	//}

	//inline void Unit_SetCharacterType(CharacterType _Type)
	//{
	//	// 잘못된 타입 수신시 실패
	//	if (_Type == CharacterType::NONE || _Type == CharacterType::MAX)
	//	{
	//		return;
	//	}

	//	CharacterType_ = _Type;
	//}
#pragma endregion

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

protected:
#pragma region 초기화

#pragma endregion

protected: // 기본정보

	class PlayerController* PlayController_; // ==nullptr ==몬스터
	Team Team_; //적인지 아군인지 판별함

	GameEngineFSM OrderState_;
	GameEngineFSM ActionState_;

	//바닥 콜리전
	GameEngineCollision* UnitGroundCollision_;
	//시야 충돌체
	GameEngineCollision* UnitSightCollision_;
	//피격 히트박스
	GameEngineCollision* UnitHitBoxCollision_;

#pragma region 컨트롤러 에서 얻어오는 변수들

	float4 Target_Pos_; // 마우스로 지정된 최종적으로 이동해야 할 위치
	Unit* Target_Unit_; // 마우스로 타겟팅한 유닛
	Order Controller_Order_; // 컨트롤러에서 전달된 명령
	//unsigned int Target_ID_; //타겟이 된 유닛의 ID 확실히 엔진 단위의 기능이 필요함,
#pragma endregion

#pragma region 이동 변수

	float4 Move_Navi_Temp_Pos_;	// 네비가 전해주는 임시 이동 위치
	float4 Move_ForwardDir_;	// 유닛의 이동 방향, 유닛이 바라봐야 할 방향	
	float4 Move_Rotate_CurForwardDir_;	// 유닛이 현재 바라보는 방향 (이동과 무관함)	
	float Move_YRotateSpeed_;	//Y축 회전 속도
#pragma endregion

#pragma region 스텟 변수

	std::map <std::string, Buff*> BufferList_;	//버프 리스트, 	
	Status Status_Base_;		// 캐릭터 기본 스텟
	Status Status_Add_;			// 캐릭터 추가 스텟 / 덧
	Status Status_Mult_;		// 캐릭터 추가 스텟 / 곱	
	Status Status_Final_;		// 캐릭터 최종 스텟

	float AttackTurm_;	//공격 텀, 공격 속도
	float AttackBuffer_;	// 히트박스 활성화 전에 뜸들이는 시간 (에니메이션) 	
	float AttackHitTime_;	//Attack Hit Box 활성화 시간
#pragma endregion

#pragma region 유닛 컨트롤 변수

	std::list<Unit*> TargetingUnits_; // 나를 타겟한 유닛들, 나를 타겟한 유닛이 아직도 날 타겟팅 했는지 매번 체크해야한다.
									// 소멸자 호출시, 이 리스트 인원 전부의 Target_Unit_을 null로 만듦
	bool EnemyInSight_; //시야에 적이 있음

	bool IsSturn_;	// 스턴 상태
#pragma endregion

protected:
#pragma region 업데이트
	void Controller_Update();
	void UnitTargetUpdate(float _DeltaTime);
	void StateUpdate(float _DeltaTime);
	void UpdateBuff(float _DeltaTime);
#pragma endregion

#pragma region 무브 함수
	void MoveUpdate(float _DeltaTime);

	void NaviUpdate(float _DeltaTime);
	void CurDirUpdate(float _DeltaTime);
	void MoveRotateUpdate(float _DeltaTime);

	bool ChaseTargetUpdate(Unit* _Target_Unit, float _ChaseDist);
	//리턴값이 true면 추적을 종료한다.
	bool ChasePosUpdate(float4 _Target_Pos, float _ChaseDist);
#pragma endregion


	void SetOrderEnd();

	//리턴값이 true면 추적을 종료한다.

protected:
	void SetChangeActionState(std::string _State)
	{
		if (ActionState_.GetCurrentState()->Name_ != _State)
		{
			ActionState_.ChangeState(_State);
		}
	}
	
	void SetChangeOrderState(std::string _State)
	{
		if (OrderState_.GetCurrentState()->Name_ != _State)
		{
			OrderState_.ChangeState(_State);
		}
	}

	void SetSyncStatus();

	const float4 GetTargetDir(float4 TargetPos)
	{
		float4 Dir = TargetPos - GetTransform()->GetWorldPosition();
		Dir.Normalize3D();

		return Dir;
	}

	void SetTargetErase()
	{
		Target_Unit_ = nullptr;
	}

	void SetUnitTarget(Unit* _Unit)
	{
		Target_Unit_ = _Unit;
		Target_Unit_->TargetingUnits_.push_back(this);
	}

	//void SetTargetID(unsigned int _Target_ID)
	//{
	//	Target_ID_ = _Target_ID;
	//}

protected:
#pragma region 명령 State
	virtual void Order_Attack_Target_Start();
	virtual void Order_Attack_Target_Update(float _DeltaTime);
	virtual void Order_Attack_Target_End();

	virtual void Order_Attack_Pos_Start();
	virtual void Order_Attack_Pos_Update(float _DeltaTime);
	virtual void Order_Attack_Pos_End();

	virtual void Order_Move_Start();
	virtual void Order_Move_Update(float _DeltaTime);
	virtual void Order_Move_End();

	virtual void Order_Stop_Start();
	virtual void Order_Stop_Update(float _DeltaTime);
	virtual void Order_Stop_End();

	virtual void Order_Hold_Start();
	virtual void Order_Hold_Update(float _DeltaTime);
	virtual void Order_Hold_End();

	//virtual void Order_Q_Start();
	//virtual void Order_Q_Update(float _DeltaTime);
	//virtual void Order_Q_End();

	//virtual void Order_W_Start();
	//virtual void Order_W_Update(float _DeltaTime);
	//virtual void Order_W_End();

	//virtual void Order_E_Start();
	//virtual void Order_E_Update(float _DeltaTime);
	//virtual void Order_E_End();

	//virtual void Order_R_Start();
	//virtual void Order_R_Update(float _DeltaTime);
	//virtual void Order_R_End();

	//virtual void Order_D_Start();
	//virtual void Order_D_Update(float _DeltaTime);
	//virtual void Order_D_End();

#pragma endregion

#pragma region 동작 State
	virtual void Action_Idle_Start();
	virtual void Action_Idle_Update(float _DeltaTime);
	virtual void Action_Idle_End();

	virtual void Action_Walk_Start();
	virtual void Action_Walk_Update(float _DeltaTime);
	virtual void Action_Walk_End();

	virtual void Action_Attack_Start();
	virtual void Action_Attack_Update(float _DeltaTime);
	virtual void Action_Attack_End();

	virtual void Action_Q_Start()=0;
	virtual void Action_Q_Update(float _DeltaTime) = 0;
	virtual void Action_Q_End() = 0;

	virtual void Action_W_Start() = 0;
	virtual void Action_W_Update(float _DeltaTime) = 0;
	virtual void Action_W_End() = 0;

	virtual void Action_E_Start() = 0;
	virtual void Action_E_Update(float _DeltaTime) = 0;
	virtual void Action_E_End() = 0;

	virtual void Action_R_Start() = 0;
	virtual void Action_R_Update(float _DeltaTime) = 0;
	virtual void Action_R_End() = 0;

	virtual void Action_D_Start() = 0;
	virtual void Action_D_Update(float _DeltaTime) = 0;
	virtual void Action_D_End() = 0;
#pragma endregion

protected:		// delete constructer
	Unit(const Unit& _other) = delete;
	Unit(Unit&& _other) = delete;
	Unit& operator=(const Unit& _other) = delete;
	Unit& operator=(const Unit&& _other) = delete;
};
