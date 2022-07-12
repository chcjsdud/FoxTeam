#include "PreCompile.h"
#include "GameEngineUIRenderer.h"
#include "GameEngineLevel.h"
#include "CameraComponent.h"
#include "GameEngineFont.h"
#include "GameEngineFontManager.h"
#include "GameEngineWindow.h"
#include "GameEngineRenderTarget.h"

GameEngineRenderTarget* GameEngineUIRenderer::FontTarget_ = nullptr;
int GameEngineUIRenderer::UIRendererCount = 0;



GameEngineUIRenderer::GameEngineUIRenderer()
	: FontName_("±Ã¼­")
	, PrintText_("")
	, FontPivot_(float4::ZERO)
{
	++UIRendererCount;
}

GameEngineUIRenderer::~GameEngineUIRenderer()
{
	--UIRendererCount;
	if (0 == UIRendererCount && nullptr != FontTarget_)
	{
		delete FontTarget_;
		FontTarget_ = nullptr;
	}
}

void GameEngineUIRenderer::Start()
{
	// Push Render¸¦ ¸Ş
	GetLevel()->GetUICamera()->PushRenderer(GetOrder(), this);
	SetRenderingPipeLine("TextureAtlas");
	GetGameEngineRenderingPipeLine()->SetOutputMergerDepthStencil("BaseDepthOff");

	if (nullptr == FontTarget_
		&& UIRendererCount == 1)
	{
		FontTarget_ = new GameEngineRenderTarget();
		FontTarget_->Create(GameEngineWindow::GetInst().GetSize(), float4::NONE);


		//FontResultTarget_ = new GameEngineRenderTarget();
		//FontResultTarget_->Create(GameEngineWindow::GetInst().GetSize(), float4::NONE);
		
	}

}

void GameEngineUIRenderer::SetRenderGroup(int _Order) 
{
	GetLevel()->GetUICamera()->ChangeRendererGroup(_Order, this);;
}

void GameEngineUIRenderer::TextSetting(std::string _FontName, std::string _PrintText, float _FontSize, float4 _Color)
{
	FontName_ = _FontName;
	PrintText_ = _PrintText;
	FontSize_ = _FontSize;
	Color_ = _Color;
}

void GameEngineUIRenderer::Render(float _DeltaTime)
{
	GameEngineRenderer::Render(_DeltaTime);

	if ("" == PrintText_)
	{
		return;
	}
	
	// À§
	float4 ScreenSize = GameEngineWindow::GetInst().GetSize();

	// 360

	ScreenSize = ScreenSize.halffloat4();
	float4 UIPos = GetTransform()->GetWorldPosition();

	 GameEngineRenderTarget* RenderTarget = GameEngineRenderTarget::GetLastRenderTarget();

	FontTarget_->Clear();
	FontTarget_->Setting();

	GameEngineFont* Font = GameEngineFontManager::GetInst().Find(FontName_);
	Font->DrawFont(PrintText_, FontSize_, ScreenSize - UIPos, Color_, FW1_CENTER);
	GameEngineDevice::ShaderReset();

	 RenderTarget->Merge(FontTarget_);

	 RenderTarget->Setting();

}