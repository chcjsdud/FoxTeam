#include "PreCompile.h"
#include "RioWSkillWind.h"
#include <GameEngine/GameEngineCollision.h>
#include "Character.h"
#include <GameEngine/GameEngineEffectRenderer.h>
#include "CharCrowdControlPacket.h"
RioWSkillWind::RioWSkillWind() // default constructer ����Ʈ ������
	:parentIndex_(-1)
{

}

RioWSkillWind::~RioWSkillWind() // default destructer ����Ʈ �Ҹ���
{

}

RioWSkillWind::RioWSkillWind(RioWSkillWind&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}

void RioWSkillWind::PlayAwake(float _time)
{
	timer_ = _time;
	renderState_ << "Awake";
}

void RioWSkillWind::Start()
{
	zoneRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	zoneRenderer_->SetImage("wMask.png", "PointSmp");
	zoneRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 20.0f, 0.0f });
	zoneRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	zoneRenderer_->GetTransform()->SetLocalScaling(zoneRenderer_->GetCurrentTexture()->GetTextureSize());
	//zoneRenderer_->SetAlpha(0.f);
	zoneRenderer_->Off();

	impactRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	impactRenderer_->SetImage("wWind.png", "PointSmp");
	impactRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 40.0f, 0.0f });
	impactRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f, 0.f,0.f });
	impactRenderer_->GetTransform()->SetLocalScaling(impactRenderer_->GetCurrentTexture()->GetTextureSize());
	//impactRenderer_->SetAlpha(0.8f);
	impactRenderer_->Off();

	collision_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
	collision_->GetTransform()->SetLocalPosition({ 0.f,0.f,0.f });
	collision_->GetTransform()->SetLocalScaling({ 450.0f, 450.0f, 450.0f });
	collision_->SetCollisionGroup(eCollisionGroup::PlayerAttack);
	collision_->SetCollisionType(CollisionType::CirCle);
	collision_->Off();

	renderState_.CreateState(MakeState(RioWSkillWind, Sleep));
	renderState_.CreateState(MakeState(RioWSkillWind, Awake));
	renderState_.CreateState(MakeState(RioWSkillWind, Fade));
	renderState_ << "Sleep";
}

void RioWSkillWind::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void RioWSkillWind::startSleep()
{
	timer_ = 0.0f;
	zoneRenderer_->Off();
	impactRenderer_->Off();
	collision_->Off();
	zoneRenderer_->SetAlpha(1.0f);
	impactRenderer_->SetAlpha(1.0f);
}

void RioWSkillWind::updateSleep(float _deltaTime)
{
}

void RioWSkillWind::startAwake()
{
	zoneRenderer_->On();
	impactRenderer_->On();
	rotationContainer_ = 0.0f;
	collision_->On();
}

void RioWSkillWind::updateAwake(float _deltaTime)
{
	if (true == collision_->IsUpdate())
	{
		GetLevel()->PushDebugRender(collision_->GetTransform(), CollisionType::CirCle, float4::BLUE);
	}

	timer_ -= _deltaTime;

	rotationContainer_ += 360.0f * _deltaTime;

	if (0.0f >= timer_)
	{
		timer_ = 0.0f;
		renderState_ << "Fade";
		return;
	}


	auto collisionList = collision_->GetCollisionList(eCollisionGroup::Player);

	for (GameEngineCollision* col : collisionList)
	{
		GameEngineActor* actor = col->GetActor();
		Character* character = nullptr;
		if (nullptr != actor)
		{
			character = dynamic_cast<Character*>(actor);

			if (nullptr != character && parentIndex_ != character->GetIndex())
			{
				character->Slow(0.5f, 0.7f);

				CharCrowdControlPacket ccPacket;
				ccPacket.SetTargetIndex(character->GetIndex());
				ccPacket.SetSlow(0.5f, 0.7f);

				FT::SendPacket(ccPacket);
			}
		}
	}

	impactRenderer_->GetTransform()->SetLocalRotationDegree({ 90.0f, rotationContainer_, 0.0f });
}

void RioWSkillWind::startFade()
{
	fadeTimer_ = 0.3f;
}

void RioWSkillWind::updateFade(float _deltaTime)
{


	fadeTimer_ -= _deltaTime;

	rotationContainer_ += 360.0f * _deltaTime;

	if (0.0f >= fadeTimer_)
	{
		timer_ = 0.0f;
		fadeTimer_ = 0.3f;
		rotationContainer_ = 0.0f;
		renderState_ << "Sleep";
		return;
	}

	zoneRenderer_->SetAlpha(fadeTimer_ / 0.3f);
	impactRenderer_->SetAlpha(fadeTimer_ / 0.3f);
	impactRenderer_->GetTransform()->SetLocalRotationDegree({ 90.0f, rotationContainer_, 0.0f });

}

