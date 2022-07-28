#include "PreCompile.h"
#include "PJW_Level.h"


#include "GameEngine/SKySphereActor.h"
#include "PJW_Hyunwoo.h"
#include "PJW_Map.h"
#include "GameEngine/LightActor.h"

PJW_Level::PJW_Level() // default constructer ����Ʈ ������
{

}

PJW_Level::~PJW_Level() // default destructer ����Ʈ �Ҹ���
{

}

PJW_Level::PJW_Level(PJW_Level&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}



void PJW_Level::LevelStart()
{
	GetMainCamera()->SetProjectionMode(ProjectionMode::Perspective);
	GetMainCameraActor()->GetTransform()->SetLocalPosition(float4(0.0f, 0.0f, -100.0f));

	Init_Actors();
	Init_Keys();
}

void PJW_Level::LevelUpdate(float _DeltaTime)
{

}

void PJW_Level::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{

}

void PJW_Level::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{

}

void PJW_Level::Init_Actors()
{
	// �÷��̾� ĳ���� ����
	{
		std::string MeshName = "Hyunwoo_01_LOD1.FBX";
		GameEngineDirectory dir;

		dir.MoveParent("FoxTeam");
		dir.MoveChild("Resources");
		dir.MoveChild("FBX");
		dir.MoveChild("PJW");
		// ������		
		if (nullptr == GameEngineFBXMeshManager::GetInst().Find(dir.PathToPlusFileName(MeshName)))
		{
			GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName(MeshName));
			Mesh->CreateRenderingBuffer();
		}
		
		GameEngineFBXAnimation* Animation = GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("Hyunwoo_01_LOD1.FBX"));

		for (size_t i = 0; i < Animation->AnimationCount(); i++)
		{
			auto Data = Animation->GetAnimationData(i);
		}

		player_ = CreateActor<PJW_Hyunwoo>();

		PJW_Hyunwoo* enemy = CreateActor<PJW_Hyunwoo>();
		enemy->GetTransform()->SetWorldPosition({ 300.0f, 0.0f, 300.0f });

		player_->SetTarget(enemy);
	}

	// �� ����
	{
		//map_ = CreateActor<PJW_Map>();
	}

	{
		SKySphereActor* Actor = CreateActor<SKySphereActor>();
	}

	// �� ����
	{
		light_A = CreateActor<LightActor>();
		light_A->GetLight()->SetDiffusePower(0.3f);
		light_A->GetLight()->SetSpacularLightPow(50.0f);

		light_B = CreateActor<LightActor>();
		light_B->GetLight()->SetDiffusePower(0.3f);
		light_B->GetLight()->SetSpacularLightPow(50.0f);
		light_B->GetTransform()->SetLocalRotationDegree({ 45.0f, 0.0f, 0.0f });
	}
}

void PJW_Level::Init_Keys()
{
	if (false == GameEngineInput::GetInst().IsKey("Skill_Q"))
	{
		GameEngineInput::GetInst().CreateKey("Skill_Q", 'Q');
		GameEngineInput::GetInst().CreateKey("Test_Move", VK_RBUTTON);
	}
}