#pragma once
#include "Monsters.h"

// 분류 : 이펙트
// 용도 : 
// 설명 : 멧돼지(몬스터)의 스킬시전개시 시점에 렌더링되는 공격범위
class GameEngineFBXRenderer;
class BoarHitRange : public GameEngineActor
{
public:
	static bool HitRangeMeshLoad;

public: // Public Inline Get Function
public: // Public Inline Set Function
	inline void SetHitRangeLength(float _HitRnageLength)
	{
		CurHitRnageLength_ = _HitRnageLength;
	}

public: // Public Initalize Related Function
	void InitalizeHitRnage();

public: // Public Function
	void SkillAttackRangeOn(float _Length, const float4& _Dir);
	void SkillAttackRangeOff();

protected:

private: // Private HitRange Length Calc Function
	void CalcHitRangeArrowCount(float _Length);
	void UpdateHitRangeArrow();

private: // Private Initalize Function
	void LoadHitRangeMesh();
	void CreateHitRangeRenderer();

public:
	BoarHitRange();
	~BoarHitRange();

protected:
	BoarHitRange(const BoarHitRange& _other) = delete;
	BoarHitRange(BoarHitRange&& _other) noexcept = delete;

private:
	BoarHitRange& operator=(const BoarHitRange& _other) = delete;
	BoarHitRange& operator=(const BoarHitRange&& _other) = delete;

public:
protected:
private:
	GameEngineFBXRenderer* Frame_;						// 직선테두리
	GameEngineFBXRenderer* Bottom_;						// 직선바닥
	std::vector<GameEngineFBXRenderer*> Arrows_;		// 직선화살표갯수

private:
	float PrevHitRangeLength_;							// 이전에 수신된 히트박스 길이
	int PrevArrowCount_;								// 이전에 생성한 화살표갯수

private:
	float CurHitRnageLength_;							// 현재 수신된 히트박스 길이
	int CurArrowCount_;									// 현재 히트박스 길이에 따른 화살표 갯수
};

// 분류 : 몬스터
// 용도 : 
// 설명 : 멧돼지(몬스터)의 고유 기능을 제공
class GameEngineFBXRenderer;
class Boar : public Monsters
{
public:
	static bool ResourceLoadFlag;

protected:
	void InitalizeStateInfo() override;
	void InitalizeResourceLoad() override;
	void InitalizeRenderAndAnimation() override;
	void InitalizeCollider() override;

protected:
	void SkillAttackProcessing(float _DeltaTime) override;

private:
public:
	Boar();
	~Boar();

protected:
	Boar(const Boar& _other) = delete;
	Boar(Boar&& _other) noexcept = delete;

private:
	Boar& operator=(const Boar& _other) = delete;
	Boar& operator=(const Boar&& _other) = delete;

public:
protected:
private:
	bool SkillAtkReady_;								// 스킬공격준비 처리
	bool SkillAtk_;										// 스킬공격 처리
	float SkillAtk_Range_;								// 스킬공격 사정거리
	float SkillAtk_CastTimeMax_;						// 스킬공격 시전시간 최대치(고정)
	float SkillAtk_CastTime_;							// 스킬공격 시전시간(갱신)
	float SkillAtk_FixedDamage_;						// 스킬공격 고정데미지(고정)
	float SkillAtk_RushSpeed_;							// 스킬공격 돌진속도(고정)

private:
	float4 SkillAtk_DetectTargetPos_;					// 최초 스킬준비시 감지된 타겟의 위치
	float4 SkillAtk_DetectTargetDir_;					// 최초 스킬준비시 감지된 타겟을 향한 방향

private:
	BoarHitRange* SkillAtk_HitRnage_;					// 히트박스 렌더러
};

