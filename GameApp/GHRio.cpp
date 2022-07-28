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
	renderer_->SetFBXMesh(meshName, "TextureDeferredLight");

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
}

void GHRio::Update(float _deltaTime)
{
}
