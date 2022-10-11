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

public:
	bool IsLongBow() { return bLongBow_; }

protected:
	void Start() override;
	void Update(float _deltaTime) override;

	// Character��(��) ���� ��ӵ�
	virtual void initRendererAndAnimation() override;
	virtual void changeAnimationWait() override;
	virtual void changeAnimationRun() override;
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
	void startSkillEBegin();
	void updateSkillEBegin(float _deltaTime);

	void startSkillEShot();
	void updateSkillEShot(float _deltaTime);

	void startSkillEEnd();
	void updateSkillEEnd(float _deltaTime);

private:
	GameEngineCollision* skillECollision_;
	GameEngineFSM customState_;
	bool bLongBow_;

	bool bSkillEPassable_;



};