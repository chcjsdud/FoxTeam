#include "PreCompile.h"
#include "AyaBullet.h"

#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "Character.h"
#include "PacketSoundPlay.h"
#include "Monsters.h"

AyaBullet::AyaBullet()
	: renderer_(nullptr)
	, collision_(nullptr)
	, target_(nullptr)
	, damage_(0.0f)
	, rotationY_(0.0f)
	, speed_(0.0f)
	, waitTime_(0.0f)
	, owner_(nullptr)
	, lifeTime_(0.5f)
	, scale_(float4(5.f, 150.f, 50.f))
	, bKnockback_(false)
	, effect_(nullptr)
{
}

AyaBullet::~AyaBullet()
{
}

void AyaBullet::Start()
{
	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->SetCollisionGroup(eCollisionGroup::Projectile);
	collision_->SetCollisionType(CollisionType::OBBBox3D);
	collision_->Off();

	GetTransform()->SetLocalScaling(scale_);

	state_.CreateState(MakeState(AyaBullet, Chase));
	state_.CreateState(MakeState(AyaBullet, Fly));
	state_.CreateState(MakeState(AyaBullet, Wait));

	state_ << "Wait";
}

void AyaBullet::Update(float _deltaTime)
{
	if (collision_->IsUpdate())
	{
		level_->PushDebugRender(GetTransform(), CollisionType::OBBBox3D);
	}

	state_.Update(_deltaTime);
}

void AyaBullet::MakeTarget(IUnit& _owner, float _damage, const float4& _position, float _speed, IUnit& _target)
{
	owner_ = &_owner;
	target_ = &_target;
	damage_ = _damage;
	transform_.SetWorldPosition(_position);
	speed_ = _speed;

	float4 destination = target_->GetTransform()->GetWorldPosition();
	destination.y += 120.f;
	float4 direction = destination - transform_.GetWorldPosition();
	direction.Normalize3D();

	setRotationTo(destination, transform_.GetWorldPosition());
}

void AyaBullet::MakeNonTarget(IUnit& _owner, float _damage, const float4& _position, float _rotationY, float _speed)
{
	owner_ = &_owner;
	target_ = nullptr;
	damage_ = _damage;
	transform_.SetWorldPosition(_position);
	speed_ = _speed;
	rotationY_ = _rotationY;
}

void AyaBullet::startWait()
{
}

void AyaBullet::updateWait(float _deltaTime)
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

void AyaBullet::startChase()
{
	collision_->On();
}

void AyaBullet::updateChase(float _deltaTime)
{
	float4 destination = target_->GetTransform()->GetWorldPosition();
	destination.y += 120.f;
	float4 direction = destination - transform_.GetWorldPosition();
	direction.Normalize3D();

	setRotationTo(destination, transform_.GetWorldPosition());

	transform_.AddWorldPosition(direction * speed_ * _deltaTime);

	if (float4::Calc_Len3D(destination, transform_.GetWorldPosition()) <= FT::Char::MOVE_FINISH_CHECK_DISTANCE)
	{
		if (damage_ > 0.0f)
		{
			//GameEngineSoundManager::GetInstance()->PlaySoundByName("Rio_ShortBow_Hit_01.wav");
			//PacketSoundPlay packet;
			//packet.SetSound("Rio_ShortBow_Hit_01.wav", transform_.GetWorldPosition());
			//FT::SendPacket(packet);
			target_->Damage(damage_, owner_);
		}

		Release();
	}
}

void AyaBullet::startFly()
{
	transform_.SetLocalRotationDegree({ 0.0f, rotationY_, 0.0f });
	collision_->On();
}

void AyaBullet::updateFly(float _deltaTime)
{
	lifeTime_ -= _deltaTime;
	if (lifeTime_ < 0)
	{
		Release();
		return;
	}

	transform_.AddWorldPosition(transform_.GetWorldForwardVector() * speed_ * _deltaTime);

	auto collisionList = collision_->GetCollisionList(eCollisionGroup::Player);
	for (GameEngineCollision* col : collisionList)
	{
		if (col->GetActor() != owner_)
		{
			Character* opponent = dynamic_cast<Character*>(col->GetActor());

			if (opponent != nullptr)
			{
				opponent->Damage(damage_, owner_);

				// sound

				Release();
				return;
			}
		}
	}

	collisionList.clear();
	collisionList = collision_->GetCollisionList(eCollisionGroup::Monster);
	for (GameEngineCollision* col : collisionList)
	{
		if (col->GetActor() != owner_)
		{
			Monsters* opponent = dynamic_cast<Monsters*>(col->GetActor());

			if (opponent != nullptr)
			{
				opponent->Damage(damage_, owner_);

				// sound

				Release();
				return;
			}
		}
	}
}

void AyaBullet::setRotationTo(const float4& _destination, const float4 _startPosition)
{
	float4 direction = _destination - _startPosition;
	direction.Normalize3D();

	float4 cross = float4::Cross3D(direction, { 0.0f, 0.0f, 1.0f });
	cross.Normalize3D();

	float angle = float4::DegreeDot3DToACosAngle(direction, { 0.0f, 0.0f, 1.0f });

	transform_.SetLocalRotationDegree({ 0.0f, angle * -cross.y, 0.0f });
}