#include "PreCompile.h"
#include "TopUI.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"

TopUI::TopUI()
	: Time(1.0f)
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
	//리소스들을 SetImage하고 위치를 조정합니다
	//위치를 조정한 리소스들은 UIRenderMap에 String과 같이 insert됩니다.
	//인게임정보(HP나 스태미너 상태, 착용장비여부)등을 받으면, UIRendererMap에서 이미지이름으로 Find해서 리소스를 바꿀 예정업니다.
	{
		//z값을 이용해 앞에오는 이미지/뒤에오는 이미지 순서를 정하고 있습니다.
		//위치정보가 될 float도 양이 늘어나면 map이나 vector로 관리할 예정입니다.
		float4 BackGroundBarPos = { -400.0f, -200.0f, 0.0f };
		float4 HPBarPos = { -395.0f, -202.0f, -1.0f };
		float4 HPBarTopPos = { -394.0f, -143.0f, -1.0f };
		float4 HPBarLinePos = { -401.0f, -180.0f, -2.0f};

		{
			TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
			TopRenderer->SetImage("BackGroundBar_Test.png", "PointSmp");
			TopRenderer->GetTransform()->SetLocalPosition(BackGroundBarPos);
			TopRenderer->GetTransform()->SetLocalScaling(TopRenderer->GetCurrentTexture()->GetTextureSize());
			UIRendererMap.insert(make_pair("BackGroundBar_Test", TopRenderer));
		}

		{
			TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
			TopRenderer->SetImage("HPBar_example.png", "PointSmp");
			TopRenderer->GetTransform()->SetLocalPosition(HPBarPos);
			TopRenderer->GetTransform()->SetLocalScaling(TopRenderer->GetCurrentTexture()->GetTextureSize());
			UIRendererMap.insert(make_pair("HPBar_example", TopRenderer));
		}

		{
			TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
			TopRenderer->SetImage("HPBar_Top.png", "PointSmp");
			TopRenderer->GetTransform()->SetLocalPosition(HPBarTopPos);
			TopRenderer->GetTransform()->SetLocalScaling(TopRenderer->GetCurrentTexture()->GetTextureSize());
			UIRendererMap.insert(make_pair("HPBar_Top", TopRenderer));
		}

		{
			TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
			TopRenderer->SetImage("Bar_Line.png", "PointSmp");
			TopRenderer->GetTransform()->SetLocalPosition(HPBarLinePos);
			TopRenderer->GetTransform()->SetLocalScaling(TopRenderer->GetCurrentTexture()->GetTextureSize());
			UIRendererMap.insert(make_pair("Bar_Line", TopRenderer));
		}
	}

}

void TopUI::Update(float _Time)
{
	if (0.0f >= (Time -= _Time))
	{
		//TopRenderer->SetAlpha(Time);
	}


//	TopRenderer->SetAlpha(0.1f);
}

