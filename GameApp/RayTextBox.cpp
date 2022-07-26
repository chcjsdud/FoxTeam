#include "PreCompile.h"
#include "RayTextBox.h"

#include <GameEngine/GameEngineCollision.h>

RayTextBox::RayTextBox()
	: collision_(nullptr)
{

}

RayTextBox::~RayTextBox()
{

}

void RayTextBox::Start()
{
	collision_ = CreateTransformComponent<GameEngineCollision>();
}

void RayTextBox::Update(float _deltaTime)
{
}
