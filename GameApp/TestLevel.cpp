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
}

void TestLevel::LevelUpdate(float _DeltaTime)
{
	if (GameEngineInput::Down("O"))
	{
		MainCameraActor_->FreeCameraModeSwitch();
	}

}

void TestLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{
	Rio::ReleaseResource();
}

void TestLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	Rio::LoadResource();

	Rio* rio_ = CreateActor<Rio>();
	rio_->Focus();
	//CreateActor<TestOBB>();

	CreateActor<SKySphereActor>();
	LightActor* light = CreateActor<LightActor>();
	light->GetLight()->SetDiffusePower(1.0f);
	light->GetLight()->SetAmbientPower(10.0f);
	light->GetLight()->SetSpacularLightPow(10.0f);
}
