#include "PreCompile.h"
#include "Rio.h"
#include "MousePointer.h"
#include "LumiaMap.h"

#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineLevelControlWindow.h>
#include <GameApp/LumiaLevel.h>

Rio::Rio()
	: Character()
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

	GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("Rio_Run.fbx"));
	mesh->CreateRenderingBuffer();


	std::vector<GameEngineFile> allFile = dir.GetAllFile("fbx");
	for (GameEngineFile& file : allFile)
	{
		GameEngineFBXAnimationManager::GetInst().Load(file.GetFullPath());
	}
}

void Rio::ReleaseResource()
{
	GameEngineDirectory dir;
	dir.MoveParent("FoxTeam");
	dir / "Resources" / "FBX" / "Character" / "Rio";

	std::vector<GameEngineFile> allFile = dir.GetAllFile("fbx");
	for (GameEngineFile& file : allFile)
	{
		GameEngineFBXAnimationManager::GetInst().Delete(file.GetFileName());
	}

	GameEngineFBXMeshManager::GetInst().Delete("Rio_Run.fbx");
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
	renderer_->SetFBXMesh("Rio_Run.fbx", "TextureDeferredLightAni");

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	renderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	renderer_->CreateFBXAnimation("Run", "Rio_Short_Run.fbx", 0);
	renderer_->CreateFBXAnimation("Wait", "Rio_Wait.fbx", 0);
	renderer_->CreateFBXAnimation("BasicAttack", "Rio_Short_Attack.fbx");
	renderer_->ChangeFBXAnimation("Wait");
}

void Rio::changeAnimationWait()
{
	curAnimation_ = "Wait";
	renderer_->ChangeFBXAnimation("Wait");
}

void Rio::changeAnimationRun()
{
	curAnimation_ = "Run";
	// Character 클래스에 새 멤버 curAnimation_ 이 추가되었습니다.
	// 

	renderer_->ChangeFBXAnimation("Run");
}

void Rio::changeAnimationBasicAttack()
{
	curAnimation_ = "BasicAttack";
	renderer_->ChangeFBXAnimation("BasicAttack", true);
}


void Rio::onStartQSkill()
{
}

void Rio::onUpdateQSkill(float _deltaTime)
{
}

void Rio::onStartWSkill()
{
}

void Rio::onUpdateWSkill(float _deltaTime)
{
}

void Rio::onStartESkill()
{
}

void Rio::onUpdateESkill(float _deltaTime)
{
}

void Rio::onStartRSkill()
{
}

void Rio::onUpdateRSkill(float _deltaTime)
{
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


