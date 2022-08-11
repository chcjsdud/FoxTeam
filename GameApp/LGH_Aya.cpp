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
		std::string MeshName = "LGH_Aya.fbx";


		FBXRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());

		FBXRenderer_->SetFBXMesh(MeshName, "TextureDeferredLightAni");

		for (UINT i = 0; i < FBXRenderer_->GetRenderSetCount(); i++)
		{
			FBXRenderer_->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex" , "Aya_01_LOD1.png");
		}

		FBXRenderer_->GetTransform()->SetLocalScaling({ 100.0f, 100.0f, 100.0f });
		FBXRenderer_->GetTransform()->SetWorldRotationDegree({ -90.0f, 180.0f, 0.0f });


		GameEngineFBXAnimation* Animation = GameEngineFBXAnimationManager::GetInst().Load(FBXFolder.PathToPlusFileName("LGH_Aya.fbx"));

		for (size_t i = 0; i < Animation->AnimationCount(); i++)
		{
			auto Data = Animation->GetAnimationData(i);
		}

		FBXRenderer_->CreateFBXAnimation("Run", "LGH_Aya.fbx", 0);
		FBXRenderer_->CreateFBXAnimation("Wait", "LGH_Aya.fbx", 1);
		FBXRenderer_->ChangeFBXAnimation("Run");
	}
}



void LGH_Aya::Update(float _DeltaTime)
{
	
}