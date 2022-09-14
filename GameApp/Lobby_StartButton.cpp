#include "Precompile.h"
#include "Lobby_StartButton.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"
#include "LobbyUIController.h"

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
	float4 ButtonPos = { -475.0f, -240.0f, -102.0f };

	ButtonRenderer = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
	ButtonRenderer->SetImage("Lobby_SelectButton_Before.png", "PointSmp");
	ButtonRenderer->GetTransform()->SetLocalPosition(ButtonPos);
	ButtonRenderer->GetTransform()->SetLocalScaling(ButtonRenderer->GetCurrentTexture()->GetTextureSize());

	ButtonCollision = CreateTransformComponent<GameEngineCollision>();
	ButtonCollision->GetTransform()->SetLocalPosition(ButtonPos);
	ButtonCollision->GetTransform()->SetLocalScaling(ButtonRenderer->GetCurrentTexture()->GetTextureSize());
	ButtonCollision->SetCollisionInfo(static_cast<int>(CollisionGroup::UI), CollisionType::Rect);
}

void Lobby_StartButton::Update(float _DeltaTime)
{
	//GetLevel()->PushDebugRender(ButtonRenderer->GetTransform(), CollisionType::Rect);

	if (true == MouseCollisionCheck())
	{
		if (true == GameEngineInput::GetInst().Down("LBUTTON"))
		{
			if (LobbyUIController::CharSelectOn == false)
			{
				ButtonRenderer->SetImage("Lobby_SelectButton_After.png", "PointSmp");
				LobbyUIController::CharSelectOn = true;
			}
			else if (LobbyUIController::CharSelectOn = true)
			{
				ButtonRenderer->SetImage("Lobby_SelectButton_Before.png", "PointSmp");
				LobbyUIController::CharSelectOn = false;
			}
			
		}
	}

}
