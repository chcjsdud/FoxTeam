#include "PreCompile.h"
#include "UI_ItemBox.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"

#include "LH_Player.h"


UI_ItemBox::UI_ItemBox()
	: Time(1.0f), UI_On(false)
{
}

UI_ItemBox::~UI_ItemBox()
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

void UI_ItemBox::Start()
{
	//���ҽ����� SetImage�ϰ� ��ġ�� �����մϴ�
	//��ġ�� ������ ���ҽ����� UIRenderMap�� String�� ���� insert�˴ϴ�.
	//�ΰ�������(HP�� ���¹̳� ����, ������񿩺�)���� ������, UIRendererMap���� �̹����̸����� Find�ؼ� ���ҽ��� �ٲ� �������ϴ�.

	float4 ItemBox_BackgroundPos = { 0.0f, -100.0f, 0.0f };


		//z���� �̿��� �տ����� �̹���/�ڿ����� �̹��� ������ ���ϰ� �ֽ��ϴ�.
		//��ġ������ �� float�� ���� �þ�� map�̳� vector�� ������ �����Դϴ�.
	{
		ItemBoxBackGround_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		ItemBoxBackGround_Renderer->SetImage("ItemBox_BackGround.png", "PointSmp");
		ItemBoxBackGround_Renderer->GetTransform()->SetLocalPosition(ItemBox_BackgroundPos);
		ItemBoxBackGround_Renderer->GetTransform()->SetLocalScaling(ItemBoxBackGround_Renderer->GetCurrentTexture()->GetTextureSize());
	}

	{
		BoxtypeFont_Renderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	//	BoxtypeFont_Renderer->TextSetting("����", "����", 30, float4::WHITE);
	//	BoxtypeFont_Renderer->GetTransform()->SetLocalPosition(ItemBox_BackgroundPos);
	}

	//����, Player ���� �Լ�
	//int HP = Player_->PlayerGetHP();
	//float Stmina = Player_->PlayerGetStamina();
	//Player_->PlayerSetHP(10);
	//Player_->PlayerSetStamina(100.f);

	{
		ItemBoxCollision = CreateTransformComponent<GameEngineCollision>();
		ItemBoxCollision->GetTransform()->SetLocalPosition(ItemBox_BackgroundPos);
		ItemBoxCollision->GetTransform()->SetLocalScaling(ItemBoxBackGround_Renderer->GetCurrentTexture()->GetTextureSize());
		ItemBoxCollision->SetCollisionInfo(static_cast<int>(CollisionGroup::UI), CollisionType::AABBBox3D);
	}


}

void UI_ItemBox::Update(float _Time)
{
	if (0.0f >= (Time -= _Time))
	{
		//TopRenderer->SetAlpha(Time);
	}

	//UI �¿��� üũ
	{
		if (false == UI_On)
		{
			ItemBoxBackGround_Renderer->Off();
			BoxtypeFont_Renderer->Off();
		}
		else
		{
			ItemBoxBackGround_Renderer->On();
			BoxtypeFont_Renderer->On();
			GetLevel()->PushDebugRenderUI(ItemBoxCollision->GetTransform(), CollisionType::AABBBox3D);
		}
	}

	if (true == GameEngineInput::GetInst().Down("Esc"))
	{
		//if (UI_On == true)
		//{
		//	RenderOff();
		//}
		//else
		//{
		//	RenderOn();
		//}
	}

}

void UI_ItemBox::RenderOff()
{
	UI_On = false;
}

void UI_ItemBox::RenderOn()
{
	UI_On = true;
}

bool UI_ItemBox::MouseCollisionCheck()
{
	if (true == ItemBoxCollision->Collision((int)CollisionGroup::MousePointer))
	{
		return true;
	}
	else
	{
		return false;
	}
}