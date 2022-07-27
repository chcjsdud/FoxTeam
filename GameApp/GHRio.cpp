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
	std::string meshName = "Rio_000.fbx";
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer_->SetFBXMesh(meshName, "TextureDeferredLight");

	renderer_->GetTransform()->SetLocalScaling({ 10.f, 10.f, 10.f });
}

void GHRio::Update(float _deltaTime)
{
}
