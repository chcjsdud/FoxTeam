#include "PreCompile.h"
#include "SJH_FloorMap.h"

#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

void SJH_FloorMap::Start()
{
	// 메쉬 로드
	//GameEngineDirectory Directory;
	//Directory.MoveParent("FoxTeam");
	//Directory.MoveChild("Resources");
	//Directory.MoveChild("FBX");

	//Mesh_ = GameEngineFBXMeshManager::GetInst().Load(Directory.PathToPlusFileName("Map_Lumia_01.fbx"));
	//Mesh_->CreateRenderingBuffer();

	//GameEngineFBXRenderer* Renderer = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	//Renderer->SetFBXMesh("Map_Lumia_01.fbx", "TextureDeferredLight");
	//Renderer->GetTransform()->SetLocalScaling(float4());

	// 테스트용 충돌체
	Collider_ = CreateTransformComponent<GameEngineCollision>();
	Collider_->GetTransform()->SetLocalScaling({ 2000.f,1.f,2000.f });
}

void SJH_FloorMap::Update(float _DeltaTime)
{
#ifdef _DEBUG
	if (nullptr != Collider_)
	{
		GetLevel()->PushDebugRender(Collider_->GetTransform(), CollisionType::AABBBox3D, float4::RED);
	}
#endif // _DEBUG




	
}

SJH_FloorMap::SJH_FloorMap()
	: Collider_(nullptr)
	, Mesh_(nullptr)
{
}

SJH_FloorMap::~SJH_FloorMap()
{
}
