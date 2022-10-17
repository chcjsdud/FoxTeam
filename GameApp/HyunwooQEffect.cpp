#include "PreCompile.h"
#include "HyunwooQEffect.h"
#include <GameEngine/GameEngineEffectRenderer.h>
#include <GameEngine/GameEngineImageRenderer.h>

HyunwooQEffect::HyunwooQEffect() // default constructer 디폴트 생성자
	: groundCrackRenderer_(nullptr), dustRenderer_(nullptr), isActivated_(false), crackFadeTime_(1.0f), timer_(0.0f)
{

}

HyunwooQEffect::~HyunwooQEffect() // default destructer 디폴트 소멸자
{

}

HyunwooQEffect::HyunwooQEffect(HyunwooQEffect&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void HyunwooQEffect::PlayAwake()
{
	isActivated_ = true;
}

void HyunwooQEffect::Start()
{

	//GameEngineTexture* hitBase = GameEngineTextureManager::GetInst().Find("FX_BI_Hit_05.png");
	//hitBase->Cut(3,3);

	groundCrackRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	groundCrackRenderer_->SetImage("FX_BI_GroundBomb_01.png", "PointSmp");
	groundCrackRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 10.0f, 150.0f});
	groundCrackRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	groundCrackRenderer_->GetTransform()->SetLocalScaling(groundCrackRenderer_->GetCurrentTexture()->GetTextureSize()/2);

	impactRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	impactRenderer_->SetImage("FX_BI_GroundBomb_01.png", "PointSmp");
	impactRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 10.0f, 150.0f });
	impactRenderer_->GetTransform()->SetLocalRotationDegree({ 90.f,0.f,0.f });
	impactRenderer_->GetTransform()->SetLocalScaling(impactRenderer_->GetCurrentTexture()->GetTextureSize() / 2);
	impactRenderer_->CreateAnimation("FX_BI_Hit_05.png", "FX_BI_Hit_05", 0, 8, 0.04f, false);
	impactRenderer_->SetChangeAnimation("FX_BI_Hit_05", true);

	renderState_.CreateState(MakeState(HyunwooQEffect, Sleep));
	renderState_.CreateState(MakeState(HyunwooQEffect, Awake));
	renderState_ << "Sleep";

}

void HyunwooQEffect::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void HyunwooQEffect::startSleep()
{


	groundCrackRenderer_->SetAlpha(0.0f);
	isActivated_ = false;
	crackFadeTime_ = 1.0f;
	timer_ = 0.0f;
}

void HyunwooQEffect::updateSleep(float _deltaTime)
{
	if (true == isActivated_)
	{
		renderState_ << "Awake";
		return;
	}
}

void HyunwooQEffect::startAwake()
{
	groundCrackRenderer_->SetAlpha(1.0f);
	impactRenderer_->SetChangeAnimation("FX_BI_Hit_05", true);
	impactRenderer_->AnimationPlay();
	crackFadeTime_ = 0.5f;
}

void HyunwooQEffect::updateAwake(float _deltaTime)
{

	
	crackFadeTime_ -= _deltaTime;


	if (0.0f >= crackFadeTime_)
	{
		renderState_ << "Sleep";
		groundCrackRenderer_->SetAlpha(0.0f);
		return;
	}
}
