#include "PreCompile.h"
#include "SJH_Yuki.h"

#include <GameEngine/GameEngineFBXMesh.h>
#include <GameEngine/GameEngineFBXRenderer.h>

void SJH_Yuki::Start()
{
	GetLevel()->GetMainCameraActor()->GetTransform()->AttachTransform(GetTransform());
	GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalPosition({ 0.0f, 10.0f, -20.0f });
	GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalRotationDegree({ 10.0f, 0.0f, 0.0f });

	// FBX Files ��� ����
	GameEngineDirectory Directory;
	Directory.MoveParent("FoxTeam");
	Directory.MoveChild("Resources");
	Directory.MoveChild("FBX");
	Directory.MoveChild("SJH");

	// �޽��ε�
	Mesh_ = GameEngineFBXMeshManager::GetInst().Load(Directory.PathToPlusFileName("Yuki.fbx"));
	Mesh_->CreateRenderingBuffer();

	// �ִϸ��̼� �ε�
	//GameEngineFBXAnimation* Animation = nullptr;
	//Animation = GameEngineFBXAnimationManager::GetInst().Load(Directory.PathToPlusFileName("Yuki_01.fbx"));
	//Animation = GameEngineFBXAnimationManager::GetInst().Load(Directory.PathToPlusFileName("Yuki_01.fbx"));

	// ������ ����
	Renderer_ = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	Renderer_->SetFBXMesh("Yuki.fbx", "TextureDeferredLight");
	Renderer_->GetTransform()->SetLocalScaling(float4(10.f, 10.f, 10.f));

	//Renderer_->CreateFBXAnimation("Idle", "Yuki_01.fbx", 1);
	//Renderer_->ChangeFBXAnimation("Idle");

	// ���� ����




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
