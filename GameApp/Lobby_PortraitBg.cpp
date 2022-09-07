#include "Precompile.h"
#include "Lobby_PortraitBg.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"

Lobby_PortraitBg::Lobby_PortraitBg()
{
}

Lobby_PortraitBg::~Lobby_PortraitBg()
{
}

void Lobby_PortraitBg::Start()
{

	float4 Position = { -580.0f, 200.0f, -102.0f };

	{
		BasicImageRenderer = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
		BasicImageRenderer->SetImage("Lobby_PortraitBG_Basic.png", "PointSmp");
		BasicImageRenderer->GetTransform()->SetLocalPosition(Position);
		BasicImageRenderer->GetTransform()->SetLocalScaling(BasicImageRenderer->GetCurrentTexture()->GetTextureSize());
	}

	{
		SelectImageRenderer = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
		SelectImageRenderer->SetImage("Lobby_PortraitBG_Select.png", "PointSmp");
		SelectImageRenderer->GetTransform()->SetLocalPosition((Position + float4{0.0f, 0.0f, -1.0f}));
		SelectImageRenderer->GetTransform()->SetLocalScaling(SelectImageRenderer->GetCurrentTexture()->GetTextureSize());
		SelectImageRenderer->Off();
	}

	{
		MouseCollision = CreateTransformComponent<GameEngineCollision>();
		MouseCollision->GetTransform()->SetLocalPosition(Position);
		MouseCollision->GetTransform()->SetLocalScaling(BasicImageRenderer->GetCurrentTexture()->GetTextureSize());
		MouseCollision->SetCollisionInfo(static_cast<int>(CollisionGroup::UI), CollisionType::Rect);
	}

}

void Lobby_PortraitBg::Update(float _DeltaTime)
{
	//GetLevel()->PushDebugRender(MouseCollision->GetTransform(), CollisionType::Rect);


	if (MouseCollision->Collision(CollisionGroup::MousePointer))
	{
		if (GameEngineInput::GetInst().Down("LBUTTON"))
		{
			SelectImageRenderer->On();
		}
	}
	else
	{
		if (GameEngineInput::GetInst().Down("LBUTTON"))
		{
			SelectImageRenderer->Off();
		}
	}
}
