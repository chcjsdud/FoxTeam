#include "Precompile.h"
#include "LobbyBackground.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"

LobbyBackground::LobbyBackground()
{
}

LobbyBackground::~LobbyBackground()
{
}

void LobbyBackground::Start()
{
	{
		BackGroundRenderer = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
		BackGroundRenderer->SetImage("LobbyLevel_BG.png", "PointSmp");
	//	BackGroundRenderer->GetTransform()->SetLocalPosition(CharUI_Pos);
		BackGroundRenderer->GetTransform()->SetLocalScaling(BackGroundRenderer->GetCurrentTexture()->GetTextureSize());
	}
}

void LobbyBackground::Update(float _DeltaTime)
{
}
