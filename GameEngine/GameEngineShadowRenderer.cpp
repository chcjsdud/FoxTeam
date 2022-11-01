#include "PreCompile.h"
#include "GameEngineShadowRenderer.h"

void GameEngineShadowRenderer::Start()
{
	SetRenderingPipeLine("ShadowAni");

	ShadowColor_ = float4(0.0f, 0.0f, 0.0f, 1.0f);
	ShaderHelper.SettingConstantBufferLink("ShadowColor", ShadowColor_);
}

void GameEngineShadowRenderer::Update(float _DeltaTime)
{
}

GameEngineShadowRenderer::GameEngineShadowRenderer()
	: FBXMesh_(nullptr)
	, BaseRenderer_(nullptr)
{
}

GameEngineShadowRenderer::~GameEngineShadowRenderer()
{
}
