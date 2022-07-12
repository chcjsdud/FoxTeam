#include "PreCompile.h"
#include <GameEngine\GameEngineImageRenderer.h>
#include <GameEngine\GameEngineCollision.h>
#include "Monster.h"
#include "Bullet.h"

Monster::Monster()
{
}

Monster::~Monster()
{
}

void Monster::TestStartCallBack()
{
	int a = 0;
}

void Monster::TestEndCallBack() 
{
	Death();
}

void Monster::Start()
{
	// 정말 세팅해줘야할게 많은 녀석입니다.
	// 랜더러로서 뭐든지 다 그릴수있는 가능성을 가지고 있는 녀석.
	//{
	//	GameEngineRenderer* Renderer = CreateTransformComponent<GameEngineRenderer>(GetTransform());
	//	Renderer->SetRenderingPipeLine("Color");
	//	Renderer->GetTransform()->SetLocalScaling({ 100.0f, 100.0f, 1.0f });
	//	Renderer->ShaderHelper.SettingConstantBufferSet("ResultColor", float4(0.0f, 0.0f, 0.0f));
	//}

	{
		GameEngineImageRenderer* PlayerImageRenderer = CreateTransformComponent<GameEngineImageRenderer>(GetTransform());
		PlayerImageRenderer->CreateAnimation("Animation.png", "Dash", 20, 10, 0.5f, false);
		PlayerImageRenderer->SetChangeAnimation("Dash");

		PlayerImageRenderer->GetTransform()->SetLocalScaling({ 100.0f, 100.0f, 1.0f });
	}

	{
		GameEngineCollision* Collision = CreateTransformComponent<GameEngineCollision>(20);
		Collision->GetTransform()->SetLocalScaling(float4{ 100.0f, 100.0f, 1.0f });
	}
}

void Monster::Update(float _DeltaTime)
{

}