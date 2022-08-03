#include "PreCompile.h"
#include "LGH_Aya.h"
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

LGH_Aya::LGH_Aya() // default constructer 디폴트 생성자
	:FBXRenderer_(nullptr)
{

}

LGH_Aya::~LGH_Aya() // default destructer 디폴트 소멸자
{

}

LGH_Aya::LGH_Aya(LGH_Aya&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

void LGH_Aya::Start()
{
	FBXFolder.MoveParent("FoxTeam");
	FBXFolder.MoveChild("Resources");
	FBXFolder.MoveChild("FBX");
	FBXFolder.MoveChild("LGH");

	GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(FBXFolder.PathToPlusFileName("LGH_Aya.fbx"));
	Mesh->CreateRenderingBuffer();

	{
		std::string MeshName = "LGH_Aya.FBX";


		FBXRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());

		FBXRenderer_->SetFBXMeshRenderSet("LGH_Aya.fbx", "TextureDeferredLight", 0); //body
		FBXRenderer_->SetFBXMeshRenderSet("LGH_Aya.fbx", "TextureDeferredLight", 1); //face
		//Renderer->SetFBXMeshRenderSet("LGH_Aya.fbx", "TextureDeferredLight", 2); //glass
		FBXRenderer_->SetFBXMeshRenderSet("LGH_Aya.fbx", "TextureDeferredLight", 3); //hair
		//Renderer->SetFBXMeshRenderSet("LGH_Aya.fbx", "TextureDeferredLight", 4); //Gun


		FBXRenderer_->GetTransform()->SetLocalScaling({ 100.0f, 100.0f, 100.0f });
		FBXRenderer_->GetTransform()->SetWorldRotationDegree({ 0.0f, 180.0f, 0.0f });

		FBXRenderer_->GetRenderSet(0).ShaderHelper->SettingTexture("DiffuseTex", "Aya_01_Body.Png");
		FBXRenderer_->GetRenderSet(1).ShaderHelper->SettingTexture("DiffuseTex", "Aya_01_Body.Png");
		FBXRenderer_->GetRenderSet(2).ShaderHelper->SettingTexture("DiffuseTex", "Aya_01_Face.Png");
		FBXRenderer_->GetRenderSet(3).ShaderHelper->SettingTexture("DiffuseTex", "Aya_01_Face.Png");
		FBXRenderer_->GetRenderSet(4).ShaderHelper->SettingTexture("DiffuseTex", "Aya_01_Hair.Png");
		FBXRenderer_->GetRenderSet(5).ShaderHelper->SettingTexture("DiffuseTex", "Aya_01_Hair.Png");


		GameEngineFBXAnimation* Animation = GameEngineFBXAnimationManager::GetInst().Load(FBXFolder.PathToPlusFileName("LGH_Aya.fbx"));

		for (size_t i = 0; i < Animation->AnimationCount(); i++)
		{
			auto Data = Animation->GetAnimationData(i);
		}

		FBXRenderer_->CreateFBXAnimation("Run", "LGH_Aya.fbx", 0);
		FBXRenderer_->CreateFBXAnimation("Idle", "LGH_Aya.fbx", 1);
	}
}



void LGH_Aya::Update(float _DeltaTime)
{
	FBXRenderer_->ChangeFBXAnimation("Run");
}