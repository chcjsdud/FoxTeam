#include "Precompile.h"
#include "OutLineTestActor.h"

#include <GameEngine/GameEngineOutlineRenderer.h>

void OutLineTestActor::Start()
{
	TestRenderer_ = CreateTransformComponent<GameEngineOutlineRenderer>(GetTransform());
}

void OutLineTestActor::Update(float _deltaTime)
{
}

OutLineTestActor::OutLineTestActor()
	: TestRenderer_(nullptr)
{
}

OutLineTestActor::~OutLineTestActor()
{
}
