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
	// Character을(를) 통해 상속됨
	virtual void initRendererAndAnimation() override;


	virtual void changeAnimationRun() override;
	virtual void changeAnimationWait() override;
	virtual void changeAnimationBasicAttack() override;


	virtual void onStartQSkill() override;
	virtual void onUpdateQSkill(float _deltaTime) override;

	virtual void onStartWSkill() override;
	virtual void onUpdateWSkill(float _deltaTime) override;

	virtual void onStartESkill() override;
	virtual void onUpdateESkill(float _deltaTime) override;

	virtual void onStartRSkill() override;
	virtual void onUpdateRSkill(float _deltaTime) override;

	virtual void onStartDSkill() override;
	virtual void onUpdateDSkill(float _deltaTime) override;

	// 0922 박종원

public:
	void InitHyunwooCollision();


private:
	float timer_collision_Q;
	float timer_end_Q;
	bool b_Qhit_;
	GameEngineCollision* collision_Q;

	
	
};

