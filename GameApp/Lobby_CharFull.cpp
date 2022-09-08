#include "Precompile.h"
#include "Lobby_CharFull.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"
#include "LobbyUIController.h"

Lobby_CharFull::Lobby_CharFull()
{
}

Lobby_CharFull::~Lobby_CharFull()
{
}

void Lobby_CharFull::SetChar(JobType _Character)
{

}

void Lobby_CharFull::Start()
{
	{
		CharFull_Renderer = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
		CharFull_Renderer->SetImage("Lobby_CharFull_Hyunwoo.png", "PointSmp");
		CharFull_Renderer->GetTransform()->SetLocalPosition({ 0.0f, 125.0f, -100.0f });
		CharFull_Renderer->GetTransform()->SetLocalScaling(CharFull_Renderer->GetCurrentTexture()->GetTextureSize());
	}
}

void Lobby_CharFull::Update(float _DeltaTime)
{
	switch (LobbyUIController::SelectedChar)
	{
	case JobType::NONE:
		CharFull_Renderer->Off();
		break;
	case JobType::YUKI:
		CharFull_Renderer->Off();
		break;
	case JobType::FIORA:
		CharFull_Renderer->Off();
		break;
	case JobType::ZAHIR:
		CharFull_Renderer->Off();
		break;
	case JobType::NADINE:
		CharFull_Renderer->Off();
		break;
	case JobType::HYUNWOO:
		CharFull_Renderer->On();
		CharFull_Renderer->SetImage("Lobby_CharFull_Hyunwoo.png", "PointSmp");
		CharFull_Renderer->GetTransform()->SetLocalScaling(CharFull_Renderer->GetCurrentTexture()->GetTextureSize());
		break;
	case JobType::JACKIE:
		CharFull_Renderer->On();
		CharFull_Renderer->SetImage("Lobby_CharFull_Jackie.png", "PointSmp");
		CharFull_Renderer->GetTransform()->SetLocalScaling(CharFull_Renderer->GetCurrentTexture()->GetTextureSize());
		break;
	case JobType::RIO:
		CharFull_Renderer->Off();
		break;
	case JobType::AYA:
		CharFull_Renderer->Off();
		break;
	case JobType::MAX:
		CharFull_Renderer->Off();
		break;
	default:
		break;
	}

	return;
}
