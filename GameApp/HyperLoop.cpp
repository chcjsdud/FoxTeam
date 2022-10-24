#include "PreCompile.h"
#include "HyperLoop.h"

HyperLoop::HyperLoop()
{

}

HyperLoop::~HyperLoop()
{

}

void HyperLoop::Start()
{
	static bool bLoad = false;

	if (false == bLoad)
	{
		GameEngineDirectory dir;

		dir.MoveParent("FoxTeam");
		dir / "Resources" / "FBX" / "Map";

		GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Hyperloop.fbx"));
		mesh->CreateRenderingBuffer();

		bLoad = true;
	}

	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer_->SetFBXMesh("Hyperloop.fbx", "TextureDeferredLight");
	renderer_->GetTransform()->SetLocalScaling(100.0f);

}

void HyperLoop::Update(float _DeltaTime)
{
}

