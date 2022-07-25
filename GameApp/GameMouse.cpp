#include "PreCompile.h"
#include "GameMouse.h"
#include "Enums.h"

#include <GameEngine/GameEngineCollision.h>

GameMouse::GameMouse() // default constructer 디폴트 생성자
	: mouseCollision_(nullptr), XYBuffer_(float4::ZERO)
{

}

GameMouse::~GameMouse() // default destructer 디폴트 소멸자
{

}

void GameMouse::ImageInit()
{
	UIRenderer = CreateTransformComponent<GameEngineUIRenderer>();
	UIRenderer->GetTransform()->SetLocalPosition({ 12.0f,-10.0f,0.0f });
	UIRenderer->GetTransform()->SetLocalScaling({ 30.0f, 30.0f,30.0f });
	SetCurSor("Cursor_01.png");
}

void GameMouse::CollisionInit()
{
	mouseCollision_ = CreateTransformComponent<GameEngineCollision>();
	
	mouseCollision_->GetTransform()->SetLocalScaling(float4{ 30.0f, 30.0f, 30.0f });
	mouseCollision_->SetCollisionInfo(CINT(CollisionGroup::MousePointer), CollisionType::AABBBox3D);

}

void GameMouse::Start()
{
	ImageInit();
	CollisionInit();
	
}

void GameMouse::Update(float _DeltaTime)
{
	InputCheck(_DeltaTime);
	UpdateMove(_DeltaTime);
	DebugUpdate(_DeltaTime);
}

void GameMouse::InputCheck(float _DeltaTime)
{
	// 마우스 이동


}


void GameMouse::UpdateMove(float _DeltaTime)
{
	GetTransform()->SetWorldPosition(GameEngineInput::GetInst().GetMouse3DPos());
	XYBuffer_ = {GetTransform()->GetWorldPosition().x,GetTransform()->GetWorldPosition().z, GetTransform()->GetWorldPosition().y };
	mouseCollision_->GetTransform()->SetWorldPosition(XYBuffer_);
	
}

void GameMouse::DebugUpdate(float _DeltaTime)
{
	GetLevel()->PushDebugRender(mouseCollision_->GetTransform(), CollisionType::AABBBox3D);
}
