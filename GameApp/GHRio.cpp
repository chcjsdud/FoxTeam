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

	//int count = renderer_->GetRenderSetCount();

	//for (int i = 0; i < count; i++)
	//{
	//	renderer_->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Rio_000_LOD1.png");
	//}

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
}

void GHRio::Update(float _deltaTime)
{
}
