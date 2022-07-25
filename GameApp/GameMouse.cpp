#include "PreCompile.h"
#include "GameMouse.h"
#include "Enums.h"

#include <GameEngine/GameEngineCollision.h>

GameMouse::GameMouse() // default constructer 디폴트 생성자
	: mouseCollision_(nullptr)
{

}

GameMouse::~GameMouse() // default destructer 디폴트 소멸자
{

}

void GameMouse::ImageInit()
{
	UIRenderer = CreateTransformComponent<GameEngineUIRenderer>();
	UIRenderer->GetTransform()->SetLocalPosition({ 12.0f,-10.0f,0.0f });
	UIRenderer->GetTransform()->SetLocalScaling({ 30.0f,30.0f,0.0f });
	SetCurSor("Cursor_01.png");
}

void GameMouse::CollisionInit()
{
	mouseCollision_ = CreateTransformComponent<GameEngineCollision>();
	
	mouseCollision_->GetTransform()->SetLocalScaling(float4{ 3.0f, 3.0f, 3.0f });
	mouseCollision_->SetCollisionGroup(static_cast<int>(CollisionType::MousePointer));
}

void GameMouse::Start()
{
	ImageInit();
	CollisionInit();
}

void GameMouse::Update(float _DeltaTime)
{
	GetTransform()->SetWorldPosition(GameEngineInput::GetInst().GetMouse3DPos());
}

