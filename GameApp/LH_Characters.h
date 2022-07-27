#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

#include "Enums.h"
#include "Status.h"

struct SkillInfo
{
	// skilltype(패시브, 공격, ...)
	// 소모자원(sp)
	// 쿨타임
	// 사거리
	// 효과
};

// 분류 : 
// 용도 : 캐릭터 최상위 클래스
// 설명 : 모든 캐릭터의 커맨드 패턴 및 기본정보를 관리 07.26 이현 : 몬스터 포함
// 플레이어블 캐릭터는 Player.h

class Characters : public GameEngineActor
{
public:
	Characters();
	~Characters();

protected:		// delete constructer
	Characters(const Characters& _other) = delete;
	Characters(Characters&& _other) = delete;
	Characters& operator=(const Characters& _other) = delete;
	Characters& operator=(const Characters&& _other) = delete;

private: // 생성정보
	CharacterType CharacterType_;		// (0: AI, 1: MAIN) : default(AI)

protected: // 기본정보
	GameEngineFSM State_;
	//버프 리스트, 
	std::map <std::string, Status*> BufferList_;
	// 캐릭터 기본 스텟
	Status Status_Base_;
	// 캐릭터 추가 스텟 / 덧
	Status Status_Add_;
	// 캐릭터 추가 스텟 / 곱
	Status Status_Mult_;
	// 캐릭터 최종 스텟
	Status Status_Final_;

	//키 방향 07.26, 조작으로 인해 움직여야할 방향, 
	float4 KeyDir_;

	//락온된 대상의 방향, 움직일 방향보단 바라봐야할 방향에 가까움
	float4 TargetDir_;

	//바라봐야 할 방향
	float4 FowordDir_;

	//현재 바라보는 방향
	float4 CurFowordDir_;

	//Y축 회전 속도
	float YRotateSpeed_;

	bool IsMove_;

	//공격 텀
	float AttackTurm_;
	//공격 에니메이션 단계
	int AttackLevel_;
	//공격 시간, 이 시간 이내에 공격키를 누르면 다음 단계 공격을 함
	float AttackTime_;
	//Attack Hit Box 활성화 시간
	float AttackHitTime_;

	GameEngineActor* Target_;


private: // 현재 메인플레이어 지정
	static Characters* MainPlayer;
	 //다른 관리자 클레스가 들고 있는게 나을거 같음


public:
	static Characters* GetMainPlayer();
	static void SetMainPlayer(Characters* _Player);

private: // 스킬정보
	SkillInfo SkillInfo_[6];			// 스킬(패시브, Q, W, E, R, D)
										// 패시브, Q~R 고유 스킬
										// D 스킬은 무기의 스
private: // 커맨드 패턴
// 1. 메인캐릭터 시야 밖에서의 패턴(이동-파밍 패턴)
// 2. 메인캐릭터 시야 범위내에서의 패턴(이동-공격 or 공격-회피)

protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

protected: 
	// 캐릭터 State, 가상함수를 통해 캐릭터 클레스가 기본적으로 가지고 있어야 할 state를 지정
	virtual void Idle_Start() = 0;
	virtual void Idle_Update(float _DeltaTime) = 0;
	virtual void Idle_End() = 0;

	virtual void Walk_Start() = 0;
	virtual void Walk_Update(float _DeltaTime) = 0;
	virtual void Walk_End() = 0;

	virtual void Run_Start() = 0;
	virtual void Run_Update(float _DeltaTime) = 0;
	virtual void Run_End() = 0;

	virtual void Attack_Start() = 0;
	virtual void Attack_Update(float _DeltaTime) = 0;
	virtual void Attack_End() = 0;

	// 스텟에 변화가 생길때마다 적용함
protected: 

	void StateInit();
	void SyncStatus();


public:
	inline void SetCharacterType(CharacterType _Type)
	{
		// 잘못된 타입 수신시 실패
		if (_Type == CharacterType::NONE || _Type == CharacterType::MAX)
		{
			return;
		}

		CharacterType_ = _Type;
	}

	std::map < std::string, Status*> PlayerGetBuffList()
	{
		return BufferList_;
	}

	const Status PlayerGetBaseStat()
	{
		return Status_Base_;
	}

	const Status PlayerGetAddStat()
	{
		return Status_Add_;
	}

	const Status PlayerGetMultStat()
	{
		return Status_Mult_;
	}

	const Status PlayerGetFinalStat()
	{
		return Status_Final_;
	}

	const int PlayerGetCurHP()
	{
		return Status_Final_.Stat_Health_;
	}

	const float PlayerGetCurStamina()
	{
		return Status_Final_.Stat_Stamina_;
	}

	GameEngineActor* PlayerGetTarget()
	{
		return Target_;
	}

	const int PlayerGetAttackPower()
	{
		return Status_Final_.Stat_AttackPower_;
	}

	void SetTarget(GameEngineActor* _Target)
	{
		Target_ = _Target;
	}

protected: // 각 캐릭터 고유 정보 및 스킬셋팅
	//virtual void Initialize() = 0;

protected: // 각 캐릭터 커맨드 부여


};
