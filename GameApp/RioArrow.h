#pragma once

#include <GameEngine/GameEngineActor.h>
#include <GameEngine/GameEngineFSM.h>

class GameEngineFBXRenderer;
class GameEngineCollision;
class Character;
class RioArrow : public GameEngineActor
{
public:
	RioArrow(); 
	~RioArrow();

	RioArrow(const RioArrow& _other) = delete; 
	RioArrow(RioArrow&& _other) = delete; 

	RioArrow& operator=(const RioArrow& _other) = delete;
	RioArrow& operator=(const RioArrow&& _other) = delete;

public:
	virtual void Start();
	virtual void Update(float _deltaTime);

public:
	void MakeTargetArrow(Character& _owner, float _damage, const float4& _position, float _speed, Character& _target);
	void MakeNonTargetArrow(Character& _owner, float _damage, const float4& _position, float _rotationY, float _speed);
	void SetWaitTime(float _waitTime) { waitTime_ = _waitTime; }

private:
	void startWait();
	void updateWait(float _deltaTime);

	void startChase();
	void updateChase(float _deltaTime);

	void startFly();
	void updateFly(float _deltaTime);
	
	void setRotationTo(const float4& _destination, const float4 _startPosition);

private:
	GameEngineFSM state_;
	GameEngineFBXRenderer* renderer_;
	GameEngineCollision* collision_;

	Character* owner_;
	Character* target_;

	float damage_;
	float rotationY_;
	float speed_;

	float waitTime_;
};

