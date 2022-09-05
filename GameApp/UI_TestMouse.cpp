#include "Precompile.h"
#include "UI_TestMouse.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"

UI_TestMouse::UI_TestMouse()
{
}

UI_TestMouse::~UI_TestMouse()
{
}

void UI_TestMouse::Start()
{

	PointerCollision = CreateTransformComponent<GameEngineCollision>();
	PointerCollision->GetTransform()->SetLocalScaling(float4{ 30.0f, 30.0f, 30.0f });
	PointerCollision->SetCollisionInfo(static_cast<int>(CollisionGroup::MousePointer), CollisionType::Rect);

	MousePointer::Start();
}

void UI_TestMouse::Update(float _DeltaTime)
{
	MousePointer::Update(_DeltaTime);
	GetTransform()->SetWorldPosition(GameEngineInput::GetInst().GetMouse3DPos());
}