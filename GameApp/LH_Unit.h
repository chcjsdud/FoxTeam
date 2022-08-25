#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include "Enums.h"

#include "Controller.h"

#include "LH_Status.h"

#include "Unit_Packet.h"
#include "LH_PlayLevel.h"

//TODO: 체크 리스트 (이현)
// 
// 1. 패킷 디자인
// 2. 유닛, 플레이어 분리작업
// 3. 아이템 인벤토리구현
// 4. 네비 완성시, 이동함수
// 5. 이동 함수 완성시, 공격함수도 같이
// 
// 6. 네비랑 무관하게 완성시 바로 대입 가능한 수준의 이동, 공격함수 완성
// 7. 스킬은 각자 알아서 구현, 스킬 사용 명령만 전달하는 ㅁ[쏘드 구현 근데 이게 패시브인지 엑티브인지 정도는 내가
// 8. 스킬 구조체를 만들고 엑티브 패시브, 즉발, 타겟, 범위, 직선, 원형, 대미지, 사거리 등등 걍 다 넣어둘까?
// 9. 정지, 홀드, 휴식 구현
// 10. 

// 플레이어 컨트롤러로 부터 키 조작에 의한 여러 값들을 전달받아 그대로 행동함
// 플레이어로 옮겨야 할 부분이 많은데 일단 완성후 할것

// 함수 명명
//
// 1. 설정 == Set
// 2. 추가 == Add
//
// 3. (클래스)_(Set||Add||Get)(대상)(행동)
//
// 4. (이름)(Update)
//


enum class Unit_Team
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

class PlayLevel;
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
	void Unit_AddBuff(BuffType _BuffType, Status _Status, float _Time = -1.f, bool _IsSturn = false, std::function<void()> _BuffFunc = nullptr); //_Time(-1.f) == 영구지속
	void Unit_RemoveBuff(BuffType _BuffType);
	void Unit_RemoveAllBuff();

	//void Unit_AddBuff(std::string _Name, Status _Status, float _Time = -1.f, bool _IsSturn = false, std::function<void()> _BuffFunc = nullptr); //_Time(-1.f) == 영구지속
	//void Unit_RemoveBuff(std::string _Name);
	//void Unit_RemoveAllBuff();

public:
	const unsigned int Unit_GetUnitID()
	{
		return UnitID_;
	}

	Unit_Packet* Unit_Get_UnitUpdatePaket()
	{
		return &Unit_Packet_;
	}

	const Unit_Team Unit_GetTeam()
	{
		return Unit_Team_;
	}

	void Unit_SetTeam(Unit_Team _Unit_Team)
	{
		Unit_Team_ = _Unit_Team;
	}

	std::map <BuffType, Status_Buff*> Unit_GetPlayerBuffList()
	{
		return Unit_BufferList_;
	}

	const Status Unit_GetBaseStat()
	{
		return Unit_Status_Base_;
	}

	const Status Unit_GetAddStat()
	{
		return Unit_Status_Add_;
	}

	const Status Unit_GetMultStat()
	{
		return Unit_Status_Mult_;
	}

	const Status Unit_GetFinalStat()
	{
		return Unit_Status_Final_;
	}

	const int Unit_GetCurHP()
	{
		return Unit_Status_Final_.Stat_Health_;
	}

	const float Unit_GetCurStamina()
	{
		return Unit_Status_Final_.Stat_Stamina_;
	}

	Unit* Unit_GetTarget()
	{
		return Target_Unit_;
	}

	const int Unit_GetAttackPower()
	{
		return Unit_Status_Final_.Stat_AttackPower_;
	}

	void Unit_SetTarget(Unit* _Target)
	{
		Target_Unit_ = _Target;
	}

	void Unit_SetUnitID(unsigned int _ID)
	{
		UnitID_ = _ID;
	}

#pragma endregion

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

protected:
#pragma region 초기화
	virtual void Unit_Set_State_Init();
	void Unit_Set_Collision_Init();

#pragma endregion
public:
	unsigned int UnitID_; // 아직 초기화 빼고 관련 함수 없음
	Unit_Packet Unit_Packet_;

	//서버만이 Actor를 생성할 경우에만 유효함,

protected: // 기본정보
	Unit_Team Unit_Team_; //컬리전으로 적의 타입을 알아낸 후, 적인지 아군인지 판별함

	GameEngineFSM OrderState_;
	GameEngineFSM ActionState_;

	GameEngineFBXRenderer* FBXRenderer_;
	//바닥 콜리전
	GameEngineCollision* UnitGroundCollision_;
	//시야 충돌체
	GameEngineCollision* UnitSightCollision_;
	//피격 히트박스
	GameEngineCollision* UnitHitBoxCollision_;

	Controller* Controller_; //TODO: 플레이어 헤더로 이동할것, 몬스터에게 명령을 하달하는 존재가 생겨나면 바로 옮겨도됨
#pragma region 컨트롤러 에서 얻어오는 변수들

	float4 Target_Pos_; // 마우스로 지정된 최종적으로 이동해야 할 위치
	Unit* Target_Unit_; // 마우스로 타겟팅한 유닛
	Controller_Order Controller_Order_; // 컨트롤러에서 전달된 명령
	//unsigned int Target_ID_; //타겟이 된 유닛의 ID 확실히 엔진 단위의 기능이 필요함,
#pragma endregion

#pragma region 이동 변수

	float4 Move_Navi_Temp_Pos_;	// 네비가 전해주는 임시 이동 위치
	float4 Move_ForwardDir_;	// 유닛의 이동 방향, 유닛이 바라봐야 할 방향	
	float4 Move_Rotate_CurForwardDir_;	// 유닛이 현재 바라보는 방향 (이동과 무관함)	
	float Move_YRotateSpeed_;	//Y축 회전 속도
#pragma endregion

#pragma region 스텟 변수

	//std::map <std::string, Status_Buff*> Unit_BufferList_;	//버프 리스트, 

	std::map <BuffType, Status_Buff*> Unit_BufferList_;	//임시 구현상태, 

	Status Unit_Status_Base_;		// 캐릭터 기본 스텟
	Status Unit_Status_Add_;			// 캐릭터 추가 스텟 / 덧
	Status Unit_Status_Mult_;		// 캐릭터 추가 스텟 / 곱	

	Status Unit_Status_Final_;		// 캐릭터 최종 스텟

	float Unit_AttackTurm_;	//공격 텀, 공격 속도
	float Unit_AttackBuffer_;	// 히트박스 활성화 전에 뜸들이는 시간 (에니메이션) 	
	float Unit_AttackHitTime_;	//Attack Hit Box 활성화 시간
#pragma endregion

#pragma region 유닛 컨트롤 변수

	std::list<Unit*> TargetingUnits_; // 나를 타겟한 유닛들, 나를 타겟한 유닛이 아직도 날 타겟팅 했는지 매번 체크해야한다.
									// 소멸자 호출시, 이 리스트 인원 전부의 Target_Unit_을 null로 만듦
	bool EnemyInSight_; //시야에 적이 있음

	bool IsSturn_;	// 스턴 상태
	bool OrderUpdate_;	// 명령 가능상태
#pragma endregion

protected:
#pragma region 업데이트
	void Unit_TargetUpdate(float _DeltaTime);
	virtual void Unit_StateUpdate(float _DeltaTime);
	void Unit_UpdateBuff(float _DeltaTime);

	//TODO: 구현 덜됨
	void Unit_Set_Receive_PaketUpdate();
	void Unit_Set_Send_PaketUpdate();

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

	void Unit_SetOrderEnd();
	void Unit_SetSyncStatus();

	void Unit_SetChangeActionState(std::string _State)
	{
		if (ActionState_.GetCurrentState()->Name_ != _State)
		{
			ActionState_.ChangeState(_State);
		}
	}
	
	void Unit_SetChangeOrderState(std::string _State)
	{
		if (OrderState_.GetCurrentState()->Name_ != _State)
		{
			OrderState_.ChangeState(_State);
		}
	}

	const float4 GetTargetDir(float4 TargetPos)
	{
		float4 Dir = TargetPos - GetTransform()->GetWorldPosition();
		Dir.Normalize3D();

		return Dir;
	}

	void Unit_SetTargetErase()
	{
		Target_Unit_ = nullptr;
	}

	void Unit_SetUnitTarget(Unit* _Unit)
	{
		Target_Unit_ = _Unit;

		if (_Unit != nullptr)
		{
			Target_Unit_->TargetingUnits_.push_back(this);
		}
	}

protected:
#pragma region 명령 State
	virtual void Order_Idle_Start();
	virtual void Order_Idle_Update(float _DeltaTime);
	virtual void Order_Idle_End();

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

	virtual void Order_Rest_Start();
	virtual void Order_Rest_Update(float _DeltaTime);
	virtual void Order_Rest_End();

	virtual void Order_Reload_Start();
	virtual void Order_Reload_Update(float _DeltaTime);
	virtual void Order_Reload_End();

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

	virtual void Action_Stop_Start() ;
	virtual void Action_Stop_Update(float _DeltaTime) ;
	virtual void Action_Stop_End() ;

	virtual void Action_Hold_Start();
	virtual void Action_Hold_Update(float _DeltaTime);
	virtual void Action_Hold_End();

	virtual void Action_Rest_Start();
	virtual void Action_Rest_Update(float _DeltaTime);
	virtual void Action_Rest_End();

	//TODO: 이 밑부분은 나중에 플레이어.h 로 이동하게 될 것이다.

#pragma endregion

protected:		// delete constructer
	Unit(const Unit& _other) = delete;
	Unit(Unit&& _other) = delete;
	Unit& operator=(const Unit& _other) = delete;
	Unit& operator=(const Unit&& _other) = delete;
};
