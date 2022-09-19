#include "PreCompile.h"
#include "NavActor.h"
#include "LumiaLevel.h"
#include "LumiaMap.h"
#include <GameEngine/GameEngineCollision.h>

NavActor::NavActor()
	: Collision_(nullptr)
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
	Collision_ = CreateTransformComponent<GameEngineCollision>();
	Collision_->GetTransform()->SetLocalScaling(100.0f);
	Collision_->SetCollisionGroup(CollisionGroup::Player);
}

void NavActor::Update(float _DeltaTime)
{
	if (nullptr != Collision_ &&
		Collision_->IsUpdate())
	{
		GetLevel()->PushDebugRender(Collision_->GetTransform(), CollisionType::AABBBox3D, float4::RED);
	}

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
	// ���� : Player�� SelectBox�� ���� �浹������

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

