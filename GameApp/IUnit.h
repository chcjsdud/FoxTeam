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
};

