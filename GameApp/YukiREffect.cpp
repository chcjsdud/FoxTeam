#include "PreCompile.h"
#include "YukiREffect.h"
#include <GameEngine/GameEngineEffectRenderer.h>
#include <GameEngine/GameEngineImageRenderer.h>

YukiREffect::YukiREffect() // default constructer 디폴트 생성자
{

}

YukiREffect::~YukiREffect() // default destructer 디폴트 소멸자
{

}

YukiREffect::YukiREffect(YukiREffect&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void YukiREffect::PlayAwake()
{
	renderState_ << "Awake";
}

void YukiREffect::Start()
{
	zoneRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	zoneRenderer_->SetImage("FX_BI_Yuki_02.png", "PointSmp");
	zoneRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 50.0f, 200.0f });
	zoneRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	zoneRenderer_->GetTransform()->SetLocalScaling(zoneRenderer_->GetCurrentTexture()->GetTextureSize());
	zoneRenderer_->SetColor({0.8f, 0.2f, 0.2f});
	zoneRenderer_->SetAlpha(0.0f);
	zoneRenderer_->Off();

	impactRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	impactRenderer_->SetImage("FX_BI_Sword_01.png", "PointSmp");
	impactRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 50.0f, 230.0f });
	impactRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	impactRenderer_->GetTransform()->SetLocalScaling(impactRenderer_->GetCurrentTexture()->GetTextureSize());
	impactRenderer_->SetAlpha(0.8f);
	impactRenderer_->Off();

	renderState_.CreateState(MakeState(YukiREffect, Sleep));
	renderState_.CreateState(MakeState(YukiREffect, Awake));
	renderState_.CreateState(MakeState(YukiREffect, Slash));
	renderState_ << "Sleep";
}

void YukiREffect::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void YukiREffect::startAwake()
{
	zoneRenderer_->Off();
	impactRenderer_->Off();
	impactRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 50.0f, 230.0f });
	slashFlag_ = false;
	timer_ = 0.0f;
	groundFadeTime_ = 0.0f;
	slashFadeTime_ = 0.8f;
}

void YukiREffect::updateAwake(float _deltaTime)
{
	if (0.8f <= timer_)
	{
		timer_ = 0.0f;
		groundFadeTime_ = 0.0f;
		renderState_ << "Intermission";
		return;
	}

	if (0.5f >= groundFadeTime_)
	{
		groundFadeTime_ = 0.5f;
	}

	zoneRenderer_->SetAlpha(groundFadeTime_ * 2);

	groundFadeTime_ += _deltaTime;
	timer_ += _deltaTime;
}


void YukiREffect::startIntermission()
{
	timer_ = 0.0f;
	groundOutTime_ = 0.3f;
}

void YukiREffect::updateIntermission(float _deltaTime)
{

	if (0.3f <= timer_)
	{
		timer_ = 0.0f;
		groundOutTime_ = 0.0f;
		renderState_ << "Slash";
		return;
	}

	zoneRenderer_->SetAlpha(groundOutTime_ /0.3f);

	groundOutTime_ -= _deltaTime;
	timer_ += _deltaTime;
}

void YukiREffect::startSlash()
{
	impactRenderer_->On();
	slashFadeTime_ = 0.8f;
}

void YukiREffect::updateSlash(float _deltaTime)
{

}

void YukiREffect::startSleep()
{
}

void YukiREffect::updateSleep(float _deltaTime)
{
}
