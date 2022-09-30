#include "PreCompile.h"
#include "TestOBB.h"

TestOBB::TestOBB()
	: collision_(nullptr)
{

}

TestOBB::~TestOBB()
{

}

void TestOBB::Start()
{
	tc2_ = CreateTransformComponent<GameEngineTransformComponent>();
	tc2_->GetTransform()->SetLocalPosition({ 0.f, 0.f, 300.f });




	collision_ = CreateTransformComponent<GameEngineCollision>(nullptr);
	collision_->GetTransform()->SetLocalScaling(100.f);
	collision_->SetCollisionGroup(0);
	collision_->SetCollisionType(CollisionType::AABBBox3D);

	tc_ = CreateTransformComponent<GameEngineTransformComponent>();
	tc_->GetTransform()->SetLocalScaling(100.f);

	collision2_ = CreateTransformComponent<GameEngineCollision>(nullptr);
	collision2_->GetTransform()->SetLocalPosition({ 0.f, 0.f, 300.f });
	collision2_->GetTransform()->SetLocalScaling(100.f);
	collision2_->SetCollisionGroup(0);
	collision2_->SetCollisionType(CollisionType::AABBBox3D);


	GameEngineInput::GetInst().CreateKey("Left", VK_LEFT);
	GameEngineInput::GetInst().CreateKey("Right", VK_RIGHT);
}

void TestOBB::Update(float _deltaTime)
{
	collision_->GetTransform()->SetWorldPosition(tc2_->GetTransform()->GetWorldPosition());

	if (GameEngineInput::Press("Left"))
	{
		transform_.SetWorldMove({ -100.f * _deltaTime, 0.0f, 0.0f });
		//transform_.AddLocalRotationDegreeY(-180.f * _deltaTime);
	}

	if (GameEngineInput::Press("Right"))
	{
		transform_.SetWorldMove({ 100.f * _deltaTime, 0.0f, 0.0f });
		//transform_.AddLocalRotationDegreeY(180.f * _deltaTime);
	}

	GetLevel()->PushDebugRender(tc_->GetTransform(), CollisionType::OBBBox3D, float4::RED);


	if (nullptr != collision_->GetCollision(0))
	{
		GetLevel()->PushDebugRender(collision_->GetTransform(), CollisionType::OBBBox3D, float4::GREEN);
	}
	else
	{
		GetLevel()->PushDebugRender(collision_->GetTransform(), CollisionType::OBBBox3D, float4::BLUE);
	}


	GetLevel()->PushDebugRender(collision2_->GetTransform(), CollisionType::OBBBox3D, float4::BLUE);
}
