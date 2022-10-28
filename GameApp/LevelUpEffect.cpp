#include "PreCompile.h"
#include "LevelUpEffect.h"
#include <GameEngine/GameEngineEffectRenderer.h>
LevelUpEffect::LevelUpEffect() // default constructer 디폴트 생성자
	: timer_(1.0f), levelUpRenderer_(nullptr), glowLineRenderer_(nullptr), ringRenderer_(nullptr), rotationDegree_(0.0f)
{

}

LevelUpEffect::~LevelUpEffect() // default destructer 디폴트 소멸자
{

}

LevelUpEffect::LevelUpEffect(LevelUpEffect&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void LevelUpEffect::PlayAwake()
{
	renderState_ << "Awake";
}

void LevelUpEffect::Start()
{
	levelUpRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());
	levelUpRenderer_->SetImage("FX_BI_Levelup_01.png", "PointSmp");
	levelUpRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 280.0f, 0.0f });
	levelUpRenderer_->GetTransform()->SetLocalRotationDegree({ 0.0f, 0.0f, 0.0f });
	levelUpRenderer_->GetTransform()->SetLocalScaling(levelUpRenderer_->GetCurrentTexture()->GetTextureSize());
	levelUpRenderer_->Off();

	glowLineRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());
	glowLineRenderer_->SetImage("FX_BI_GlowLine_01.png", "PointSmp");
	glowLineRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	glowLineRenderer_->GetTransform()->SetLocalRotationDegree({ 0.0f, 0.0f, 0.0f });
	glowLineRenderer_->GetTransform()->SetLocalScaling(glowLineRenderer_->GetCurrentTexture()->GetTextureSize());
	glowLineRenderer_->Off();

	ringRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());
	ringRenderer_->SetImage("levelupring.png", "PointSmp");
	ringRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	ringRenderer_->GetTransform()->SetLocalRotationDegree({ 90.0f,0.0f,0.0f });
	ringRenderer_->GetTransform()->SetLocalScaling(ringRenderer_->GetCurrentTexture()->GetTextureSize());
	ringRenderer_->Off();

	renderState_.CreateState(MakeState(LevelUpEffect, Sleep));
	renderState_.CreateState(MakeState(LevelUpEffect, Awake));
	renderState_ << "Sleep";
}

void LevelUpEffect::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void LevelUpEffect::startAwake()
{
	glowLineRenderer_->SetAlpha(0.7f);
	levelUpRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 280.0f, 0.0f });
	levelUpRenderer_->SetAlpha(1.0f);

	glowLineRenderer_->GetTransform()->SetLocalRotationDegree({ 0.0f, 0.0f, 0.0f });
	glowLineRenderer_->GetTransform()->SetLocalScaling(glowLineRenderer_->GetCurrentTexture()->GetTextureSize());

	ringRenderer_->GetTransform()->SetLocalScaling(ringRenderer_->GetCurrentTexture()->GetTextureSize());

	levelUpRenderer_->On();
	glowLineRenderer_->On();
	ringRenderer_->On();

	rotationDegree_ = 0.0f;
	timer_ = 1.0f;
}

void LevelUpEffect::updateAwake(float _deltaTime)
{

	timer_ -= _deltaTime;

	if (timer_ <= 0.0f)
	{
		timer_ = 1.0f;
		renderState_ << "Sleep";
		return;
	}

	rotationDegree_ += 60.0f * _deltaTime;


	levelUpRenderer_->GetTransform()->SetLocalMove({0.0f, 10.0f * _deltaTime, 0.0f}); // 위로 올라가면서 투명
	levelUpRenderer_->SetAlpha(timer_);

	ringRenderer_->GetTransform()->SetLocalRotationDegree({ 90.0f, rotationDegree_, 0.0f });
	ringRenderer_->GetTransform()->SetLocalScaling(ringRenderer_->GetCurrentTexture()->GetTextureSize() + float4{10.0f * _deltaTime, 10.0f * _deltaTime, 10.0f * _deltaTime});
	ringRenderer_->SetAlpha(timer_);

	glowLineRenderer_->GetTransform()->SetLocalScaling(glowLineRenderer_->GetCurrentTexture()->GetTextureSize() + float4{ 10.0f * _deltaTime, 10.0f * _deltaTime, 10.0f * _deltaTime });
	glowLineRenderer_->SetAlpha(timer_);
}

void LevelUpEffect::startSleep()
{
	levelUpRenderer_->Off();
	glowLineRenderer_->Off();
	ringRenderer_->Off();
	timer_ = 1.0f;
	rotationDegree_ = 0.0f;

	glowLineRenderer_->SetAlpha(0.7f);
	levelUpRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 280.0f, 0.0f });
	levelUpRenderer_->SetAlpha(1.0f);

	glowLineRenderer_->GetTransform()->SetLocalRotationDegree({ 0.0f, 0.0f, 0.0f });
	glowLineRenderer_->GetTransform()->SetLocalScaling(glowLineRenderer_->GetCurrentTexture()->GetTextureSize());

	ringRenderer_->GetTransform()->SetLocalScaling(ringRenderer_->GetCurrentTexture()->GetTextureSize());
}

void LevelUpEffect::updateSleep(float _deltaTime)
{
}

