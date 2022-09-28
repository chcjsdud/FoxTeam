#pragma once
#include "Monsters.h"

// 분류 : 몬스터
// 용도 : 
// 설명 : 늑대(몬스터)의 고유 특성을 정의하는 클래스
class Wolf : public Monsters
{
public:
	static bool ResourceLoadFlag;

protected:
	void InitalizeStateInfo() override;
	void InitalizeResourceLoad() override;
	void InitalizeRenderAndAnimation() override;
	void InitalizeCollider() override;

private:

public:
	Wolf();
	~Wolf();

protected:
	Wolf(const Wolf& _other) = delete;
	Wolf(Wolf&& _other) = delete;

private:
	Wolf& operator=(const Wolf& _other) = delete;
	Wolf& operator=(const Wolf&& _other) = delete;

public:
protected:
private:
};

