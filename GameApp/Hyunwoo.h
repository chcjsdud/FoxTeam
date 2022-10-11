#pragma once
#include <GameEngine/GameEngineRenderer.h>
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

public:
	static void LoadResource();
	static void ReleaseResource();

public:
	virtual JobType GetJobType() { return JobType::HYUNWOO; }

protected:
	void Start() override;
	void Update(float _deltaTime) override;

protected:
	// Character��(��) ���� ��ӵ�
	virtual void initRendererAndAnimation() override;
	void initHyunwooCollision();


	virtual void changeAnimationRun() override;
	virtual void changeAnimationWait() override;
	virtual void changeAnimationBasicAttack() override;

	virtual void onStartBasicAttacking(Character* _target) override;
	virtual void onUpdateBasicAttacking(Character* _target, float _deltaTime) override;

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

	virtual void onStartDeath() override;
	virtual void onUpdateDeath(float _deltaTime) override;

	virtual void onUpdateCustomState(float _deltaTime) override;

private:
	bool atkFlag_;

	float timer_collision_Q;
	float timer_end_Q;
	bool b_Qhit_;
	GameEngineCollision* collision_Q;


	float timer_Dash_E;		// �뽬 ���ӽð�
	GameEngineCollision* collision_E;
	bool b_Ehit_;

	// ĳ������ ������ ���¸� ó���ϰ� ���� �� ���
	GameEngineFSM customState_;
};
