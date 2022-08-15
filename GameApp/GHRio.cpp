#include "PreCompile.h"
#include "GHRio.h"
#include "GHRayTestLevel.h"
#include "GHMousePointer.h"

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

	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();

	renderer_->SetFBXMesh("Rio_Wait.UserMesh", "TextureDeferredLightAni");

	for (size_t i = 0; i < renderer_->GetRenderSetCount(); i++)
	{
		renderer_->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Rio_000_LOD1.png");
	}

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	renderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	renderer_->CreateFBXAnimation("Run", "Rio_Run.UserAnimation", 0);
	renderer_->CreateFBXAnimation("Wait", "Rio_Wait.UserAnimation", 0);

	renderer_->ChangeFBXAnimation("Wait");

	GameEngineInput::GetInst().CreateKey("LButton", VK_LBUTTON);
	GameEngineInput::GetInst().CreateKey("RButton", VK_RBUTTON);

	
}

void GHRio::Update(float _deltaTime)
{
	GHRayTestLevel* level = dynamic_cast<GHRayTestLevel*>(GetLevel());
	GHMousePointer* mouse = nullptr;
	if (nullptr != level)
	{
		mouse = level->GetMousePointer();
	}

	if (GameEngineInput::GetInst().Down("LButton"))
	{
		if (nullptr != mouse)
		{
			destination_ = mouse->GetIntersectionYAxisPlane(0.0f, 1000.f);
		}
	}

	if ((destination_ - GetTransform()->GetWorldPosition()).Len3D() > 10.f)
	{
		renderer_->ChangeFBXAnimation("Run");


		direction_ = destination_ - GetTransform()->GetWorldPosition();
		direction_.Normalize3D();

		float4 cross = float4::Cross3D(direction_, { 0.0f, 0.0f, 1.0f });
		cross.Normalize3D();

		float angle = float4::DegreeDot3DToACosAngle(direction_, { 0.0f, 0.0f, 1.0f });

		GetTransform()->SetLocalRotationDegree({ 0.0f, angle * -cross.y, 0.0f });

		GetTransform()->SetWorldDeltaTimeMove(direction_ * SPEED);
	}
	else
	{
		renderer_->ChangeFBXAnimation("Wait");
	}
}

void GHRio::startIdle(float _deltaTime)
{
}

void GHRio::updateIdle(float _deltaTime)
{
}

void GHRio::startRun(float _deltaTime)
{
}

void GHRio::updateRun(float _deltaTime)
{
}
