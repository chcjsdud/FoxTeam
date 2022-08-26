#include "PreCompile.h"
#include "UI_ItemBox.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"
#include "GameEngine/GameEngineCollision.h"
#include "GameEngineBase/GameEngineDebug.h"

#include "ItemBase.h"
#include "LH_Player.h"


UI_ItemBox::UI_ItemBox()
	: Time(1.0f), UI_On(false),
	Slot0_Item(nullptr), Slot1_Item(nullptr), Slot2_Item(nullptr), Slot3_Item(nullptr),
	Slot4_Item(nullptr), Slot5_Item(nullptr), Slot6_Item(nullptr), Slot7_Item(nullptr)
{
}

UI_ItemBox::~UI_ItemBox()
{
	//for (auto& ItemSlot : SlotList_)
	//{
	//	if (true)
	//	{
	//		delete ItemSlot;
	//		ItemSlot = nullptr;
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
		//���콺���� �浹�� ���ؼ� aabbbox3d�� �浹
		ItemBoxCollision = CreateTransformComponent<GameEngineCollision>();
		ItemBoxCollision->GetTransform()->SetLocalPosition(ItemBox_BackgroundPos);
		ItemBoxCollision->GetTransform()->SetLocalScaling(ItemBoxBackGround_Renderer->GetCurrentTexture()->GetTextureSize());
		ItemBoxCollision->SetCollisionInfo(static_cast<int>(CollisionGroup::UI), CollisionType::AABBBox3D);
	}

	//for (int i = 0; i < 8; i++)
	//{
	//	ItemBase* Item = new ItemBase();
	//	Item->SetName("Slot_" + to_string(i));
	//	SlotList_.push_back(Item);
	//}

	//SlotList_.push_back(Slot0_Item);
	//SlotList_.push_back(Slot1_Item);
	//SlotList_.push_back(Slot2_Item);
	//SlotList_.push_back(Slot3_Item);
	//SlotList_.push_back(Slot4_Item);
	//SlotList_.push_back(Slot5_Item);
	//SlotList_.push_back(Slot6_Item);
	//SlotList_.push_back(Slot7_Item);
	


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
		if (UI_On == true)
		{
			RenderOff();
		}
		else
		{
			RenderOn();
		}
	}

	CheckItemSlot();

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

void UI_ItemBox::PushItem(ItemBase* _OriginItemBase, int _SlotNumber)
{
	switch (_SlotNumber)
	{
	case 0:
	{
		EmptySlotReturn(_OriginItemBase);
		return;
	}
		break;
	case 1:
	{
		if (Slot1_Item == nullptr)
		{
			Slot1_Item = _OriginItemBase->Copy();
			return;
		}
		else
		{
			GameEngineDebug::MsgBoxError("�̹� ���õ� ���Կ� �������� �����Ϸ� �߽��ϴ�");
			return;
		}
	}
		break;
	case 2:
	{
		if (Slot2_Item == nullptr)
		{
			Slot2_Item = _OriginItemBase->Copy();
			return;
		}
		else
		{
			GameEngineDebug::MsgBoxError("�̹� ���õ� ���Կ� �������� �����Ϸ� �߽��ϴ�");
			return;
		}
	}
		break;
	case 3:
	{
		if (Slot3_Item == nullptr)
		{
			Slot3_Item = _OriginItemBase->Copy();
			return;
		}
		else
		{
			GameEngineDebug::MsgBoxError("�̹� ���õ� ���Կ� �������� �����Ϸ� �߽��ϴ�");
			return;
		}
	}
		break;
	case 4:
	{
		if (Slot4_Item == nullptr)
		{
			Slot4_Item = _OriginItemBase->Copy();
			return;
		}
		else
		{
			GameEngineDebug::MsgBoxError("�̹� ���õ� ���Կ� �������� �����Ϸ� �߽��ϴ�");
			return;
		}
	}
		break;
	case 5:
	{
		if (Slot5_Item == nullptr)
		{
			Slot5_Item = _OriginItemBase->Copy();
			return;
		}
		else
		{
			GameEngineDebug::MsgBoxError("�̹� ���õ� ���Կ� �������� �����Ϸ� �߽��ϴ�");
			return;
		}
	}
		break;
	case 6:
	{
		if (Slot6_Item == nullptr)
		{
			Slot6_Item = _OriginItemBase->Copy();
			return;
		}
		else
		{
			GameEngineDebug::MsgBoxError("�̹� ���õ� ���Կ� �������� �����Ϸ� �߽��ϴ�");
			return;
		}
	}
		break;
	case 7:
	{
		if (Slot7_Item == nullptr)
		{
			Slot7_Item = _OriginItemBase->Copy();
			return;
		}
		else
		{
			GameEngineDebug::MsgBoxError("�̹� ���õ� ���Կ� �������� �����Ϸ� �߽��ϴ�");
			return;
		}
	}
		break;
	default:
	{
		GameEngineDebug::MsgBoxError("���Թ�ȣ�� ����� ���õ��� �ʾҽ��ϴ�");
		return;
	}
		break;
	}
}

void UI_ItemBox::EmptySlotReturn(ItemBase* _TargetSlot)
{

	if (Slot0_Item == nullptr)
	{
		Slot0_Item = _TargetSlot->Copy();
	//	Slot0_Item->SetImage();
		return;
	}

	if (Slot1_Item == nullptr)
	{
		Slot1_Item = _TargetSlot->Copy();
		return;
	}

	if (Slot2_Item == nullptr)
	{
		Slot2_Item = _TargetSlot->Copy();
		return;
	}

	if (Slot3_Item == nullptr)
	{
		Slot3_Item = _TargetSlot->Copy();
		return;
	}

	if (Slot4_Item == nullptr)
	{
		Slot4_Item = _TargetSlot->Copy();
		return;
	}

	if (Slot5_Item == nullptr)
	{
		Slot5_Item = _TargetSlot->Copy();
		return;
	}

	if (Slot6_Item == nullptr)
	{
		Slot6_Item = _TargetSlot->Copy();
		return;
	}

	if (Slot7_Item == nullptr)
	{
		Slot7_Item = _TargetSlot->Copy();
		return;
	}

	GameEngineDebug::MsgBox("������ �ڽ��� �� ������ �����ϴ�");
	return;

}

void UI_ItemBox::CheckItemSlot()
{
	if (Slot0_Item != nullptr)
	{
		Slot0_Item->GetTransform()->AttachTransform(ItemBoxBackGround_Renderer->GetTransform());
	}
	else
	{
		//���Կ��� ���� Collision ���� ����
	}

	if (Slot1_Item != nullptr)
	{
		Slot1_Item->GetTransform()->AttachTransform(ItemBoxBackGround_Renderer->GetTransform());
	}

	if (Slot2_Item != nullptr)
	{
		Slot2_Item->GetTransform()->AttachTransform(ItemBoxBackGround_Renderer->GetTransform());
	}

	if (Slot3_Item != nullptr)
	{
		Slot3_Item->GetTransform()->AttachTransform(ItemBoxBackGround_Renderer->GetTransform());
	}

	if (Slot4_Item != nullptr)
	{
		Slot4_Item->GetTransform()->AttachTransform(ItemBoxBackGround_Renderer->GetTransform());
	}

	if (Slot5_Item != nullptr)
	{
		Slot5_Item->GetTransform()->AttachTransform(ItemBoxBackGround_Renderer->GetTransform());
	}

	if (Slot6_Item != nullptr)
	{
		Slot6_Item->GetTransform()->AttachTransform(ItemBoxBackGround_Renderer->GetTransform());
	}

	if (Slot7_Item != nullptr)
	{
		Slot7_Item->GetTransform()->AttachTransform(ItemBoxBackGround_Renderer->GetTransform());
	}

}
