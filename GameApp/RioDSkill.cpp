#include "PreCompile.h"
#include "RioDSkill.h"

#include "GameEngine/GameEngineCollision.h"
#include "IUnit.h"

RioDSkill::RioDSkill()
	: waitTime_(0.0f)
	, damage_(0.0f)
	, collision_(nullptr)
	, owner_(nullptr)
	, soundCount_(4)
{

}

RioDSkill::~RioDSkill()
{

}

void RioDSkill::Start()
{
	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->SetCollisionType(CollisionType::Sphere3D);
	collision_->GetTransform()->SetLocalScaling(500.0f);

	state_.CreateState(MakeState(RioDSkill, Wait));
	state_.CreateState(MakeState(RioDSkill, Falling));
	state_.CreateState(MakeState(RioDSkill, Fall));

	state_ << "Wait";
}

void RioDSkill::Update(float _deltaTime)
{
	level_->PushDebugRender(collision_->GetTransform(), CollisionType::Sphere3D);

	state_.Update(_deltaTime);
}

void RioDSkill::startWait()
{
	float4 skillPosition = transform_.GetWorldPosition();
	float4 ownerPosition = owner_->GetTransform()->GetWorldPosition();
	float length = float4::Calc_Len3D(skillPosition, ownerPosition);

	if (length > 700.f)
	{
		float4 direction = skillPosition - ownerPosition;
		direction.Normalize3D();
		transform_.SetWorldPosition(ownerPosition + direction * 700.0f);
	}
}

void RioDSkill::updateWait(float _deltaTime)
{
	waitTime_ -= _deltaTime;

	if (waitTime_ < 0.f)
	{
		state_ << "Falling";
	}
}

void RioDSkill::startFalling()
{
}

void RioDSkill::updateFalling(float _deltaTime)
{
	if (state_.GetCurrentState()->Time_ > 3.0f)
	{
		state_ << "Fall";
	}
}

void RioDSkill::startFall()
{
	auto list = collision_->GetCollisionList(eCollisionGroup::Player);
	for (GameEngineCollision* col : list)
	{
		IUnit* unit = dynamic_cast<IUnit*>(col->GetActor());

		if (unit == owner_)
		{
			continue;
		}

		unit->Damage(damage_);
	}
}

void RioDSkill::updateFall(float _deltaTime)
{
	if (soundCount_ < 0)
	{
		Release();
	}

	if (state_.GetCurrentState()->Time_ > 0.7f)
	{
		FT::PlaySoundAndSendPacket("Rio_ShortBow_Hit_01.wav", transform_.GetWorldPosition());
		state_.GetCurrentState()->Time_ = 0.0f;
		soundCount_--;
	}
}
