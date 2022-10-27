#include "PreCompile.h"
#include "StunEffect.h"

#include <GameEngine/GameEngineEffectRenderer.h>
StunEffect::StunEffect() // default constructer 디폴트 생성자
	: timer_(0.0f), rotationDegree_(0.0f), stunRingRenderer_(nullptr), starRenderer0_(nullptr), starRenderer1_(nullptr)
{

}

StunEffect::~StunEffect() // default destructer 디폴트 소멸자
{

}

StunEffect::StunEffect(StunEffect&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}


void StunEffect::PlayAwake(float _time)
{
	timer_ = _time;

	if (0.0f >= timer_)
	{
		timer_ = 0.0f;
	}

	renderState_ << "Awake";
}

void StunEffect::Start()
{
	stunRingRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());
	stunRingRenderer_->SetImage("stunring.png", "PointSmp");

	stunRingRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f, 0.0f, 0.f });
	stunRingRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 300.0f, 0.0f });

	stunRingRenderer_->GetTransform()->SetLocalScaling(stunRingRenderer_->GetCurrentTexture()->GetTextureSize() * 0.5f);
	stunRingRenderer_->Off();

	starRenderer0_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());
	starRenderer0_->SetImage("stunstar.png", "PointSmp");
	//starRenderer0_->AttachTransform(stunRingRenderer_->GetTransform());
	starRenderer0_->GetTransform()->SetLocalRotationDegree({ 0.0f,0.0f,0.0f });
	starRenderer0_->GetTransform()->SetLocalPosition({ 10.0f, 300.0f, 0.0f });

	starRenderer0_->GetTransform()->SetLocalScaling(starRenderer0_->GetCurrentTexture()->GetTextureSize() * 0.05f);
	starRenderer0_->Off();

	starRenderer1_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());
	starRenderer1_->SetImage("stunstar.png", "PointSmp");
	//starRenderer1_->AttachTransform(stunRingRenderer_->GetTransform());
	starRenderer1_->GetTransform()->SetLocalRotationDegree({ 0.0f,0.0f,0.0f });
	starRenderer1_->GetTransform()->SetLocalPosition({ -10.0f, 300.0f, 0.0f });

	starRenderer1_->GetTransform()->SetLocalScaling(starRenderer1_->GetCurrentTexture()->GetTextureSize() * 0.05f);
	starRenderer1_->Off();

	renderState_.CreateState(MakeState(StunEffect, Sleep));
	renderState_.CreateState(MakeState(StunEffect, Awake));
	renderState_ << "Sleep";

}

void StunEffect::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void StunEffect::startAwake()
{
	stunRingRenderer_->On();
	starRenderer0_->On();
	starRenderer1_->On();


}

void StunEffect::updateAwake(float _deltaTime)
{
	if (timer_ <= 0.0f)
	{
		timer_ = 0.0f;
		renderState_ << "Sleep";
		return;
	}

	rotationDegree_ += 320.0f * _deltaTime;

	timer_ -= _deltaTime;
	stunRingRenderer_->GetTransform()->SetLocalRotationDegree({ 90.0f, rotationDegree_, 0.0f });
	//zoneRenderer1_->GetTransform()->SetLocalRotationDegree({ 60.0f, rotationDegree_, 0.0f });
	//zoneRenderer2_->GetTransform()->SetLocalRotationDegree({ 115.0f, rotationDegree_, 0.0f });

}

void StunEffect::startSleep()
{
	stunRingRenderer_->Off();
	starRenderer0_->Off();
	starRenderer1_->Off();
	timer_ = 0.0f;
}

void StunEffect::updateSleep(float _deltaTime)
{

	return;
}

