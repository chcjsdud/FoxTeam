#pragma once
#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>
#include "IUnit.h"

class GameEngineEffectRenderer;
class RioDSkillEffect : public GameEngineActor
{
public:
	RioDSkillEffect(); // default constructer ����Ʈ ������
	~RioDSkillEffect(); // default destructer ����Ʈ �Ҹ���
	RioDSkillEffect(const RioDSkillEffect& _other) = delete; // default Copy constructer ����Ʈ ���������
	RioDSkillEffect(RioDSkillEffect&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	RioDSkillEffect& operator=(const RioDSkillEffect& _other) = delete; // default Copy operator ����Ʈ ���� ������
	RioDSkillEffect& operator=(const RioDSkillEffect&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void PlayAwake();
	void PlayShot();

public:
	virtual void Start() override;
	virtual void Update(float _deltaTime);

protected:
	void startSleep();
	void updateSleep(float _deltaTime);

	void startAwake();
	void updateAwake(float _deltaTime);

	void startShot();
	void updateShot(float _deltaTime);

private:
	GameEngineFSM renderState_;
	GameEngineEffectRenderer* zoneRenderer_;
	GameEngineEffectRenderer* impactRenderer_;
	GameEngineEffectRenderer* targetRenderer_;
	GameEngineCollision* collision_;

private:
	float timer_;
	float rotationContainer_;

	float4 impactScaleContainer_;
	float4 impactOriginalScale_;
	float4 targetScaleContainer_;
	float4 targetOriginalScale_;

	float shotTimer_;
};
