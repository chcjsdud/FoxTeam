#pragma once
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineEffectRenderer.h>
#include "Character.h"
// �뵵 : 
// ���� : 
class Jackie : public Character
{
public:
	Jackie(); // default constructer ����Ʈ ������
	~Jackie(); // default destructer ����Ʈ �Ҹ���
	Jackie(const Jackie& _other) = delete; // default Copy constructer ����Ʈ ���������
	Jackie(Jackie&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	Jackie& operator=(const Jackie& _other) = delete; // default Copy operator ����Ʈ ���� ������
	Jackie& operator=(const Jackie&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	static void LoadResource();
	static void ReleaseResource();

public:
	virtual JobType GetJobType() { return JobType::JACKIE; }

protected:
	void Start() override;
	void Update(float _deltaTime) override;


protected:
	// Character��(��) ���� ��ӵ�
	virtual void initRendererAndAnimation() override;
	void initJackieCollision();
	void initJackieCustomState();
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


	virtual void onPlayEffect(const std::string& _effectName);

private:
	bool atkFlag_;

	float timer_collision_Q;
	float timer_end_Q;
	bool b_Qhit_;
	GameEngineCollision* collision_Q;


	float timer_collision_E;
	float timer_end_E;
	bool b_Ehit_;
	GameEngineCollision* collision_E;

	GameEngineEffectRenderer* basicAttackEffectRenderer_;
};

