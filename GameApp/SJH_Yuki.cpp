#include "PreCompile.h"
#include "SJH_Yuki.h"

#include <GameEngine/GameEngineFBXMesh.h>
#include <GameEngine/GameEngineFBXRenderer.h>

void SJH_Yuki::Start()
{
	GetLevel()->GetMainCameraActor()->GetTransform()->AttachTransform(GetTransform());
	GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalPosition({ 0.0f, 10.0f, -20.0f });
	GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalRotationDegree({ 10.0f, 0.0f, 0.0f });

	// FBX Files 경로 지정
	GameEngineDirectory Directory;
	Directory.MoveParent("FoxTeam");
	Directory.MoveChild("Resources");
	Directory.MoveChild("FBX");
	Directory.MoveChild("SJH");

	// 메쉬로드
	Mesh_ = GameEngineFBXMeshManager::GetInst().Load(Directory.PathToPlusFileName("Yuki.fbx"));
	Mesh_->CreateRenderingBuffer();

	// 애니메이션 로드
	//GameEngineFBXAnimation* Animation = nullptr;
	//Animation = GameEngineFBXAnimationManager::GetInst().Load(Directory.PathToPlusFileName("Yuki_01.fbx"));
	//Animation = GameEngineFBXAnimationManager::GetInst().Load(Directory.PathToPlusFileName("Yuki_01.fbx"));

	// 렌더러 생성
	Renderer_ = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	Renderer_->SetFBXMesh("Yuki.fbx", "TextureDeferredLight");
	Renderer_->GetTransform()->SetLocalScaling(float4(10.f, 10.f, 10.f));

	//Renderer_->CreateFBXAnimation("Idle", "Yuki_01.fbx", 1);
	//Renderer_->ChangeFBXAnimation("Idle");

	// 상태 생성




}

void SJH_Yuki::Update(float _DeltaTime)
{
}

SJH_Yuki::SJH_Yuki()
	: Mesh_(nullptr)
	, Renderer_(nullptr)
{
}

SJH_Yuki::~SJH_Yuki()
{
}
