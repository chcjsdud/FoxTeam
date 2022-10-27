#include "PreCompile.h"
#include "GameEngineOutlineRenderer.h"

void GameEngineOutlineRenderer::Start()
{
	GetLevel()->GetMainCamera()->PushRenderer(GetOrder(), this);

	// ���� ����
	SetRenderingPipeLine("ObjectOutLine");

	// ������� ��ũ
	OutLineData_.LineColor = float4(0.0f, 1.0f, 0.0f, 1.0f);
	OutLineData_.LineThickness = 2.0f;
	ShaderHelper.SettingConstantBufferLink("OutLineData", OutLineData_);
}

void GameEngineOutlineRenderer::Update(float _DeltaTime)
{
}

GameEngineOutlineRenderer::GameEngineOutlineRenderer()
	: OutLineData_{}
{
}

GameEngineOutlineRenderer::~GameEngineOutlineRenderer()
{
}
