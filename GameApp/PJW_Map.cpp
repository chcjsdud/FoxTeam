#include "PreCompile.h"
#include "PJW_Map.h"
#include "PJW_Enum.h"
#include <GameEngine/GameEngineFBXRenderer.h>

#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineImageRenderer.h>

PJW_Map::PJW_Map() // default constructer ����Ʈ ������
{

}

PJW_Map::~PJW_Map() // default destructer ����Ʈ �Ҹ���
{

}

PJW_Map::PJW_Map(PJW_Map&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
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