#include "PreCompile.h"
#include "Rio.h"
#include "MousePointer.h"
#include "LumiaMap.h"

#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineLevelControlWindow.h>
#include <GameApp/LumiaLevel.h>

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

	renderer_->CreateFBXAnimation("Run", "Rio_Short_Run.UserAnimation", 0);
	renderer_->CreateFBXAnimation("Run_Long", "Rio_Long_Run.UserAnimation", 0);
	renderer_->CreateFBXAnimation("Wait", "Rio_Short_Wait.UserAnimation", 0);
	renderer_->CreateFBXAnimation("BasicAttack", "Rio_Short_Attack.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillQ_Short", "Rio_Short_Skill_Q.UserAnimation", 0, false);
	renderer_->CreateFBXAnimation("SkillQ_Long", "Rio_Long_Skill_Q.UserAnimation", 0, false);
	renderer_->ChangeFBXAnimation("Wait");
}

void Rio::changeAnimationWait()
{
	curAnimationName_ = "Wait";
	renderer_->ChangeFBXAnimation("Wait");
}

void Rio::changeAnimationRun()
{
	curAnimationName_ = "Run";
	// Character Ŭ������ �� ��� curAnimationName_ �� �߰��Ǿ����ϴ�.
	// 

	renderer_->ChangeFBXAnimation("Run");
}

void Rio::changeAnimationBasicAttack()
{
	curAnimationName_ = "BasicAttack";
	renderer_->ChangeFBXAnimation("BasicAttack", true);
}


void Rio::onStartQSkill()
{
	overrideAnimationName_ = "SkillQ_Long";
	overrideAnimationBoneName_ = "Bip001 Spine2";
	renderer_->OverrideFBXAnimation("SkillQ_Long", "Bip001 Spine2");
	//renderer_->OverrideFBXAnimation("SkillQ", "Bip001 L UpperArm");
}

void Rio::onUpdateQSkill(float _deltaTime)
{
	moveProcess(_deltaTime);
	if (renderer_->IsOverrideAnimationEnd())
	{
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


