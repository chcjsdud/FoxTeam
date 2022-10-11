#pragma once

#include <GameEngine/GameEnginePacketBase.h>
#include "Character.h"

class PacketCreateProjectile : public GameEnginePacketBase
{
public:
	PacketCreateProjectile(); 
	~PacketCreateProjectile();

	PacketCreateProjectile(const PacketCreateProjectile& _other) = delete; 
	PacketCreateProjectile(PacketCreateProjectile&& _other) = delete; 

	PacketCreateProjectile& operator=(const PacketCreateProjectile& _other) = delete;
	PacketCreateProjectile& operator=(const PacketCreateProjectile&& _other) = delete;

public:
	void MakeTargetArrow(Character& _owner, float _damage, const float4& _position, float _speed, Character& _target);
	void MakeNonTargetArrow(Character& _owner, float _damage, const float4& _position, float _rotationY, float _speed);
	
	void SetWaitTime(float _waitTime) { waitTime_ = _waitTime; }
	void SetLifeTime(float _lifeTime) { lifeTime_ = _lifeTime; }
	void SetScale(const float4& _scale) { scale_ = _scale; }

protected:
	virtual void initPacketID() override;
	virtual void userSerialize() override;
	virtual void userDeserialize() override;
	virtual GameEnginePacketBase* getUserObject() override;
	virtual void execute(SOCKET _socketSender, GameEngineSocketInterface* _network, bool _bServer) override;

private:
	float4 position_;
	float4 scale_;
	float damage_;
	float speed_;
	float rotationY_;
	float waitTime_;
	float lifeTime_;
	int ownerIndex_;
	int targetIndex_;
};

