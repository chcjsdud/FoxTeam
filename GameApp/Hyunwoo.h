#pragma once

#include "Character.h"

class Hyunwoo : public Character
{
public:
	Hyunwoo(); 
	~Hyunwoo();

	Hyunwoo(const Hyunwoo& _other) = delete; 
	Hyunwoo(Hyunwoo&& _other) = delete; 

	Hyunwoo& operator=(const Hyunwoo& _other) = delete;
	Hyunwoo& operator=(const Hyunwoo&& _other) = delete;


protected:
	// Character��(��) ���� ��ӵ�
	virtual void initRendererAndAnimation() override;
	virtual void changeAnimationRun() override;
	virtual void changeAnimationWait() override;
	virtual void changeAnimationBasicAttack() override;

};

