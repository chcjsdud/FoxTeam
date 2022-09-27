#include "PreCompile.h"
#include "Hyunwoo.h"
#include "PlayerInfoManager.h"
#include <GameEngine/GameEngineCollision.h>


Hyunwoo::Hyunwoo()
	: timer_collision_Q(0.0f), timer_end_Q(0.0f), collision_Q(nullptr), b_Qhit_(false)
{

}

Hyunwoo::~Hyunwoo()
{

}

void Hyunwoo::LoadResource()
{
	GameEngineDirectory dir;

	dir.MoveParent("FoxTeam");
	dir / "Resources" / "FBX" / "PJW";

	GameEngineFBXMesh* mesh = GameEngineFBXMeshManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_run.fbx"));
	mesh->CreateRenderingBuffer();

	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_run.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_wait.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_death.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_atk0.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_atk1.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_skillQ.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_skillE_start.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_skillE_loop.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_skillE_end.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_skillR_start.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_skillR_loop.fbx"));
	GameEngineFBXAnimationManager::GetInst().Load(dir.PathToPlusFileName("hyunwoo_skillR_end.fbx"));
}

void Hyunwoo::ReleaseResource()
{
	GameEngineFBXMeshManager::GetInst().Delete("hyunwoo_run.fbx");

	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_run.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_wait.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_death.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_atk0.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_atk1.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_skillQ.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_skillE_start.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_skillE_loop.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_skillE_end.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_skillR_start.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_skillR_loop.fbx");
	GameEngineFBXAnimationManager::GetInst().Delete("hyunwoo_skillR_end.fbx");

}

void Hyunwoo::initRendererAndAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer_->SetFBXMesh("hyunwoo_run.fbx", "TextureDeferredLightAni");

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	renderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	renderer_->CreateFBXAnimation("Run", "hyunwoo_run.fbx", 0);
	renderer_->CreateFBXAnimation("Wait", "hyunwoo_wait.fbx", 0);
	renderer_->CreateFBXAnimation("Death", "hyunwoo_death.fbx", 0, false);
	renderer_->CreateFBXAnimation("Atk0", "hyunwoo_atk0.fbx", 0, false);
	renderer_->CreateFBXAnimation("Atk1", "hyunwoo_atk1.fbx", 0, false);
	renderer_->CreateFBXAnimation("SkillQ", "hyunwoo_skillQ.fbx", 0, false);
	renderer_->CreateFBXAnimation("SkillE_start", "hyunwoo_skillE_start.fbx", 0, false);
	renderer_->CreateFBXAnimation("SkillE_loop", "hyunwoo_skillE_loop.fbx", 0);
	renderer_->CreateFBXAnimation("SkillE_end", "hyunwoo_skillE_end.fbx", 0, false);
	renderer_->CreateFBXAnimation("SkillR_start", "hyunwoo_skillR_start.fbx", 0, false);
	renderer_->CreateFBXAnimation("SkillR_loop", "hyunwoo_skillR_loop.fbx", 0);
	renderer_->CreateFBXAnimation("SkillR_end", "hyunwoo_skillR_end.fbx", 0, false);

	renderer_->ChangeFBXAnimation("Wait");
}

void Hyunwoo::changeAnimationRun()
{
	curAnimation_ = "Run";
	renderer_->ChangeFBXAnimation("Run");
}

void Hyunwoo::changeAnimationWait()
{
	curAnimation_ = "Wait";
	renderer_->ChangeFBXAnimation("Wait");
}

void Hyunwoo::changeAnimationBasicAttack()
{
	curAnimation_ = "Atk0";
	renderer_->ChangeFBXAnimation("Atk0");
}

void Hyunwoo::onStartQSkill()
{
}

void Hyunwoo::onUpdateQSkill(float _deltaTime)
{
	timer_collision_Q += _deltaTime;
	timer_end_Q += _deltaTime;

	if (0.3f <= timer_collision_Q && false == b_Qhit_)
	{
		// 여기서 피격 충돌 판정이 나옴
		collision_Q->On();
		b_Qhit_ = true;
	}

	if (true == b_Qhit_)
	{
		collision_Q->Off();
	}

	if (1.05f <= timer_end_Q)
	{
		// 모든 카운터 초기화
		timer_collision_Q = 0.0f;
		timer_end_Q = 0.0f;
		b_Qhit_ = false;
		normalState_ << "Watch";
		return;
	}
	// 델타타임에 걸맞게 
	/*피해량:50/100/150/200/250(+공격력의 40%)(+스킬 증폭의 55%)
	이동 속도 감소: -40%
	사정거리: 3m
	스테미너 소모: 60/70/80/90/100
	시전 시간: 즉시
	둔화 지속 시간: 2초
	쿨다운: 8.5/7.5/6.5/5.5/4.5초*/


	// 0.3초 쯤의 시전딜레이가 확인됨.
	// 정확히 말하자면 즉발은 맞지만, 타격 이펙트가 생기는 부분은 0.3초 이후

}

void Hyunwoo::onStartWSkill()
{
}

void Hyunwoo::onUpdateWSkill(float _deltaTime)
{
}

void Hyunwoo::onStartESkill()
{
}

void Hyunwoo::onUpdateESkill(float _deltaTime)
{
}

void Hyunwoo::onStartRSkill()
{
}

void Hyunwoo::onUpdateRSkill(float _deltaTime)
{
}

void Hyunwoo::onStartDSkill()
{
}

void Hyunwoo::onUpdateDSkill(float _deltaTime)
{
}

void Hyunwoo::InitHyunwooCollision()
{
	collision_Q = CreateTransformComponent<GameEngineCollision>(GetTransform());
	collision_Q->GetTransform()->SetLocalScaling(120.0f);
	collision_Q->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 50.0f });
	collision_Q->SetCollisionGroup(CollisionGroup::PlayerAttack);
	collision_Q->SetCollisionType(CollisionType::AABBBox3D);
	collision_Q->Off();
}
