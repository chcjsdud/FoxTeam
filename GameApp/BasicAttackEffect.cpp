#include "PreCompile.h"
#include "BasicAttackEffect.h"

#include <GameEngine/GameEngineEffectRenderer.h>

BasicAttackEffect::BasicAttackEffect() // default constructer ����Ʈ ������
	: atkRenderer_(nullptr)
{

}

BasicAttackEffect::~BasicAttackEffect() // default destructer ����Ʈ �Ҹ���
{

}

BasicAttackEffect::BasicAttackEffect(BasicAttackEffect&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}

void BasicAttackEffect::PlayAwake(const std::string& _animationName)
{
	animationName_ = _animationName;
	renderState_ << "Awake";
}

void BasicAttackEffect::Start()
{
	atkRenderer_ = CreateTransformComponent<GameEngineEffectRenderer>(GetTransform());
	atkRenderer_->SetImage("effectDummy.png", "PointSmp");

	renderState_.CreateState(MakeState(BasicAttackEffect, Sleep));
	renderState_.CreateState(MakeState(BasicAttackEffect, Awake));
	renderState_ << "Sleep";
}

void BasicAttackEffect::Update(float _deltaTime)
{
	renderState_.Update(_deltaTime);
}

void BasicAttackEffect::startSleep()
{

	atkRenderer_->Off();
}

void BasicAttackEffect::updateSleep(float _deltaTime)
{
}

void BasicAttackEffect::startAwake()
{
	atkRenderer_->On();
	atkRenderer_->SetChangeAnimation(animationName_, true);
	atkRenderer_->AnimationPlay();
}

void BasicAttackEffect::updateAwake(float _deltaTime)
{
	if (true == atkRenderer_->IsCurAnimationEnd())
	{

		renderState_ << "Sleep";
	}
}

