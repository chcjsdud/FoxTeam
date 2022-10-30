#include "Precompile.h"
#include "Lobby_ButtonOne.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"

Lobby_ButtonOne::Lobby_ButtonOne()
{
}

Lobby_ButtonOne::~Lobby_ButtonOne()
{
}

bool Lobby_ButtonOne::MouseCollisionCheck()
{
	return ButtonOneCollision->Collision(static_cast<int>(eCollisionGroup::MousePointer));
}

void Lobby_ButtonOne::Start()
{
	{
		ButtonOneRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), static_cast<int>(RenderOrder::UIPANEL0));
		ButtonOneRenderer->SetImage("Lobby_BasicButtonOne.png", "PointSmp");
		ButtonOneRenderer->GetTransform()->SetLocalPosition({ 420.0f, 330.0f});
		ButtonOneRenderer->GetTransform()->SetLocalScaling(ButtonOneRenderer->GetCurrentTexture()->GetTextureSize());

		ButtonOneCollision = CreateTransformComponent<GameEngineCollision>(GetTransform(), static_cast<int>(RenderOrder::UICOL));
		ButtonOneCollision->GetTransform()->SetLocalPosition({ 420.0f, 330.0f});
		ButtonOneCollision->GetTransform()->SetLocalScaling(ButtonOneRenderer->GetCurrentTexture()->GetTextureSize());
		ButtonOneCollision->SetCollisionInfo(static_cast<int>(eCollisionGroup::UI), CollisionType::Rect);
	}

}

void Lobby_ButtonOne::Update(float _DeltaTime)
{
	GetLevel()->PushDebugRender(ButtonOneCollision->GetTransform(), CollisionType::Rect);
}
