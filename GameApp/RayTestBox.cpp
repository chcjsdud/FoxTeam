#include "PreCompile.h"
#include "RayTestBox.h"

#include <GameEngine/GameEngineCollision.h>

RayTestBox::RayTestBox()
	: collision_(nullptr)
{

}

RayTestBox::~RayTestBox()
{

}

void RayTestBox::Start()
{
	collision_ = CreateTransformComponent<GameEngineCollision>();
}

void RayTestBox::Update(float _deltaTime)
{
}
