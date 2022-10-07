#include "PreCompile.h"
#include "RioArrow.h"

#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "Character.h"

RioArrow::RioArrow()
	: renderer_(nullptr)
	, collision_(nullptr)
	, target_(nullptr)
	, damage_(0.0f)
	, rotationY_(0.0f)
	, speed_(0.0f)
	, waitTime_(0.0f)
	, owner_(nullptr)
{

}

RioArrow::~RioArrow()
{

}

void RioArrow::Start()
{
	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->SetCollisionGroup(eCollisionGroup::Projectile);
	collision_->SetCollisionType(CollisionType::OBBBox3D);

	transform_.SetLocalScaling(50.f);

	state_.CreateState(MakeState(RioArrow, Chase));
	state_.CreateState(MakeState(RioArrow, Fly));
	state_.CreateState(MakeState(RioArrow, Wait));

	state_ << "Wait";
}

void RioArrow::Update(float _deltaTime)
{
	level_->PushDebugRender(GetTransform(), CollisionType::OBBBox3D);

	state_.Update(_deltaTime);
}

void RioArrow::MakeTargetArrow(Character* _owner, float _damage, const float4& _position, float _speed, Character* _target)
{
	if (nullptr == _owner || nullptr == _target)
	{
		Release();
		return;
	}
	owner_ = owner_;
	target_ = _target;
	damage_ = _damage;
	transform_.SetWorldPosition(_position);
	speed_ = _speed;
}

void RioArrow::MakeNonTargetArrow(Character* _owner, float _damage, const float4& _position, float _rotationY, float _speed)
{
	if (nullptr == _owner)
	{
		Release();
		return;
	}

	owner_ = _owner;
	damage_ = _damage;
	transform_.SetWorldPosition(_position);
	speed_ = _speed;
	rotationY_ = _rotationY;
}

void RioArrow::startWait()
{
}

void RioArrow::updateWait(float _deltaTime)
{
	waitTime_ -= _deltaTime;

	if (waitTime_ > 0.f)
	{
		return;
	}

	if (target_ == nullptr)
	{
		state_ << "Fly";
	}
	else
	{
		state_ << "Chase";
	}
}

void RioArrow::startChase()
{
}

void RioArrow::updateChase(float _deltaTime)
{
	//float4 destination = target_->GetTransform()->GetWorldPosition();
	//destination.y += 50.f;
	//setRotationTo(destination, transform_.GetWorldPosition());

	//transform_.AddWorldPosition(transform_.GetWorldForwardVector() * speed_ * _deltaTime);

	//if (float4::Calc_Len3D(destination, transform_.GetWorldPosition()) < FT::Char::MOVE_FINISH_CHECK_DISTANCE)
	//{
	//	target_->Damage(damage_);
	//	Release();
	//}

	float4 destination = target_->GetTransform()->GetWorldPosition();
	float4 direction = destination - transform_.GetWorldPosition();
	direction.Normalize3D();

	setRotationTo(destination, transform_.GetWorldPosition());

	transform_.AddWorldPosition(direction * _deltaTime);

	if (float4::Calc_Len3D(destination, transform_.GetWorldPosition()) <= FT::Char::MOVE_FINISH_CHECK_DISTANCE)
	{
		target_->Damage(damage_);
		Release();
	}
}

void RioArrow::startFly()
{
}

void RioArrow::updateFly(float _deltaTime)
{
	transform_.AddWorldPosition(transform_.GetWorldForwardVector() * speed_ * _deltaTime);

	auto collisionList = collision_->GetCollisionList(eCollisionGroup::Player);
	for (GameEngineCollision* col : collisionList)
	{
		if (col->GetActor() != owner_)
		{
			Character* opponent = dynamic_cast<Character*>(col->GetActor());

			if (opponent != nullptr)
			{
				opponent->Damage(damage_);
				Release();
				break;
			}
		}
	}
}

void RioArrow::setRotationTo(const float4& _destination, const float4 _startPosition)
{
	float4 direction = _destination - _startPosition;
	direction.Normalize3D();

	float4 cross = float4::Cross3D(direction, { 0.0f, 0.0f, 1.0f });
	cross.Normalize3D();

	float angle = float4::DegreeDot3DToACosAngle(direction, { 0.0f, 0.0f, 1.0f });

	transform_.SetLocalRotationDegree({ 0.0f, angle * -cross.y, 0.0f });
}
