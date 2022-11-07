#include "PreCompile.h"
#include "GameEngineRendererBase.h"
#include "GameEngineLevel.h"
#include "GameEngineRenderingPipeLineManager.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineTransform.h"
#include "GameEngineVertexShader.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineIndexBuffer.h"
#include "GameEnginePixelShader.h"
#include "CameraComponent.h"

void GameEngineRendererBase::ShadowOff()
{
	RendererDataInst.IsShadow = 0;
}

void GameEngineRendererBase::ShadowOn()
{
	//// 221107 SJH EDIT : �ּ�ó��(���� ���� ���Ұ��� ó������)
	//// �׸���ó�� ������ ������������ �����Ǿ������ʴٸ� ���� ��
	//if (nullptr == DefaultShadowPipeLine_)
	//{
	//	DefaultShadowPipeLine_ = GameEngineRenderingPipeLineManager::GetInst().Find("DepthShadow");		// Find DepthShadow PipeLine
	//	DefaultShadowPipeLine_->CreateLayOut();															// Create InputLayout

	//	ShadowHelper_.ShaderResourcesCheck(DefaultShadowPipeLine_);										// DefaultShadowPipeLine_�� ������۸� üũ			: ������ ��Ī ������� üũ, ���̴����ۿ� ������ۿ� ������ ũ���� �޸��Ҵ�
	//	
	//	// ������� ��ũ
	//	if (true == ShadowHelper_.IsConstantBuffer("RendererData"))
	//	{
	//		ShadowHelper_.SettingConstantBufferLink("RendererData", RendererDataInst);
	//	}

	//	if (true == ShadowHelper_.IsConstantBuffer("TransformData"))
	//	{
	//		ShadowHelper_.SettingConstantBufferLink("TransformData", GetTransform()->GetTransformData());
	//	}

	//	if (true == ShadowHelper_.IsConstantBuffer("LightsData"))
	//	{
	//		const LightsData& AllLightData = GetLevel()->GetMainCamera()->GetLightData();
	//		ShadowHelper_.SettingConstantBufferLink("LightsData", AllLightData);
	//	}

	//	DefaultShadowPipeLine_->SetViewPort(4096, 4096, 0, 0);
	//}

	//// �׸��ڻ�� Flag On
	//RendererDataInst.IsShadow = 1;
}

void GameEngineRendererBase::Start()
{
	GetLevel()->GetMainCamera()->PushRenderer(GetOrder(), this);
}

void GameEngineRendererBase::SetRenderGroup(int _Order)
{
	GetLevel()->GetMainCamera()->ChangeRendererGroup(_Order, this);
}

void GameEngineRendererBase::Render(float _DeltaTime, bool _IsDeferred)
{
}

void GameEngineRendererBase::ShadowInit(GameEngineRenderingPipeLine* _ShadowPipe)
{
	// �������� �޾Ƽ� ó���Ҳ���
}

void GameEngineRendererBase::ShadowRender(float _DeltaTime)
{
	if (nullptr == DefaultShadowPipeLine_)
	{
		return;
	}

	ShadowInit(DefaultShadowPipeLine_);
	ShadowHelper_.Setting();
	DefaultShadowPipeLine_->Rendering();
}

GameEngineRendererBase::GameEngineRendererBase()
	: IsPreprocessing_(false)
	, DefaultShadowPipeLine_(nullptr)
	, ShadowHelper_()
{
}

GameEngineRendererBase::~GameEngineRendererBase()
{
}
