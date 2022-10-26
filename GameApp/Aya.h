#pragma once

#include <GameEngine/GameEngineActor.h>
#include "Character.h"
#include <GameEngine/GameEngineFSM.h>

class GameEngineFBXRenderer;
class LumiaMap;
class Aya : public Character
{
public:
	Aya();
	~Aya();
	Aya(const Aya& _other) = delete;
	Aya(Aya&& _other) = delete;
	Aya& operator=(const Aya& _other) = delete;
	Aya& operator=(const Aya&& _other) = delete;

public:
	static void LoadResource();
	static void ReleaseResource();

public:


	// Character을(를) 통해 상속됨
	virtual JobType GetJobType() override;

	virtual void initRendererAndAnimation() override;

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

	virtual void onUpdateRSkill(float _d1eltaTime) override;

	virtual void onStartDSkill() override;

	virtual void onUpdateDSkill(float _deltaTime) override;

	virtual void onUpdateCustomState(float _deltaTime) override;

	virtual void onStartDeath() override;

	virtual void onUpdateDeath(float _deltaTime) override;

	virtual void onPlayEffect(const std::string& _effectName) override;

private:

};