#include "PreCompile.h"
#include "GHRayTestBox.h"

#include <GameEngine/GameEngineCollision.h>

GHRayTestBox::GHRayTestBox()
	: collision_(nullptr)
	, renderer_(nullptr)
{

}

GHRayTestBox::~GHRayTestBox()
{

}

void GHRayTestBox::Start()
{
	renderer_ = CreateTransformComponent<GameEngineRenderer>();
	renderer_->SetRenderingPipeLine("Box");
	renderer_->ShaderHelper.SettingConstantBufferLink("ResultColor", float4(1.0f, 0.0f, 0.0f, 0.0f));
	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });

	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->SetCollisionGroup(100);

	GetLevel()->PushDebugRender(renderer_->GetTransform(), CollisionType::OBBBox3D);
}

void GHRayTestBox::Update(float _deltaTime)
{
}
