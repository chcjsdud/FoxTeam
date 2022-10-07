#include "PreCompile.h"
#include "RioArrow.h"

#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineCollision.h>

RioArrow::RioArrow()
	: renderer_(nullptr)
	, collision_(nullptr)
{

}

RioArrow::~RioArrow()
{

}

void RioArrow::Start()
{
	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->SetCollisionGroup(eCollisionGroup::Projectile);
	collision_->SetCollisionType(CollisionType::OBBBox3D);
	collision_->GetTransform()->SetLocalScaling(50.f);
}

void RioArrow::Update(float _deltaTime)
{
	level_->PushDebugRender(collision_->GetTransform(), CollisionType::OBBBox3D);
}
