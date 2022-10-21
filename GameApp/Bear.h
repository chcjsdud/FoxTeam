#pragma once
#include "Monsters.h"

// 분류 : 몬스터
// 용도 : 
// 설명 : 곰(몬스터)의 고유 기능을 제공
class Bear : public Monsters
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
	Bear();
	~Bear();

protected:
	Bear(const Bear& _other) = delete;
	Bear(Bear&& _other) noexcept = delete;

private:
	Bear& operator=(const Bear& _other) = delete;
	Bear& operator=(const Bear&& _other) = delete;

public:
protected:
private:
	float SkillAtk_Range_;								// 스킬공격 사정거리
	float SkillAtk_CastTime_;							// 스킬공격 시전시간

private:
	float4 PivotPos_;									// 렌더러의 피벗 조정값
};

