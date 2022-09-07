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
		BackGroundRenderer->GetTransform()->SetLocalPosition({0.0f,0.0f, -100.0f});
		BackGroundRenderer->GetTransform()->SetLocalScaling(BackGroundRenderer->GetCurrentTexture()->GetTextureSize());
	}

	{
		Deco_GlowingRenderer = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
		Deco_GlowingRenderer->SetImage("Lobby_CharDeco_Glowing.png", "PointSmp");
		Deco_GlowingRenderer->GetTransform()->SetLocalPosition({ 0.0f, 125.0f, -101.0f });
		Deco_GlowingRenderer->GetTransform()->SetLocalScaling(Deco_GlowingRenderer->GetCurrentTexture()->GetTextureSize());
	}

	{
		Deco_CircleRenderer = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
		Deco_CircleRenderer->SetImage("Lobby_CharDeco_Circle.png", "PointSmp");
		Deco_CircleRenderer->GetTransform()->SetLocalPosition({ 0.0f,90.0f, -102.0f });
		Deco_CircleRenderer->GetTransform()->SetLocalScaling(Deco_CircleRenderer->GetCurrentTexture()->GetTextureSize());
	}

	{
		Deco_CharSelectBgRenderer = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
		Deco_CharSelectBgRenderer->SetImage("Lobby_CharSelectBG.png", "PointSmp");
		Deco_CharSelectBgRenderer->GetTransform()->SetLocalPosition({ -475.0f, 50.0f, -101.0f });
		Deco_CharSelectBgRenderer->GetTransform()->SetLocalScaling(Deco_CharSelectBgRenderer->GetCurrentTexture()->GetTextureSize());
	}


}

void LobbyBackground::Update(float _DeltaTime)
{
}
