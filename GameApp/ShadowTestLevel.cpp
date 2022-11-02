#include "Precompile.h"
#include "ShadowTestLevel.h"

#include <GameEngine/LightActor.h>

#include "Enums.h"
#include "UserGame.h"
#include "MousePointer.h"

bool ShadowTestLevel::ThreadLoadingEnd = false;

void ShadowTestLevel::LoadBasicActorResrouce()
{
}

void ShadowTestLevel::CreateBasicActor()
{
	// ���콺 ����
	if (nullptr == MousePointer::InGameMouse)
	{
		MousePointer::InGameMouse = CreateActor<MousePointer>();
		MousePointer::InGameMouse->GetTransform()->SetLocalPosition(GameEngineInput::GetInst().GetMouse3DPos());
	}

	// �׽�Ʈ ����Ʈ����
	TestLightActor_ = CreateActor<LightActor>();
	TestLightActor_->GetLight()->SetDiffusePower(1.0f);
	TestLightActor_->GetLight()->SetAmbientPower(10.0f);
	TestLightActor_->GetLight()->SetSpacularLightPow(10.0f);
}

void ShadowTestLevel::LevelStart()
{
}

void ShadowTestLevel::LevelUpdate(float _DeltaTime)
{
	// ���ҽ� �ε� �Ϸ� �� ���ͻ���
	if (0 >= UserGame::LoadingFolder &&
		false == ThreadLoadingEnd)
	{
		LoadBasicActorResrouce();
		CreateBasicActor();
		ThreadLoadingEnd = true;
	}

	// Switching FreeCamMode
	if (true == GameEngineInput::GetInst().Down("O"))
	{
		GetMainCameraActor()->FreeCameraModeSwitch();
	}
}

void ShadowTestLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
}

void ShadowTestLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	// ī�޶���ġ ����
	GetMainCamera()->SetProjectionMode(ProjectionMode::Perspective);
	GetMainCameraActor()->GetTransform()->SetWorldPosition({ 0.0f, 500.f, -500.f });
	GetMainCameraActor()->GetTransform()->SetWorldRotationDegree({ 70.f, 0.0f, 0.0f });
}

ShadowTestLevel::ShadowTestLevel()
	: TestLightActor_(nullptr)
{
}

ShadowTestLevel::~ShadowTestLevel()
{
}
