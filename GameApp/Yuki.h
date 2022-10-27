#pragma once
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineImageRenderer.h>
#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineEffectRenderer.h>
#include "Character.h"

// 분류 : 
// 용도 : 
// 설명 : 
class YukiREffect;
class Yuki : public Character
{	
public:
	Yuki(); // default constructer 디폴트 생성자
	~Yuki(); // default destructer 디폴트 소멸자
	Yuki(const Yuki& _other) = delete; // default Copy constructer 디폴트 복사생성자
	Yuki(Yuki&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	Yuki& operator=(const Yuki& _other) = delete; // default Copy operator 디폴트 대입 연산자
	Yuki& operator=(const Yuki&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자


public:
	static void LoadResource();
	static void ReleaseResource();

public:
	virtual JobType GetJobType() { return JobType::YUKI; }

protected:
	void Start() override;
	void Update(float _deltaTime) override;

protected:
	// Character을(를) 통해 상속됨
	virtual void initRendererAndAnimation() override;
	void initYukiCollision();
	void initYukiCustomState();
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

	// customstate
	void startCustomRStandBy();
	void updateCustomRStandBy(float _deltaTime);
	void endCustomRStandBy();

	void startCustomRSlash();
	void updateCustomRSlash(float _deltaTime);
	void endCustomRSlash();

	void startCustomRAfterBurst();
	void updateCustomRAfterBurst(float _deltaTime);
	void endCustomRAfterBurst();

private:
	GameEngineFBXRenderer* swordRenderer_;

	GameEngineEffectRenderer* basicAttackEffectRenderer_;
	GameEngineEffectRenderer* groundEffectRenderer_;

	GameEngineEffectRenderer* rearEffectRenderer_;


	bool atkFlag_;

	bool b_isQ_;
	float timer_Q;
	bool b_Qhit_;

	float timer_Dash_E;		// 대쉬 지속시간
	GameEngineCollision* collision_E;
	bool b_Ehit_;

	YukiREffect* rEffect_;
	GameEngineFSM customState_;
	GameEngineCollision* collision_R;
	float timer_R;
	bool b_RHit_;
};