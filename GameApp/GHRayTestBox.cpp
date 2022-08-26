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
	renderer_->SetRenderingPipeLine("ColorBox");
	renderer_->ShaderHelper.SettingConstantBufferLink("ResultColor", float4(1.0f, 0.0f, 0.0f, 1.0f));
	//renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });

	renderer_->GetGameEngineRenderingPipeLine()->SetRasterizer("EngineBaseRasterizerWireFrame");

	collision_ = CreateTransformComponent<GameEngineCollision>(renderer_->GetTransform());
	collision_->SetCollisionGroup(100);
	collision_->SetCollisionType(CollisionType::AABBBox3D);

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
}

void GHRayTestBox::Update(float _deltaTime)
{
	if (collision_->Collision(CollisionGroup::MouseRay))
	{
		renderer_->ShaderHelper.SettingConstantBufferLink("ResultColor", float4(0.0f, 1.0f, 0.0f, 1.0f));
	}
	else
	{
		renderer_->ShaderHelper.SettingConstantBufferLink("ResultColor", float4(1.0f, 0.0f, 0.0f, 1.0f));
	}
}
