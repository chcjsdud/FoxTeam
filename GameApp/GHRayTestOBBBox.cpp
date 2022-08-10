#include "PreCompile.h"
#include "GHRayTestOBBBox.h"

#include <GameEngine/GameEngineCollision.h>

GHRayTestOBBBox::GHRayTestOBBBox()
	: collision_(nullptr)
	, renderer_(nullptr)
{

}

GHRayTestOBBBox::~GHRayTestOBBBox()
{

}

void GHRayTestOBBBox::Start()
{
	renderer_ = CreateTransformComponent<GameEngineRenderer>();
	renderer_->SetRenderingPipeLine("ColorBox");
	renderer_->ShaderHelper.SettingConstantBufferLink("ResultColor", float4(1.0f, 0.0f, 0.0f, 1.0f));
	//renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });

	renderer_->GetGameEngineRenderingPipeLine()->SetRasterizer("EngineBaseRasterizerWireFrame");

	collision_ = CreateTransformComponent<GameEngineCollision>(renderer_->GetTransform());
	collision_->SetCollisionGroup(100);
	collision_->SetCollisionType(CollisionType::OBBBox3D);

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
}

void GHRayTestOBBBox::Update(float _deltaTime)
{
	renderer_->GetTransform()->AddLocalDeltaTimeRotation({ 30.0f, 90.f, 60.0f });

	if (collision_->Collision(eCollisionGroup::MouseRay))
	{
		renderer_->ShaderHelper.SettingConstantBufferLink("ResultColor", float4(0.0f, 1.0f, 0.0f, 1.0f));
	}
	else
	{
		renderer_->ShaderHelper.SettingConstantBufferLink("ResultColor", float4(1.0f, 0.0f, 0.0f, 1.0f));
	}
}
