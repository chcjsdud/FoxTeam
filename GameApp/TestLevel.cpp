#include "PreCompile.h"
#include "TestLevel.h"

#include <GameEngine/GameEngineInput.h>
#include <GameEngine/SKySphereActor.h>
#include <GameEngine/LightActor.h>
#include "Rio.h"

#include "TestOBB.h"

TestLevel::TestLevel()
	: rio_(nullptr)
{
}

TestLevel::~TestLevel()
{

}

void TestLevel::LevelStart()
{
	GameEngineInput::GetInst().CreateKey("O", 'O');
	GameEngineInput::GetInst().CreateKey("Z", 'Z');
	GameEngineInput::GetInst().CreateKey("X", 'X');
	GameEngineInput::GetInst().CreateKey("C", 'C');
}

void TestLevel::LevelUpdate(float _DeltaTime)
{
	if (GameEngineInput::Down("O"))
	{
		MainCameraActor_->FreeCameraModeSwitch();
	}

	if (GameEngineInput::Press("C"))
	{
		rio_->ChangeAnimation(std::string("Run_Long"));
	}

	if (GameEngineInput::Press("X"))
	{
		rio_->ChangeAnimation(std::string("Run_Short"));
	}

	if (GameEngineInput::Press("Z"))
	{
		rio_->ChangeAnimation(std::string("Wait_Short"));
	}
}

void TestLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
	this->ClearAll();
	Rio::ReleaseResource();

	MainCameraActor_ = CreateActor<CameraActor>();
	UICameraActor_ = CreateActor<CameraActor>();
	UICameraActor_->GetCamera()->SetProjectionMode(ProjectionMode::Orthographic);
}

void TestLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	Rio::LoadResource();

	rio_ = CreateActor<Rio>();
	rio_->Focus();
	//CreateActor<TestOBB>();

	CreateActor<SKySphereActor>();
	LightActor* light = CreateActor<LightActor>();
	light->GetLight()->SetDiffusePower(1.0f);
	light->GetLight()->SetAmbientPower(10.0f);
	light->GetLight()->SetSpacularLightPow(10.0f);
}
