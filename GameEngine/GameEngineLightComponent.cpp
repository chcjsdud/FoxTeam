#include "PreCompile.h"
#include "GameEngineLightComponent.h"
#include "GameEngineDepthBuffer.h"
#include "GameEngineWindow.h"

void GameEngineLightComponent::ShadowTargetSetting()
{
	float4 ClearColor = float4::RED;

	// RenderingPipeLine RenderTarget Clear & RenderTarget Setting
	GameEngineDevice::GetContext()->ClearRenderTargetView(ShadowTargetView_, ClearColor.Arr1D);
	GameEngineDevice::GetContext()->ClearDepthStencilView(ShadowTargetDepth_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	GameEngineDevice::GetContext()->OMSetRenderTargets(1, &ShadowTargetView_, ShadowTargetDepth_);
}

void GameEngineLightComponent::Start()
{
	GetLevel()->GetMainCamera()->PushLight(this);													// ����ī�޶��� ����(��)��Ͽ� �ڽ��� �߰�

	GetLevel()->AllLights_.push_back(this);															// ���� �������� �����ϴ� ����(��)��Ͽ� �ڽ��� �߰�

	//================================= �׸���ó��

	// 0. ���� �������� �����ϴ� �׸����ؽ��İ� �̹� �����Ѵٸ� ���� �ؽ��ĸ� ����
	// -> ���ο� ����(��)�� �߰��� �ؽ��Ĺ迭�� �����ؾ��ϹǷ� ���� �ؽ��ĸ� ����
	if (nullptr != GetLevel()->ShadowTexture_)
	{
		GetLevel()->ReleaseShadowTarget();
	}

	// 1. ���ο� ����(��)�� �߰��Ǿ����Ƿ� �ش� ������ �߰��� �ؽ��Ĺ迭�� ����
	GetLevel()->ShadowTexture_ = new GameEngineTexture();

	// 1-1. ��� ����(��) ������ŭ�� ���̹��� �ؽ��ĸ� ����
	D3D11_TEXTURE2D_DESC DepthTextureInfo = { 0, };
	DepthTextureInfo.ArraySize = static_cast<unsigned int>(GetLevel()->AllLights_.size());			// ���緹������ �����ϴ� ��� ������ ����ŭ ���̹����ؽ��Ĺ迭 ����
	DepthTextureInfo.Width = ShadowClipingRange_.uix();
	DepthTextureInfo.Height = ShadowClipingRange_.uiy();
	DepthTextureInfo.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
	DepthTextureInfo.SampleDesc.Count = 1;
	DepthTextureInfo.SampleDesc.Quality = 0;
	DepthTextureInfo.MipLevels = 1;
	DepthTextureInfo.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	DepthTextureInfo.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;
	GetLevel()->ShadowTexture_->Create(DepthTextureInfo, false);
	GetLevel()->ShadowTexture_->CreateShaderResourceView();

	// 1-2. �� ����(��)�� ����Ÿ�ٸ��ҽ��� ����/���ٽǹ��� ���ҽ��� ����
	int LightCount = static_cast<int>(GetLevel()->AllLights_.size());
	for (int LightNum = 0; LightNum < LightCount; ++LightNum)
	{
		// ����Ÿ�� ���ҽ� �迭
		ID3D11RenderTargetView* Result = GetLevel()->ShadowTexture_->CreateRenderTargetViewArrayIndex(LightNum);

		// ����/���ٽǹ��� ���ҽ� �迭
		GameEngineDepthBuffer* DepthBuffer = new GameEngineDepthBuffer();
		DepthBuffer->Create({ ShadowClipingRange_.x, ShadowClipingRange_.y });

		GetLevel()->AllLights_[LightNum]->ShadowTargetView_ = Result;
		GetLevel()->AllLights_[LightNum]->ShadowTargetDepth_ = DepthBuffer->GetDepthStencilView();
		GetLevel()->Depths_.push_back(DepthBuffer);
		GetLevel()->RenderTargets_.push_back(Result);
	}
}

void GameEngineLightComponent::Update(float _DeltaTime)
{
	//=================================== ����(��)ó�� : ������� ������ ����
	LightDataObject_.ViewLightDir = GetTransform()->GetWorldForwardVector();
	LightDataObject_.ViewLightPosition = GetTransform()->GetWorldPosition();

	//=================================== �׸���ó�� : ������� ������ ����
	GetTransform()->GetTransformData().View_.ViewToLH(GetTransform()->GetWorldPosition(), GetTransform()->GetWorldForwardVector(), GetTransform()->GetWorldUpVector());
	if (ProjectionMode::Orthographic == ProjectionMode_)
	{
		GetTransform()->GetTransformData().Projection_.OrthographicLH(ShadowClipingRange_.x, ShadowClipingRange_.y, ShadowClipingNear_, ShadowClipingFar_);
	}

	LightDataObject_.LightView = GetTransform()->GetTransformData().View_;
	LightDataObject_.LightProj = GetTransform()->GetTransformData().Projection_;
	LightDataObject_.LightVP = LightDataObject_.LightView * LightDataObject_.LightProj;
}

GameEngineLightComponent::GameEngineLightComponent()
{
	// Initalize LightData
	LightDataObject_.DiffuseLightColor = float4::ONE;
	LightDataObject_.AmbientLightColor = float4::ONE;
	LightDataObject_.SpacularLightColor = float4::ONE;
	LightDataObject_.AmbientLight = float4(0.1f, 0.1f, 0.1f, 0.1f);
	LightDataObject_.SpacularLightPow.x = 50.0f;
	LightDataObject_.LightPower = float4::ONE;

	// Initalize Shadow Data
	ProjectionMode_ = ProjectionMode::Orthographic;
	ShadowClipingRange_.x = 4096.0f;
	ShadowClipingRange_.y = 4096.0f;
	ShadowClipingNear_ = 0.1f;
	ShadowClipingFar_ = 10000.0f;
}

GameEngineLightComponent::~GameEngineLightComponent()
{
}
