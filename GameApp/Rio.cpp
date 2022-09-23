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

	renderer_->CreateFBXAnimation("Run", "Rio_Run.fbx", 0);
	renderer_->CreateFBXAnimation("Wait", "Rio_Wait.fbx", 0);
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
	// Character Ŭ������ �� ��� curAnimation_ �� �߰��Ǿ����ϴ�.
	// 

	renderer_->ChangeFBXAnimation("Run");
}

void Rio::changeAnimationBasicAttack()
{
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


