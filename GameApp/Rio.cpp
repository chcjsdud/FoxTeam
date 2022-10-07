#include "PreCompile.h"
#include "Rio.h"
#include "MousePointer.h"
#include "LumiaMap.h"

#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineLevelControlWindow.h>
#include <GameApp/LumiaLevel.h>
#include "RioArrow.h"

Rio::Rio()
	: Character()
	, bLongBow_(false)
{

}

Rio::~Rio()
{

}

void Rio::LoadResource()
{
	GameEngineDirectory dir;

	dir.MoveParent("FoxTeam");
	dir / "Resources" / "FBX" / "Character" / "Rio";

	GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Rio_Short_Run.fbx"));
	mesh->CreateRenderingBuffer();


	std::vector<GameEngineFile> allFile = dir.GetAllFile("UserAnimation");
	for (GameEngineFile& file : allFile)
	{
		GameEngineFBXAnimationManager::GetInst().LoadUser(file.GetFullPath());
	}
}

void Rio::ReleaseResource()
{
	GameEngineDirectory dir;
	dir.MoveParent("FoxTeam");
	dir / "Resources" / "FBX" / "Character" / "Rio";

	std::vector<GameEngineFile> allFile = dir.GetAllFile("UserAnimation");
	for (GameEngineFile& file : allFile)
	{
		GameEngineFBXAnimationManager::GetInst().Delete(file.GetFileName());
	}

	GameEngineFBXMeshManager::GetInst().Delete("Rio_Short_Run.fbx");
}

void Rio::Start()
{
	Character::Start();
}

void Rio::Update(float _deltaTime)
{
	Character::Update(_deltaTime);
}

void Rio::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer_->SetFBXMesh("Rio_Short_Run.fbx", "TextureDeferredLightAni");

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	renderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	renderer_->CreateFBXAnimation("Run_Short", "Rio_Short_Run.UserAnimation", 0);
	renderer_->CreateFBXAnimation("Run_Long", "Rio_Long_Run.UserAnimation", 0);

	renderer_->CreateFBXAnimation("Wait_Short", "Rio_Short_Wait.UserAnimation", 0);
	renderer_->CreateFBXAnimation("Wait_Long", "Rio_Long_Wait.UserAnimation", 0);

	renderer_->CreateFBXAnimation("BasicAttack_Short", "Rio_Short_Attack.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("BasicAttack_Long", "Rio_Long_Attack.UserAnimation", 0, false);

	renderer_->CreateFBXAnimation("SkillQ_Short", "Rio_Short_Skill_Q.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillQ_Long", "Rio_Long_Skill_Q.UserAnimation", 0, false);

	renderer_->CreateFBXAnimation("SkillW_Short", "Rio_Short_Skill_W.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillW_Long", "Rio_Short_Skill_W.UserAnimation", 0, false);

	renderer_->CreateFBXAnimation("SkillE_Short", "Rio_Short_Skill_W.UserAnimation", 0, false);

	//renderer_->CreateFBXAnimation("SkillW_Short", "Rio_Short_Skill_W.UserAnimation", 0, false);
	//renderer_->CreateFBXAnimation("SkillW_Long", "Rio_Short_Skill_W.UserAnimation", 0, false);

	renderer_->ChangeFBXAnimation("Wait_Short");
}

void Rio::changeAnimationWait()
{
	if (bLongBow_)
	{
		ChangeAnimation("Wait_Long");
	}
	else
	{
		ChangeAnimation("Wait_Short");
	}
}

void Rio::changeAnimationRun()
{
	if (bLongBow_)
	{
		ChangeAnimation("Run_Long");
	}
	else
	{
		ChangeAnimation("Run_Short");
	}
}

void Rio::changeAnimationBasicAttack()
{
	if (bLongBow_)
	{
		ChangeAnimation("BasicAttack_Long");
	}
	else
	{
		ChangeAnimation("BasicAttack_Short");
	}
}

void Rio::onStartBasicAttacking(Character* _target)
{
	if (bLongBow_)
	{
		float4 startPosition = transform_.GetWorldPosition();
		startPosition.y += 50.f;
		startPosition += transform_.GetWorldForwardVector() * 50.f;

		RioArrow* arrow = level_->CreateActor<RioArrow>();
		arrow->MakeTargetArrow(this, stat_.AttackPower, startPosition, 1000.f, target_);
	}
	else
	{
		float doubleStrikeDelay = (stat_.AttackEndTime - stat_.AttackStartTime) / stat_.AttackSpeed / 2.0f;

		float4 startPosition = transform_.GetWorldPosition();
		startPosition.y += 50.f;
		startPosition += transform_.GetWorldForwardVector() * 50.f;

		RioArrow* arrow = level_->CreateActor<RioArrow>();
		arrow->MakeTargetArrow(this, stat_.AttackPower / 2.0f, startPosition, 1000.f, target_);

		arrow = level_->CreateActor<RioArrow>();
		arrow->MakeTargetArrow(this, stat_.AttackPower / 2.0f, startPosition, 1000.f, target_);
		arrow->SetWaitTime(doubleStrikeDelay);
	}

}

void Rio::onUpdateBasicAttacking(Character* _target, float _deltaTime)
{
}

void Rio::onStartQSkill()
{
	overrideAnimationBoneName_ = "Bip001 Spine2";
	if (bLongBow_)
	{
		overrideAnimationName_ = "SkillQ_Long";
	}
	else
	{
		overrideAnimationName_ = "SkillQ_Short";
	}

	renderer_->OverrideFBXAnimation(overrideAnimationName_, overrideAnimationBoneName_);


	//renderer_->OverrideFBXAnimation("SkillQ", "Bip001 L UpperArm");
}

void Rio::onUpdateQSkill(float _deltaTime)
{
	inputProcess(_deltaTime);
	moveProcess(_deltaTime);
	if (renderer_->IsOverrideAnimationEnd())
	{
		bLongBow_ = !bLongBow_;

		overrideAnimationName_ = "";
		overrideAnimationBoneName_ = "";
		renderer_->ClearOverrideAnimation();
		mainState_ << "NormalState";
	}
}

void Rio::onStartWSkill()
{
}

void Rio::onUpdateWSkill(float _deltaTime)
{
	mainState_ << "NormalState";
}

void Rio::onStartESkill()
{
}

void Rio::onUpdateESkill(float _deltaTime)
{
	mainState_ << "NormalState";
}

void Rio::onStartRSkill()
{
}

void Rio::onUpdateRSkill(float _deltaTime)
{
	mainState_ << "NormalState";
}

void Rio::onStartDSkill()
{
}

void Rio::onUpdateDSkill(float _deltaTime)
{
}

void Rio::onStartDeath()
{
}

void Rio::onUpdateDeath(float _deltaTime)
{
}

