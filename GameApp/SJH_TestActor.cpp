#include "PreCompile.h"
#include "SJH_TestActor.h"

#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineCollision.h>

void SJH_TestActor::Start()
{
	BoxRenderer_ = CreateTransformComponent<GameEngineRenderer>();
	BoxRenderer_->SetRenderingPipeLine("ColorSphere");
	BoxRenderer_->ShaderHelper.SettingConstantBufferLink("ResultColor", float4(1.0f, 0.0f, 1.0f, 1.0f));

	BoxCollider_ = CreateTransformComponent<GameEngineCollision>();
}

void SJH_TestActor::Update(float _deltaTime)
{
#ifdef _DEBUG
	if (nullptr != BoxCollider_)
	{
		GetLevel()->PushDebugRender(BoxCollider_->GetTransform(), CollisionType::Sphere3D);
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

void SJH_TestActor::SetResultColor(const float4& _Color)
{
	if (nullptr != BoxRenderer_)
	{
		BoxRenderer_->ShaderHelper.SettingConstantBufferLink("ResultColor", _Color);
	}
}
