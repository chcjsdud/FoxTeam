#include "PreCompile.h"
#include "TestLevel.h"

#include <GameEngine/GameEngineInput.h>
#include <GameEngine/SKySphereActor.h>
#include <GameEngine/LightActor.h>
#include "Rio.h"

#include "TestOBB.h"
#include "TestLevelBox.h"
#include "MousePointer.h"
#include "TestMap.h"

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
	GameEngineInput::GetInst().CreateKey("V", 'V');
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
		if (rio_->IsLongBow())
		{
			rio_->ChangeAnimation(std::string("Wait_Long"));
		}
		else
		{
			rio_->ChangeAnimation(std::string("Wait_Short"));

		}
	}

	if (false == GetMainCameraActor()->IsFreeCameraMode())
	{
		float4 playerPosition = rio_->GetTransform()->GetWorldPosition();
		GetMainCameraActor()->GetTransform()->SetWorldPosition(playerPosition + float4(200.f, 640.f, -300.f));
		GetMainCameraActor()->GetTransform()->SetLocalRotationDegree({ 60.f, -35.f, 0.0f });
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

	TestLevelBox* box = nullptr;
	//box = CreateActor<TestLevelBox>();
	box = CreateActor<TestLevelBox>();
	//box->GetTransform()->SetLocalScaling({ 1000.f, 1000.f });
	box->GetTransform()->SetWorldPosition({ 0.f, 100.f });


	mouse_ = CreateActor<MousePointer>();
	map_ = CreateActor<TestMap>();
	rio_->SetMouse(mouse_);
	rio_->SetMap(map_);

	CreateActor<SKySphereActor>();
	LightActor* light = CreateActor<LightActor>();
	light->GetLight()->SetDiffusePower(1.0f);
	light->GetLight()->SetAmbientPower(10.0f);
	light->GetLight()->SetSpacularLightPow(10.0f);

	GameServer::GetInstance()->TestOpen();
}
