#include "PreCompile.h"
#include "RioArrow.h"

#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "Character.h"
#include "PacketSoundPlay.h"
#include "Monsters.h"

RioArrow::RioArrow()
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

RioArrow::~RioArrow()
{

}

void RioArrow::Start()
{
	//effect_ = CreateTransformComponent<GameEngineEffectRenderer>();
	//effect_->SetImage("FX_BI_TX_RioShootFire.png", "LinearSmp");
	//effect_->GetTransform()->SetLocalRotationDegree({ 90.f,90.f,0.f });
	//effect_->GetTransform()->SetLocalScaling(effect_->GetCurrentTexture()->GetTextureSize() / 2);
	//effect_->SetAlpha(0.5f);

	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->SetCollisionGroup(eCollisionGroup::Projectile);
	collision_->SetCollisionType(CollisionType::OBBBox3D);
	collision_->Off();

	GetTransform()->SetLocalScaling(scale_);

	state_.CreateState(MakeState(RioArrow, Chase));
	state_.CreateState(MakeState(RioArrow, Fly));
	state_.CreateState(MakeState(RioArrow, Wait));

	state_ << "Wait";
}

void RioArrow::Update(float _deltaTime)
{
	if (collision_->IsUpdate())
	{
		level_->PushDebugRender(GetTransform(), CollisionType::OBBBox3D);
	}

	state_.Update(_deltaTime);
}

void RioArrow::MakeTargetArrow(IUnit& _owner, float _damage, const float4& _position, float _speed, IUnit& _target)
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

void RioArrow::MakeNonTargetArrow(IUnit& _owner, float _damage, const float4& _position, float _rotationY, float _speed)
{
	owner_ = &_owner;
	target_ = nullptr;
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
	GameEngineSoundManager::GetInstance()->PlaySoundByName("Rio_ShortBow_NormalAttack_01.wav");
	PacketSoundPlay packet;
	packet.SetSound("Rio_ShortBow_NormalAttack_01.wav", transform_.GetWorldPosition());
	FT::SendPacket(packet);
	collision_->On();
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
	destination.y += 120.f;
	float4 direction = destination - transform_.GetWorldPosition();
	direction.Normalize3D();

	setRotationTo(destination, transform_.GetWorldPosition());

	transform_.AddWorldPosition(direction * speed_ * _deltaTime);

	if (float4::Calc_Len3D(destination, transform_.GetWorldPosition()) <= FT::Char::MOVE_FINISH_CHECK_DISTANCE)
	{
		if (damage_ > 0.0f)
		{
			GameEngineSoundManager::GetInstance()->PlaySoundByName("Rio_ShortBow_Hit_01.wav");
			PacketSoundPlay packet;
			packet.SetSound("Rio_ShortBow_Hit_01.wav", transform_.GetWorldPosition());
			FT::SendPacket(packet);
			target_->Damage(damage_, owner_);
		}

		Release();
	}
}

void RioArrow::startFly()
{
	transform_.SetLocalRotationDegree({ 0.0f, rotationY_, 0.0f });
	collision_->On();

	if (bKnockback_)
	{
		GameEngineSoundManager::GetInstance()->PlaySoundByName("Rio_LongBow_Skill04_Projectile.wav");
		PacketSoundPlay packet;
		packet.SetSound("Rio_LongBow_Skill04_Projectile.wav", transform_.GetWorldPosition());
		FT::SendPacket(packet);
	}
}

void RioArrow::updateFly(float _deltaTime)
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

				if (bKnockback_)
				{
					GameEngineSoundManager::GetInstance()->PlaySoundByName("Rio_LongBow_Skill04_Hit.wav");
					PacketSoundPlay packet;
					packet.SetSound("Rio_LongBow_Skill04_Hit.wav", transform_.GetWorldPosition());
					opponent->WallSlam(0.25f, transform_.GetWorldForwardVector() * 1000.f, 0.0f);
					FT::SendPacket(packet);
				}
				else
				{
					GameEngineSoundManager::GetInstance()->PlaySoundByName("Rio_ShortBow_Skill02_Hit.wav");
					PacketSoundPlay packet;
					packet.SetSound("Rio_ShortBow_Skill02_Hit.wav", transform_.GetWorldPosition());
					FT::SendPacket(packet);
				}

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

				if (bKnockback_)
				{
					GameEngineSoundManager::GetInstance()->PlaySoundByName("Rio_LongBow_Skill04_Hit.wav");
					PacketSoundPlay packet;
					packet.SetSound("Rio_LongBow_Skill04_Hit.wav", transform_.GetWorldPosition());
					opponent->WallSlam(0.25f, transform_.GetWorldForwardVector() * 1000.f, 0.0f);
					FT::SendPacket(packet);
				}
				else
				{
					GameEngineSoundManager::GetInstance()->PlaySoundByName("Rio_ShortBow_Skill02_Hit.wav");
					PacketSoundPlay packet;
					packet.SetSound("Rio_ShortBow_Skill02_Hit.wav", transform_.GetWorldPosition());
					FT::SendPacket(packet);
				}

				Release();
				return;
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
