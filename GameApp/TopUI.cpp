#include "PreCompile.h"
#include "TopUI.h"
#include "GameEngine/GameEngineUIRenderer.h"

TopUI::TopUI()
	: Time(3.0f)
{
}

TopUI::~TopUI()
{
}

void TopUI::Start()
{
	TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	TopRenderer->GetTransform()->SetLocalScaling({ 1280.0f, 100.0f, 1.0f });
	TopRenderer->GetTransform()->SetLocalPosition({ 0.0f, 300.0f, 0.0f });
	TopRenderer->SetImage("Char.png", "LinearSmp");
	TopRenderer->TextSetting("궁서", "테스트 중입니다", 50);

	TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	TopRenderer->GetTransform()->SetLocalScaling({ 1280.0f, 100.0f, 1.0f });
	TopRenderer->GetTransform()->SetLocalPosition({ 0.0f, -300.0f, 0.0f });
	TopRenderer->TextSetting("궁서", "진짜 안나올까요?", 50);

}

void TopUI::Update(float _Time)
{
	if (0.0f >= (Time -= _Time))
	{
		TopRenderer->Death();
	}
}
