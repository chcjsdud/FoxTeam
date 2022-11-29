#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 유닛베이스
// 용도 : 
// 설명 : 

class IUnit : public GameEngineActor
{
public:
	virtual int GetIndex() = 0; // 캐릭터를 구분지을 ID 를 얻는다.

	virtual void Damage(float _amount, IUnit* _subject) = 0; // 데미지를 받는다.
	virtual void Knockback(float _knockbackTime, float4 _knockbackSpeed) = 0; // 넉백
	virtual void WallSlam(float _knockbackTime, float4 _knockbackSpeed, float _stunTime) = 0; // 넉백. 벽에 부딪힐 경우 스턴
	virtual void Slow(float _slowTime, float _slowRatio) = 0; // 느려짐
	virtual void Stun(float _stunTime) = 0; // 기절

public:
	UnitType UnitType_;
};

