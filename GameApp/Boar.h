#pragma once
#include "Monsters.h"

// 분류 : 몬스터
// 용도 : 
// 설명 : 멧돼지(몬스터)의 고유 기능을 제공
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
	float SkillAtk_Range_;								// 스킬공격 사정거리
	float SkillAtk_CastTime_;							// 스킬공격 시전시간
};

