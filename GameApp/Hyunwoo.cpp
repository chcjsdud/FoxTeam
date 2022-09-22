#include "PreCompile.h"
#include "Hyunwoo.h"
#include "PlayerInfoManager.h"

Hyunwoo::Hyunwoo()
{

}

Hyunwoo::~Hyunwoo()
{

}

void Hyunwoo::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer_->SetFBXMesh("hyunwoo_run.fbx", "TextureDeferredLightAni");

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	renderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	renderer_->CreateFBXAnimation("Run", "hyunwoo_run.fbx", 0);
	renderer_->CreateFBXAnimation("Wait", "hyunwoo_wait.fbx", 0);
	renderer_->ChangeFBXAnimation("Wait");
}

void Hyunwoo::changeAnimationRun()
{
	//PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	//pm->GetPlayerList()[pm->GetMyNumber()].curAnimation_ = "Run";
	curAnimation_ = "Run";
	renderer_->ChangeFBXAnimation("Run");
}

void Hyunwoo::changeAnimationWait()
{
	//PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	//pm->GetPlayerList()[pm->GetMyNumber()].curAnimation_ = "Wait";
	curAnimation_ = "Wait";
	renderer_->ChangeFBXAnimation("Wait");
}

void Hyunwoo::changeAnimationBasicAttack()
{
}

