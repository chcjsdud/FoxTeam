#include "PreCompile.h"
#include "TopUI.h"
#include "GameEngine/GameEngineUIRenderer.h"

TopUI::TopUI()
	: Time(3.0f)
{
}

TopUI::~TopUI()
{
	//for (auto& UIRenderer : UIRendererMap)
	//{
	//	if (true)
	//	{
	//		delete UIRenderer.second;
	//		UIRenderer.second = nullptr;
	//	}
	//}
}

void TopUI::Start()
{
	
	TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	TopRenderer->SetImage("AlphaTest.png", "PointSmp");
	TopRenderer->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	TopRenderer->GetTransform()->SetLocalScaling(TopRenderer->GetCurrentTexture()->GetTextureSize());
	//TopRenderer->SetAlpha(0.0f);
	//TopRenderer->TextSetting("�ü�", "�׽�Ʈ ���Դϴ�", 50);

	//���İ��� ����� ��������, �̹��� ������ �ƴϴ�
	float4 color = TopRenderer->GetCurrentTexture()->GetPixel(30,150);
	float4 color2 = TopRenderer->GetCurrentTexture()->GetPixel(50, 100);
	
	UIRendererMap.insert(make_pair("AlphaTest", TopRenderer));

}

void TopUI::Update(float _Time)
{
	//if (0.0f >= (Time -= _Time))
	//{
	//	TopRenderer->Death();
	//}
}
