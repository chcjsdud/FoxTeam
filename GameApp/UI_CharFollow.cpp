#include "Precompile.h"
#include "UI_CharFollow.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"
#include "GameEngine/GameEngineUIRenderer.h"

UI_CharFollow::UI_CharFollow()
{
}

UI_CharFollow::~UI_CharFollow()
{
}

void UI_CharFollow::Start()
{
	HpBarPos = { -19.0f, -335.0f, -1.0f };
	HpBarSize = { 70.0f, 9.0f };

	SpBarPos = { -19.0f, -347.0f, -1.0f };
	SpBarSize = { 70.f, 3.f };

	EmptyBarSize = {70.f, 13.f};

	{
		HPBar_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		HPBar_Renderer->SetImage("HPBar_UI.png", "PointSmp");
		//HPBar_Renderer->GetTransform()->SetLocalPosition(HpBarPos);
		HPBar_Renderer->GetTransform()->SetLocalScaling(HpBarSize);

		SPBar_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::UIICON);
		SPBar_Renderer->SetImage("SPBar_UI.png", "PointSmp");
		SPBar_Renderer->GetTransform()->SetLocalScaling(SpBarSize);

		EmptyBar_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform(), (int)UIRenderOrder::BACKDROP);
		EmptyBar_Renderer->SetImage("EmptyBar_UI.png", "PointSmp");
		EmptyBar_Renderer->GetTransform()->SetLocalScaling(EmptyBarSize);
	}


}

void UI_CharFollow::Update(float _DeltaTime)
{
}

void UI_CharFollow::SetFollowInfo(float4 _Pos, CharacterStat* _Stat) 
{

}
