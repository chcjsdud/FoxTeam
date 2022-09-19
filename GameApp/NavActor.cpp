#include "PreCompile.h"
#include "NavActor.h"
#include "LumiaLevel.h"
#include "LumiaMap.h"
#include <GameEngine/GameEngineCollision.h>
#include "ItemBox.h"

NavActor::NavActor()
	: collision_(nullptr)
	, currentNavFace_(nullptr)
	, currentNavMesh_(nullptr)
{
}

NavActor::~NavActor()
{

}

void NavActor::SetNaviMesh(NavMesh* _NaviMesh)
{
	currentNavMesh_ = _NaviMesh;
	currentNavFace_ = currentNavMesh_->CurrentCheck(GetTransform(), float4::DOWN);
}

void NavActor::Start()
{
	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->GetTransform()->SetLocalScaling(100.0f);
	collision_->SetCollisionGroup(CollisionGroup::Player);
	collision_->SetCollisionType(CollisionType::AABBBox3D);
}

void NavActor::Update(float _DeltaTime)
{
	if (nullptr != collision_ &&
		collision_->IsUpdate())
	{
		GetLevel()->PushDebugRender(collision_->GetTransform(), CollisionType::AABBBox3D, float4::RED);
	}

	CheckCurrentNav();

	CheckItemBox();
}

void NavActor::CheckCurrentNav()
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

void NavActor::OpenItemBox()
{
	// Player�� ������ �����ϴ� ��� �ڽ��� ����.
	// ItemBox UI �� ����
}

void NavActor::GetItem()
{
	// �ڽ��� �����ִ� ���¿���
	// �����ִ� UI�� �������� Ŭ���ϴ� ���
	// �� �������� ������´�.
}

void NavActor::CloseItemBox()
{
	// Player�� �ٸ� ���� Ŭ���ϴ� ���
	// UI�� ������.
	// SelectBox�� nullptr�� �ʱ�ȭ
	// �ʱ�ȭ���� ������ SelectBox ��ó�� �ٰ����� UI�� ��� ������ ��
}

void NavActor::CheckItemBox()
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
		OpenItemBox();
	}
}

