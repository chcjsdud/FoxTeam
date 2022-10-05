#pragma once

#include <GameEngine/GameEngineActor.h>
#include "Character.h"
#include <GameEngine/GameEngineFSM.h>

class GameEngineFBXRenderer;
class LumiaMap;
class Rio : public Character
{
public:
	Rio();
	~Rio();
	Rio(const Rio& _other) = delete;
	Rio(Rio&& _other) = delete;
	Rio& operator=(const Rio& _other) = delete;
	Rio& operator=(const Rio&& _other) = delete;

public:
	static void LoadResource();
	static void ReleaseResource();

public:
	virtual JobType GetJobType() { return JobType::RIO; }

protected:
	void Start() override;
	void Update(float _deltaTime) override;

	// Character을(를) 통해 상속됨
	virtual void initRendererAndAnimation() override;
	virtual void changeAnimationWait() override;
	virtual void changeAnimationRun() override;
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


	virtual void onStartDeath() override;
	virtual void onUpdateDeath(float _deltaTime) override;

};