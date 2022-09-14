#include "Precompile.h"
#include "Lobby_StartButton.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"

Lobby_StartButton::Lobby_StartButton()
{
}

Lobby_StartButton::~Lobby_StartButton()
{
}

bool Lobby_StartButton::MouseCollisionCheck()
{
	return ButtonCollision->Collision(static_cast<int>(CollisionGroup::MousePointer));
}

void Lobby_StartButton::Start()
{
	{
		ButtonRenderer = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
		ButtonRenderer->SetImage("Lobby_BasicButtonOne.png", "PointSmp");
		ButtonRenderer->GetTransform()->SetLocalPosition({ 420.0f, 330.0f, -102.0f });
		ButtonRenderer->GetTransform()->SetLocalScaling(ButtonRenderer->GetCurrentTexture()->GetTextureSize());

		ButtonCollision = CreateTransformComponent<GameEngineCollision>();
		ButtonCollision->GetTransform()->SetLocalPosition({ 420.0f, 330.0f, -102.0f });
		ButtonCollision->GetTransform()->SetLocalScaling(ButtonRenderer->GetCurrentTexture()->GetTextureSize());
		ButtonCollision->SetCollisionInfo(static_cast<int>(CollisionGroup::UI), CollisionType::Rect);
	}

}

void Lobby_StartButton::Update(float _DeltaTime)
{
	GetLevel()->PushDebugRender(ButtonRenderer->GetTransform(), CollisionType::Rect);
}
