#include "PreCompile.h"
#include "HyperLoop.h"
#include <GameEngine/GameEngineCollision.h>

HyperLoop::HyperLoop()
	: renderer_(nullptr)
	, collision_(nullptr)

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

	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->GetTransform()->SetLocalScaling({ 150.0f , 250.0f, 150.0f });
	collision_->GetTransform()->SetLocalMove({ 0.0f , 125.0f, 0.0f });
	collision_->SetCollisionGroup(eCollisionGroup::HyperLoop);
	collision_->SetCollisionType(CollisionType::OBBBox3D);
}

void HyperLoop::Update(float _DeltaTime)
{
	if (nullptr != collision_ &&
		collision_->IsUpdate())
	{
		GetLevel()->PushDebugRender(collision_->GetTransform(), CollisionType::OBBBox3D, float4::BLACK);
	}
}

