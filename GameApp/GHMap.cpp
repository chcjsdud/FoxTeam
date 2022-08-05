#include "PreCompile.h"
#include "GHMap.h"

#include <GameEngine/GameEngineFBXRenderer.h>


GHMap::GHMap()
{

}

GHMap::~GHMap()
{

}

void GHMap::Start()
{
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer_->SetFBXMesh("Bg_NaviMesh_Cobalt.fbx", "TextureDeferredLight");

	int count = renderer_->GetRenderSetCount();
	for (int i = 0; i < count; i++)
	{
		renderer_->GetRenderSet(i).PipeLine_->SetRasterizer("EngineBaseRasterizerWireFrame");
	}
	
	

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
}

void GHMap::Update(float _deltaTime)
{
}
