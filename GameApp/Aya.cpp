#include "PreCompile.h"
#include "Aya.h"

Aya::Aya()
{

}

Aya::~Aya()
{

}

void Aya::LoadResource()
{
	{
		GameEngineDirectory dir;

		dir.MoveParent("FoxTeam");
		dir / "Resources" / "FBX" / "Character" / "Aya";

		GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Aya_Idle.fbx"));
		mesh->CreateRenderingBuffer();

		std::vector<GameEngineFile> allFile = dir.GetAllFile("UserAnimation");
		for (GameEngineFile& file : allFile)
		{
			GameEngineFBXAnimationManager::GetInst().LoadUser(file.GetFullPath());
		}
	}

	{
		GameEngineDirectory dir;
		dir.MoveParent("FoxTeam");
		dir / "Resources" / "Sound" / "Char" / "Aya";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("wav");
		for (GameEngineFile& file : allFile)
		{
			GameEngineSoundManager::GetInstance()->CreateSound(file.FileName(), file.GetFullPath());
		}
	}
}

void Aya::ReleaseResource()
{
	{
		GameEngineDirectory dir;
		dir.MoveParent("FoxTeam");
		dir / "Resources" / "Sound" / "Char" / "Aya";

		std::vector<GameEngineFile> allFile = dir.GetAllFile("wav");
		for (GameEngineFile& file : allFile)
		{
			GameEngineSoundManager::GetInstance()->ReleaseSound(file.FileName());
		}
	}

	GameEngineDirectory dir;
	dir.MoveParent("FoxTeam");
	dir / "Resources" / "FBX" / "Character" / "Aya";

	std::vector<GameEngineFile> allFile = dir.GetAllFile("UserAnimation");
	for (GameEngineFile& file : allFile)
	{
		GameEngineFBXAnimationManager::GetInst().Delete(file.GetFileName());
	}

	GameEngineFBXMeshManager::GetInst().Delete("Aya_Idle.fbx");
}

JobType Aya::GetJobType()
{
    return JobType::AYA;
}

void Aya::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer_->SetFBXMesh("Aya_Idle.fbx", "TextureDeferredLightAni", true);

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	renderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	std::string ext = "UserAnimation";

	renderer_->CreateFBXAnimation("Idle", "Aya_Idle." +  ext, 0, true);
	renderer_->CreateFBXAnimation("Run", "Aya_Run." + ext, 0, true);
	renderer_->CreateFBXAnimation("Attack", "Aya_Attack." +  ext, 0, true);
	renderer_->CreateFBXAnimation("Reload", "Aya_Reload." +  ext, 0, true);
	renderer_->CreateFBXAnimation("SkillD", "Aya_SkillD." +  ext, 0, true);
	renderer_->CreateFBXAnimation("SkillE", "Aya_SkillE." +  ext, 0, true);
	renderer_->CreateFBXAnimation("SkillQ", "Aya_SkillQ." +  ext, 0, true);
	renderer_->CreateFBXAnimation("SkillR_End", "Aya_SkillR_End." +  ext, 0, true);
	renderer_->CreateFBXAnimation("SkillR_Start", "Aya_SkillR_Start." +  ext, 0, true);
	renderer_->CreateFBXAnimation("SkillW_Back", "Aya_SkillW_Back." +  ext, 0, true);
	renderer_->CreateFBXAnimation("SkillW_Forward", "Aya_SkillW_Forward." +  ext, 0, true);
	renderer_->CreateFBXAnimation("SkillW_Left", "Aya_SkillW_Left." +  ext, 0, true);
	renderer_->CreateFBXAnimation("SkillW_Right", "Aya_SkillW_Right." +  ext, 0, true);
	renderer_->CreateFBXAnimation("SkillW_Shot", "Aya_SkillW_Shot." +  ext, 0, true);
	renderer_->CreateFBXAnimation("SkillW_Wait", "Aya_SkillW_Wait." + ext, 0, true);
	renderer_->CreateFBXAnimation("Death", "Aya_Death." + ext, 0, true);

	renderer_->ChangeFBXAnimation("Idle");

}

void Aya::changeAnimationRun()
{
	ChangeAnimation("Run");
}

void Aya::changeAnimationWait()
{
	ChangeAnimation("Idle");
}

void Aya::changeAnimationBasicAttack()
{
	ChangeAnimation("Attack");
}

void Aya::changeDeathAnimation()
{
	ChangeAnimation("Death");
}

void Aya::onStartBasicAttacking(IUnit* _target)
{
}

void Aya::onUpdateBasicAttacking(IUnit* _target, float _deltaTime)
{
}

void Aya::onStartQSkill()
{
}

void Aya::onUpdateQSkill(float _deltaTime)
{
}

void Aya::onStartWSkill()
{
}

void Aya::onUpdateWSkill(float _deltaTime)
{
}

void Aya::onStartESkill()
{
}

void Aya::onUpdateESkill(float _deltaTime)
{
}

void Aya::onStartRSkill()
{
}

void Aya::onUpdateRSkill(float _d1eltaTime)
{
}

void Aya::onStartDSkill()
{
}

void Aya::onUpdateDSkill(float _deltaTime)
{
}

void Aya::onUpdateCustomState(float _deltaTime)
{
}

void Aya::onStartDeath()
{
}

void Aya::onUpdateDeath(float _deltaTime)
{
}

void Aya::onPlayEffect(const std::string& _effectName)
{
}
