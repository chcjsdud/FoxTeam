#pragma once
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineEffectRenderer.h>
#include "Character.h"


class HyunwooQEffect;
class HyunwooREffect;
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
	void initHyunwooCustomState();
	void initEffectRenderer();


	virtual void changeAnimationRun() override;
	virtual void changeAnimationWait() override;
	virtual void changeAnimationBasicAttack() override;
	virtual void changeDeathAnimation() override;

	virtual void onStartBasicAttacking(IUnit* _target) override;
	virtual void onUpdateBasicAttacking(IUnit* _target, float _deltaTime) override;

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


	virtual void onPlayEffect(const std::string& _effectName, IUnit* _victim = nullptr) override;
	virtual void onEffectTransformCheck(float _deltaTime) override;

	// customstate
	void startCustomRSkill();
	void updateCustomRSkill(float _deltaTime);
	void endCustomRSkill();

private:
	HyunwooQEffect* qEffect_;
	HyunwooREffect* rEffect_;
	
	GameEngineEffectRenderer* basicAttackEffectRenderer_;
	GameEngineEffectRenderer* QGroundCrackEffectRenderer_;
	GameEngineEffectRenderer* rearEffectRenderer_;


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


	bool b_Rhit_;
	
	GameEngineCollision* collision_R;
	float collisionRRate_;

	bool b_Dhit_;
};
