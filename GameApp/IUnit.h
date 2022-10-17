#pragma once
#include <GameEngine/GameEngineActor.h>

// �з� : ���ֺ��̽�
// �뵵 : 
// ���� : 
class IUnit : public GameEngineActor
{
public:
	virtual int GetIndex() = 0;
	virtual void Damage(float _amount ) = 0;
	virtual void Damage(float _Amount, GameEngineActor* _Target) = 0;			// 221017 SJH ADD : ���ʹ� Ÿ���ʿ�

public:
	inline UnitType GetUnitType() const											// 221017 SJH ADD : �ش� ������ Ÿ��(����Ÿ�� ������ ���)
	{
		return UnitType_;
	}

protected:
	UnitType UnitType_;
};

