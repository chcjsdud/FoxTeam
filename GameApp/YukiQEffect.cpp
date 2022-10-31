#include "PreCompile.h"
#include "YukiQEffect.h"
#include "GameEngine/GameEngineEffectRenderer.h"

YukiQEffect::YukiQEffect() // default constructer 디폴트 생성자
	: kiCoreRenderer_(nullptr), kiGatheringRenderer_(nullptr), kiBurstRenderer_(nullptr), slashRenderer_(nullptr), timer_(0.0f)
{

}

YukiQEffect::~YukiQEffect() // default destructer 디폴트 소멸자
{

}

YukiQEffect::YukiQEffect(YukiQEffect&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void YukiQEffect::PlayAwake()
{
	timer_ = 0.0f;
	renderState_ << "Awake";
}

void YukiQEffect::PlayBurst(float4 _pos)
{
	targetPos_ = _pos;
	renderState_ << "Burst";
}

void YukiQEffect::Stop()
{
	renderState_ << "Sleep";
}

void YukiQEffect::Start()
{
	GameEngineTexture* kiGatheringTexture = GameEngineTextureManager::GetInst().Find("FX_BI_Dust_04.png");
	kiGatheringTexture->Cut(6, 6);

	GameEngineTexture* slashTexture = GameEngineTextureManager::GetInst().Find("FX_BI_SwordLine_01SE.png");
	slashTexture->Cut(12, 1);

	GameEngineTexture* burstTexture = GameEngineTextureManager::GetInst().Find("qburst.png");
	burstTexture->Cut(2,2);

	kiCoreRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	kiCoreRenderer_->SetImage("Fx_UI_Tierlight_OW.png", "PointSmp");
	kiCoreRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 50.0f, 90.0f });
	kiCoreRenderer_->GetTransform()->SetLocalRotationDegree({ 60.f,-35.f,0.f });
	kiCoreRenderer_->GetTransform()->SetLocalScaling(kiCoreRenderer_->GetCurrentTexture()->GetTextureSize());
	kiCoreRenderer_->Off();

	kiGatheringRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	kiGatheringRenderer_->SetImage("FX_BI_Dust_04.png", "PointSmp");
	kiGatheringRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 50.0f, 90.0f });
	kiGatheringRenderer_->GetTransform()->SetLocalRotationDegree({ 60.f,-35.f,0.f });
	kiGatheringRenderer_->GetTransform()->SetLocalScaling(kiGatheringRenderer_->GetCurrentTexture()->GetTextureSize());
	kiGatheringRenderer_->CreateAnimation("FX_BI_Dust_04.png", "FX_BI_Dust_04", 0, 35, 0.02f, false);
	kiGatheringRenderer_->SetChangeAnimation("FX_BI_Dust_04", true);
	kiGatheringRenderer_->Off();

	kiBurstRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	kiBurstRenderer_->SetImage("qburst.png", "PointSmp");
	kiBurstRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	kiBurstRenderer_->GetTransform()->SetLocalRotationDegree({ 60.f,-35.f,0.f });
	kiBurstRenderer_->GetTransform()->SetLocalScaling(kiBurstRenderer_->GetCurrentTexture()->GetTextureSize());
	kiBurstRenderer_->CreateAnimation("qburst.png", "qburst", 0, 3, 0.04f, false);
	kiBurstRenderer_->SetChangeAnimation("qburst", true);
	kiBurstRenderer_->SetColor({ 0.8f, 0.8f, 1.0f });
	kiBurstRenderer_->Off();

	float4 slashOriSclae = { 42.7f, 512.0f };
	slashRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>();
	slashRenderer_->SetImage("FX_BI_SwordLine_01SE.png", "PointSmp");
	slashRenderer_->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	slashRenderer_->GetTransform()->SetLocalRotationDegree({ 0.f, -35.f,0.f });
	slashRenderer_->GetTransform()->SetLocalScaling(slashOriSclae * 3.0f);
	slashRenderer_->CreateAnimation("FX_BI_SwordLine_01SE.png", "FX_BI_SwordLine_01SE", 0, 11, 0.02f, false);
	slashRenderer_->SetChangeAnimation("FX_BI_SwordLine_01SE", true);
	slashRenderer_->Off();

	renderState_.CreateState(MakeState(YukiQEffect, Sleep));
	renderState_.CreateState(MakeState(YukiQEffect, Awake));
	renderState_.CreateState(MakeState(YukiQEffect, Burst));
	renderState_ << "Sleep";
}

void YukiQEffect::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void YukiQEffect::startSleep()
{
	timer_ = 0.0f;
	kiCoreRenderer_->Off();
	kiGatheringRenderer_->Off();
	kiBurstRenderer_->Off();
	slashRenderer_->Off();
}

void YukiQEffect::updateSleep(float _deltaTime)
{
}

void YukiQEffect::startAwake()
{
	timer_ = 1.0f;
	kiCoreRenderer_->On();
	kiGatheringRenderer_->On();
	kiGatheringRenderer_->AnimationPlay();
	scaleContainer_ = { 256.0f, 256.0f };
}

void YukiQEffect::updateAwake(float _deltaTime)
{
	if (0.0f >= timer_)
	{
		return;
	}

	timer_ -= _deltaTime;

	rotationDegree_ += 360.0f * _deltaTime;

	if (0.0f <= scaleContainer_.x)
	{
		scaleContainer_ -= {360.0f * _deltaTime, 360.0f * _deltaTime};
	}

	kiCoreRenderer_->GetTransform()->SetLocalRotationDegree({ 90.0f, rotationDegree_, 0.0f });
	kiCoreRenderer_->GetTransform()->SetLocalScaling(scaleContainer_);

	kiGatheringRenderer_->GetTransform()->SetLocalRotationDegree({ 90.0f, rotationDegree_, 0.0f });
	kiGatheringRenderer_->GetTransform()->SetLocalScaling(scaleContainer_);
}

void YukiQEffect::startBurst()
{
	timer_ = 0.0f;

	kiBurstRenderer_->GetTransform()->SetWorldPosition(targetPos_);
	slashRenderer_->GetTransform()->SetWorldPosition(targetPos_);


}

void YukiQEffect::updateBurst(float _deltaTime)
{

	timer_ += _deltaTime;

	if (0.1f >= timer_)
	{
		slashRenderer_->On();
		slashRenderer_->SetChangeAnimation("FX_BI_SwordLine_01SE", true);
		slashRenderer_->AnimationPlay();

		kiBurstRenderer_->On();
		kiBurstRenderer_->SetChangeAnimation("qburst", true);
		kiBurstRenderer_->AnimationPlay();
		renderState_ << "Sleep";
	}
}

