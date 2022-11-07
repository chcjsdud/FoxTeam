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
	//// 221107 SJH EDIT : 주석처리(현재 게임 사용불가로 처리안함)
	//// 그림자처리 렌더링 파이프라인이 지정되어있지않다면 지정 후
	//if (nullptr == DefaultShadowPipeLine_)
	//{
	//	DefaultShadowPipeLine_ = GameEngineRenderingPipeLineManager::GetInst().Find("DepthShadow");		// Find DepthShadow PipeLine
	//	DefaultShadowPipeLine_->CreateLayOut();															// Create InputLayout

	//	ShadowHelper_.ShaderResourcesCheck(DefaultShadowPipeLine_);										// DefaultShadowPipeLine_의 상수버퍼를 체크			: 동일한 명칭 상수버퍼 체크, 쉐이더헬퍼에 상수버퍼와 동일한 크기의 메모리할당
	//	
	//	// 상수버퍼 링크
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

	//// 그림자사용 Flag On
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
	// 하위에서 받아서 처리할꺼임
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
