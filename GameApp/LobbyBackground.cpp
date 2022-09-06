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
		ButtonTwoRenderer = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
		ButtonTwoRenderer->SetImage("Lobby_BasicButton.png", "PointSmp");
		//이미지 크기는 126, 두 이미지의 거리는 8
		ButtonTwoRenderer->GetTransform()->SetLocalPosition({ 554.0f,330.0f, -102.0f });
		ButtonTwoRenderer->GetTransform()->SetLocalScaling(ButtonTwoRenderer->GetCurrentTexture()->GetTextureSize());
	}

}

void LobbyBackground::Update(float _DeltaTime)
{
}
