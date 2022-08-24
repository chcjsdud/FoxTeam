#include "PreCompile.h"
#include "PJW_Hyunwoo.h"
#include "PJW_Enum.h"
#include "PJW_Level.h"
#include "PJW_Mouse.h"
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineLevel.h>
#include <GameEngine/GameEngineFBXAnimation.h>

PJW_Hyunwoo::PJW_Hyunwoo() // default constructer ����Ʈ ������
	:FBXRenderer_(nullptr)
{

}

PJW_Hyunwoo::~PJW_Hyunwoo() // default destructer ����Ʈ �Ҹ���
{

}

PJW_Hyunwoo::PJW_Hyunwoo(PJW_Hyunwoo&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}

void PJW_Hyunwoo::Start()
{
	GameEngineInput::GetInst().CreateKey("M1", VK_LBUTTON);

	{
		// �������� ���ؽ�, �ε��� ���� ����
		GameEngineDirectory dir;
		dir.MoveParent("FoxTeam");
		dir.MoveChild("Resources");
		dir.MoveChild("FBX");
		//dir.MoveChild("UserMesh");
		dir.MoveChild("PJW");

		GameEngineDirectory dir1;
		dir1.MoveParent("FoxTeam");
		dir1.MoveChild("Resources");
		dir1.MoveChild("FBX");
		dir1.MoveChild("UserAni");

		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_01_LOD1.fbx"));
		GameEngineFBXAnimation* animationQ = GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_01_LOD1.fbx"));
		//GameEngineFBXAnimation* animationW = GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_OnlyRun.fbx"));
		//GameEngineFBXAnimation* animationE = GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_OnlyQ.fbx"));
		//GameEngineFBXAnimation* animationR = GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_OnlyD.fbx"));

		Mesh->CreateRenderingBuffer();

		FBXRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());

		FBXRenderer_->SetFBXMesh(Mesh->GetName(), "TextureDeferredLightAni");


		for (size_t i = 0; i < FBXRenderer_->GetRenderSetCount(); i++)
		{
			FBXRenderer_->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Hyunwoo_01_LOD1.png");
		}

		FBXRenderer_->GetTransform()->SetLocalPosition({ 0.0f, -50.0f, 0.0f });
		FBXRenderer_->GetTransform()->SetLocalScaling({ 30.0f, 30.0f, 30.0f });
		FBXRenderer_->GetTransform()->SetLocalRotationDegree({ -90.0f, 0.0f, 0.0f });

		FBXRenderer_->CreateFBXAnimation("1", animationQ->GetName(), 0);
		FBXRenderer_->CreateFBXAnimation("2", animationQ->GetName(), 1);
		FBXRenderer_->CreateFBXAnimation("3", animationQ->GetName(), 2);
		FBXRenderer_->CreateFBXAnimation("4", animationQ->GetName(), 3);
		FBXRenderer_->CreateFBXAnimation("5", animationQ->GetName(), 4);
		FBXRenderer_->CreateFBXAnimation("6", animationQ->GetName(), 5);
		FBXRenderer_->ChangeFBXAnimation("1");
	}
}



//void PJW_Hyunwoo::Init_Collision()
//{
//	GetTransform()->SetLocalPosition(float4::ZERO);
//
//	////Ÿ�� ��Ʈ �ڽ�
//	collision_Body_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
//	collision_Body_->GetTransform()->SetLocalScaling(float4{ 50.f,50.f, 50.f ,1.f });
//
//	collision_Body_->SetCollisionType(CollisionType::AABBBox3D);
//
//	////�ǰ� ��Ʈ�ڽ� ����� �� �ø����� �����ҵ�
//	collision_Attack_ = CreateTransformComponent<GameEngineCollision>(GetTransform());
//	collision_Attack_->GetTransform()->SetLocalScaling(float4{ 100.f,0.f,100.f ,1.f });
//	collision_Attack_->GetTransform()->SetLocalPosition({ 0.f,-50.f,0.f });
//
//	collision_Attack_->SetCollisionType(CollisionType::CirCle);
//}


void PJW_Hyunwoo::Update(float _DeltaTime)
{
	if (true == GameEngineInput::GetInst().Down("Q"))
	{
		FBXRenderer_->ChangeFBXAnimation("1", true);
	}

	if (true == GameEngineInput::GetInst().Down("W"))
	{
		FBXRenderer_->ChangeFBXAnimation("2", true);
	}

	if (true == GameEngineInput::GetInst().Down("E"))
	{
		FBXRenderer_->ChangeFBXAnimation("3", true);
	}

	if (true == GameEngineInput::GetInst().Down("R"))
	{
		FBXRenderer_->ChangeFBXAnimation("4", true);
	}

}	