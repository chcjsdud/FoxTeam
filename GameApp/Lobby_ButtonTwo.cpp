#include "Precompile.h"
#include "Lobby_ButtonTwo.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"

Lobby_ButtonTwo::Lobby_ButtonTwo()
{
}

Lobby_ButtonTwo::~Lobby_ButtonTwo()
{
}

bool Lobby_ButtonTwo::MouseCollisionCheck()
{
	return ButtonTwoCollision->Collision(static_cast<int>(eCollisionGroup::MousePointer));
}

void Lobby_ButtonTwo::Start()
{
	{
		ButtonTwoRenderer = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
		ButtonTwoRenderer->SetImage("Lobby_BasicButtonTwo.png", "PointSmp");
		//이미지 크기는 126, 두 이미지의 거리는 8
		ButtonTwoRenderer->GetTransform()->SetLocalPosition({ 554.0f,330.0f, -102.0f });
		ButtonTwoRenderer->GetTransform()->SetLocalScaling(ButtonTwoRenderer->GetCurrentTexture()->GetTextureSize());
	}
	
	{
		ButtonTwoCollision = CreateTransformComponent<GameEngineCollision>();
		ButtonTwoCollision->GetTransform()->SetLocalPosition({ 554.0f,330.0f, -102.0f });
		ButtonTwoCollision->GetTransform()->SetLocalScaling(ButtonTwoRenderer->GetCurrentTexture()->GetTextureSize());
		ButtonTwoCollision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);
	}

}

void Lobby_ButtonTwo::Update(float _DeltaTime)
{
	GetLevel()->PushDebugRender(ButtonTwoRenderer->GetTransform(), CollisionType::Rect);
}
