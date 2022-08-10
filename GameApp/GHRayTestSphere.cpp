#include "PreCompile.h"
#include "GHRayTestSphere.h"

#include <GameEngine/GameEngineCollision.h>

GHRayTestSphere::GHRayTestSphere()
	: collision_(nullptr)
	, renderer_(nullptr)
{

}

GHRayTestSphere::~GHRayTestSphere()
{

}

void GHRayTestSphere::Start()
{
	renderer_ = CreateTransformComponent<GameEngineRenderer>();
	renderer_->SetRenderingPipeLine("ColorSphere");
	renderer_->ShaderHelper.SettingConstantBufferLink("ResultColor", float4(1.0f, 0.0f, 0.0f, 1.0f));
	//renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });

	renderer_->GetGameEngineRenderingPipeLine()->SetRasterizer("EngineBaseRasterizerWireFrame");

	collision_ = CreateTransformComponent<GameEngineCollision>(renderer_->GetTransform());
	collision_->SetCollisionGroup(100);
	collision_->SetCollisionType(CollisionType::Sphere3D);

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
}

void GHRayTestSphere::Update(float _deltaTime)
{
	if (collision_->Collision(eCollisionGroup::MouseRay))
	{
		renderer_->ShaderHelper.SettingConstantBufferLink("ResultColor", float4(0.0f, 1.0f, 0.0f, 1.0f));
	}
	else
	{
		renderer_->ShaderHelper.SettingConstantBufferLink("ResultColor", float4(1.0f, 0.0f, 0.0f, 1.0f));
	}
}
