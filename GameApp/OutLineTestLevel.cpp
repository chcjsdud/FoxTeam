#include "PreCompile.h"
#include "OutLineTestLevel.h"

#include "OutLineTestWindow.h"

#include "Enums.h"
#include "UserGame.h"
#include "MousePointer.h"

bool OutLineTestLevel::ThreadLoadingEnd = false;

void OutLineTestLevel::CreateBasicActor()
{
	// 마우스 생성
	if (nullptr == MousePointer::InGameMouse)
	{
		MousePointer::InGameMouse = CreateActor<MousePointer>();
		MousePointer::InGameMouse->GetTransform()->SetLocalPosition(GameEngineInput::GetInst().GetMouse3DPos());
	}

	// ...
}

void OutLineTestLevel::LevelStart()
{
}

void OutLineTestLevel::LevelUpdate(float _DeltaTime)
{
	// 리소스 로딩 완료 후 액터생성
	if (0 >= UserGame::LoadingFolder &&
		false == ThreadLoadingEnd)
	{
		CreateBasicActor();
		ThreadLoadingEnd = true;
	}
}

void OutLineTestLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
	// IMGUI Window
	OutLineWindow_ = GameEngineGUI::GetInst()->FindGUIWindowConvert<OutLineTestWindow>("OutLineTestWindow");
	if (nullptr == OutLineWindow_)
	{
		OutLineWindow_ = GameEngineGUI::GetInst()->CreateGUIWindow<OutLineTestWindow>("OutLineTestWindow");
		OutLineWindow_->Off();
	}
	OutLineWindow_->Off();
}

void OutLineTestLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	// IMGUI Window
	OutLineWindow_ = GameEngineGUI::GetInst()->FindGUIWindowConvert<OutLineTestWindow>("OutLineTestWindow");
	if (nullptr == OutLineWindow_)
	{
		OutLineWindow_ = GameEngineGUI::GetInst()->CreateGUIWindow<OutLineTestWindow>("OutLineTestWindow");
		OutLineWindow_->On();
	}
	OutLineWindow_->On();

	// 카메라위치 조정
	GetMainCamera()->SetProjectionMode(ProjectionMode::Perspective);
	GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.0f, 0.0f, -100.0f));
}

OutLineTestLevel::OutLineTestLevel()
	: OutLineWindow_(nullptr)
{
}

OutLineTestLevel::~OutLineTestLevel()
{
}
