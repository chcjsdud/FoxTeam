#include "PreCompile.h"
#include "HyunwooREffect.h"
#include <GameEngine/GameEngineEffectRenderer.h>
#include <GameEngine/GameEngineImageRenderer.h>
HyunwooREffect::HyunwooREffect() // default constructer 디폴트 생성자
	: isActivated_(false), isTriggered_(false), timer_(0.0f)
{

}

HyunwooREffect::~HyunwooREffect() // default destructer 디폴트 소멸자
{

}

HyunwooREffect::HyunwooREffect(HyunwooREffect&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void HyunwooREffect::PlayAwake()
{
	renderState_ << "Awaken";
}

void HyunwooREffect::PlayExplode()
{
	renderState_ << "Explode";
}

void HyunwooREffect::SetSleepMandatory()
{
	renderState_ << "Sleeping";
}

void HyunwooREffect::Start()
{
	//GameEngineTexture* hitBase = GameEngineTextureManager::GetInst().Find("FX_BI_Hit_05.png");
	//hitBase->Cut(3, 3);
	
	hitBoxRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	hitBoxRenderer_->SetImage("FX_BI_Jan_Skill01_01_Range_1.png", "PointSmp");
	hitBoxRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 10.0f, 150.0f });
	hitBoxRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	hitBoxRenderer_->GetTransform()->SetLocalScaling({350.0f, 10.0f, 150.0f});

	impactRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	impactRenderer_->SetImage("FX_BI_Hit_05.png", "PointSmp");
	impactRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 30.0f, 300.0f });
	impactRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	impactRenderer_->GetTransform()->SetLocalScaling(impactRenderer_->GetCurrentTexture()->GetTextureSize());
	impactRenderer_->CreateAnimation("FX_BI_Hit_05.png", "FX_BI_Hit_05", 0, 8, 0.04f, false);
	impactRenderer_->SetChangeAnimation("FX_BI_Hit_05", true);

	renderState_.CreateState(MakeState(HyunwooREffect, Sleeping));
	renderState_.CreateState(MakeState(HyunwooREffect, Awaken));
	renderState_.CreateState(MakeState(HyunwooREffect, Explode));

	renderState_ << "Sleeping";
}

void HyunwooREffect::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void HyunwooREffect::startAwaken()
{
	hitBoxRenderer_->SetAlpha(1.0f);
	timer_ = 0.0f;
}

void HyunwooREffect::updateAwaken(float _deltaTime)
{
	//if (true == isTriggered_)
	//{
	//	renderState_ << "Explode";
	//	return;
	//}

	timer_ += _deltaTime;

	hitBoxRenderer_->GetTransform()->SetLocalPosition({ 0.f,10.f,150.f + (timer_ * 50.0f) });
	hitBoxRenderer_->GetTransform()->SetLocalScaling({ 350.0f, 150.0f + (timer_ * 100.0f), 10.0f });
}

void HyunwooREffect::startExplode()
{
	hitBoxRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 10.0f, 150.0f });
	hitBoxRenderer_->GetTransform()->SetLocalScaling(hitBoxRenderer_->GetCurrentTexture()->GetTextureSize());


	impactRenderer_->SetAlpha(1.0f);
	impactRenderer_->SetChangeAnimation("FX_BI_Hit_05", true);
	impactRenderer_->AnimationPlay();
}

void HyunwooREffect::updateExplode(float _deltaTime)
{

	if (true == impactRenderer_->IsCurAnimationEnd())
	{
		renderState_ << "Sleeping";
		return;
	}
}



void HyunwooREffect::startSleeping()
{
	hitBoxRenderer_->SetAlpha(0.0f);
	impactRenderer_->SetAlpha(0.0f);

	isActivated_ = false;
	isTriggered_ = false;

	hitBoxRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 10.0f, 150.0f });
	hitBoxRenderer_->GetTransform()->SetLocalScaling(hitBoxRenderer_->GetCurrentTexture()->GetTextureSize());

	timer_ = 0.0f;
}

void HyunwooREffect::updateSleeping(float _deltaTime)
{
	//if (true == isActivated_)
	//{
	//	renderState_ << "Awaken";
	//	return;
	//}
}

