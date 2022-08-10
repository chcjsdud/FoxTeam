#include "PreCompile.h"
#include "GHRio.h"

#include <GameEngine/GameEngineFBXRenderer.h>

GHRio::GHRio()
	: renderer_(nullptr)
{

}

GHRio::~GHRio()
{

}

void GHRio::Start()
{

	std::string meshName = "Rio_Run.fbx";
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();

	renderer_->SetFBXMesh(meshName, "TextureDeferredLightAni");


	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	renderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	//renderer_->CreateFBXAnimation("Run", "Rio_Run.fbx");
	//renderer_->CreateFBXAnimation("Wait", "Rio_Wait.fbx");

	//renderer_->ChangeFBXAnimation("Wait");
}

void GHRio::Update(float _deltaTime)
{
	//if (GameEngineInput::GetInst().Press("W"))
	//{
	//	renderer_->ChangeFBXAnimation("Run");
	//}
	//else
	//{
	//	renderer_->ChangeFBXAnimation("Wait");
	//}
}
