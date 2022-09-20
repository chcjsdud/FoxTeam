#include "PreCompile.h"

#include <GameEngine/GameEngineCollision.h>

#include "Character.h"
#include "LumiaLevel.h"
#include "MousePointer.h"
#include "LumiaMap.h"
#include "ItemBox.h"

Character::Character()
	: collision_(nullptr)
	, currentNavFace_(nullptr)
	, currentNavMesh_(nullptr)
	, manager_(nullptr)
	, currentMap_(nullptr)
	, mouse_(nullptr)
{
}

Character::~Character()
{

}

void Character::SetNaviMesh(NavMesh* _NaviMesh)
{
	currentNavMesh_ = _NaviMesh;
	currentNavFace_ = currentNavMesh_->CurrentCheck(GetTransform(), float4::DOWN);
}

void Character::Start()
{
	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->GetTransform()->SetLocalScaling(150.0f);
	collision_->SetCollisionGroup(CollisionGroup::Player);
	collision_->SetCollisionType(CollisionType::AABBBox3D);

	manager_ = GetLevelConvert<LumiaLevel>()->GetItemBoxManager();


	LumiaLevel* level = dynamic_cast<LumiaLevel*>(level_);
	if (nullptr == level)
	{
		GameEngineDebug::MsgBoxError("level ��ȯ�� �����߽��ϴ�. class GameEngineLevel to " + std::string(typeid(LumiaLevel).name()));
	}

	currentMap_ = level->GetMap();
	mouse_ = level->GetMousePointer();

	if (nullptr == currentMap_)
	{
		GameEngineDebug::MsgBoxError("level�� Map ������ �����ϴ�.");
	}

	if (nullptr == mouse_)
	{
		GameEngineDebug::MsgBoxError("level�� MousePointer ������ �����ϴ�.");
	}
}

void Character::Update(float _DeltaTime)
{
	if (nullptr != collision_ &&
		collision_->IsUpdate())
	{
		GetLevel()->PushDebugRender(collision_->GetTransform(), CollisionType::AABBBox3D, float4::RED);
	}

	CheckCurrentNav();

	CheckItemBox();
}

void Character::CheckCurrentNav()
{
	if (nullptr != currentNavFace_)
	{
		if (true == currentNavFace_->OutCheck(GetTransform()))
		{
			currentNavFace_ = currentNavFace_->MoveCheck(GetTransform());
		}
	}
	else
	{
		if (nullptr != GetLevelConvert<LumiaLevel>()->GetMap())
		{
			SetCurrentNavi(GetLevelConvert<LumiaLevel>()->GetMap()->GetNavMesh()->CurrentCheck(GetTransform(), float4::DOWN));
		}
	}
}

void Character::GetItem()
{
	// �ڽ��� �����ִ� ���¿���
	// �����ִ� UI�� �������� Ŭ���ϴ� ���
	// �� �������� ������´�.
}

void Character::CheckItemBox()
{
	// Player�� SelectBox�� ���� �浹���������� üũ

	GameEngineCollision* OtherCol = collision_->CollisionPtr(static_cast<int>(CollisionGroup::ItemBox));

	if (nullptr != OtherCol)
	{
		ItemBox* Box = GetLevelConvert<LumiaLevel>()->GetItemBoxManager()->GetSelectBox();

		if (nullptr == Box)
		{
			return;
		}

		if (OtherCol != Box->GetCollision())
		{
			return;
		}

		// ���� �ڽ��� ���� �Ŀ� Ŀ���� SelectBox �ٱ����� �ű�� �ڽ��� ������ ����
		// Player�� ������ �����ϴ� ��� �ڽ��� ����.
		// ItemBox UI �� ����
		manager_->OpenItemBox();
		return;
	}

	manager_->CloseItemBox();


	// CloseItemBox
	// Player�� �����۹ڽ��κ��� �־��� ���
	// UI�� ������.
	// SelectBox�� nullptr�� �ʱ�ȭ
	// �ʱ�ȭ���� ������ SelectBox ��ó�� �ٰ����� UI�� ��� ������ ��
}

