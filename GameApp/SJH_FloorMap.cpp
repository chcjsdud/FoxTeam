#include "PreCompile.h"
#include "SJH_FloorMap.h"

#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

void SJH_FloorMap::Start()
{
	GetTransform()->SetLocalPosition(float4(0.0f, -100.0f, 0.0f));

	//// 메쉬 로드
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
	Collider_->GetTransform()->SetLocalScaling(float4(2000.f, 100.f, 2000.f));

	// 테스트 메쉬
	Renderer_ = CreateTransformComponent<GameEngineRenderer>();
	Renderer_->SetRenderingPipeLine("ColorSphere");
	Renderer_->ShaderHelper.SettingConstantBufferLink("ResultColor", float4(1.0f, 0.0f, 1.0f, 1.0f));
	Renderer_->GetTransform()->SetLocalScaling(float4(2000.f, 100.f, 2000.f));
}

void SJH_FloorMap::Update(float _DeltaTime)
{
#ifdef _DEBUG
	if (nullptr != Renderer_)
	{
		//GetLevel()->PushDebugRender(Renderer_->GetTransform(), CollisionType::Sphere3D, float4(1.f, 0.f, 1.f));
	}

	if (nullptr != Collider_)
	{
		//GetLevel()->PushDebugRender(Collider_->GetTransform(), CollisionType::Sphere3D, float4::RED);
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
