#include "PreCompile.h"
#include "PJW_Map.h"
#include "PJW_Enum.h"
#include <GameEngine/GameEngineFBXRenderer.h>

#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineImageRenderer.h>

PJW_Map::PJW_Map() // default constructer 디폴트 생성자
{

}

PJW_Map::~PJW_Map() // default destructer 디폴트 소멸자
{

}

PJW_Map::PJW_Map(PJW_Map&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{
		
}


void PJW_Map::Start()
{
	GetTransform()->SetWorldPosition({ 0.0f, -50.0f, 0.0f });
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	renderer_->SetFBXMesh("Bg_NaviMesh_Cobalt.fbx", "TextureDeferredLight");

	int count = renderer_->GetRenderSetCount();
	for (int i = 0; i < count; i++)
	{
		renderer_->GetRenderSet(i).PipeLine_->SetRasterizer("EngineBaseRasterizerWireFrame");
	}

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
}