#include "PreCompile.h"
#include "SJH_TestActor.h"

#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineCollision.h>

void SJH_TestActor::Start()
{
	BoxRenderer_ = CreateTransformComponent<GameEngineRenderer>();
	BoxRenderer_->SetRenderingPipeLine("ColorBox");
	BoxRenderer_->ShaderHelper.SettingConstantBufferLink("ResultColor", float4(1.0f, 0.0f, 1.0f, 1.0f));

	BoxCollider_ = CreateTransformComponent<GameEngineCollision>();

	GetTransform()->SetLocalPosition(float4(50.f, 0.f));
	GetTransform()->SetLocalScaling(float4(10.f, 10.f, 10.f));
}

void SJH_TestActor::Update(float _deltaTime)
{
#ifdef _DEBUG
	if (nullptr != BoxCollider_)
	{
		GetLevel()->PushDebugRender(BoxCollider_->GetTransform(), CollisionType::OBBBox3D);
	}
#endif // _DEBUG

}

SJH_TestActor::SJH_TestActor()
	: BoxRenderer_(nullptr)
	, BoxCollider_(nullptr)
{
}

SJH_TestActor::~SJH_TestActor()
{
}
