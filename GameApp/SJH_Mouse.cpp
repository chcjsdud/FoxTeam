#include "PreCompile.h"
#include "SJH_Mouse.h"

#include <GameEngine/GameEngineUIRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "Enums.h"
#include "SJH_Ray.h"
#include "SJH_Yuki.h"

#include "SJH_FloorMap.h"
#include "SJH_NaviCell.h"

void SJH_Mouse::Start()
{
	Renderer_ = CreateTransformComponent<GameEngineUIRenderer>();
	Renderer_->SetImage(GameEngineString::toupper("Cursor_01.png"));
	Renderer_->GetTransform()->SetLocalPosition({ 12.0f, -10.0f, 0.0f });
	Renderer_->GetTransform()->SetLocalScaling({ 30.0f, 30.0f, 30.0f });

	Collider_ = CreateTransformComponent<GameEngineCollision>();
	Collider_->GetTransform()->SetLocalScaling(float4{ 30.0f, 30.0f, 30.0f });
	Collider_->SetCollisionInfo(static_cast<int>(CollisionGroup::MousePointer), CollisionType::AABBBox3D);
	
	// ���� ����
	Ray_ = GetLevel()->CreateActor<SJH_Ray>();
}

void SJH_Mouse::Update(float _DeltaTime)
{
	// ���콺 ��ġ����
	GetTransform()->SetWorldPosition(GameEngineInput::GetInst().GetMouse3DPos());

	// Űüũ
	if (true == GameEngineInput::GetInst().Down("LBUTTON"))
	{
		// ��, ȭ������� ����� ����
		if (true == GameEngineWindow::GetInst().IsWindowRangeOut(GameEngineInput::GetInst().GetMousePos()))
		{
			return;
		}

		float4 MovePos = float4::ZERO;

#pragma region BoundingCollider To Ray CrossCheck (220805 SJH �ּ�ó��)
		//// ī�޶�κ��� ���콺Ŭ���������� ���ϴ� ���⺤�͸� �����ϰ� �׿� �����ϴ� �浹ü�� Ž���Ͽ�
		//// �����ϴ� �浹ü �߽߰� �ش� ��ġ�� ��ȯ���ش�
		//if (true == Ray_->IsColliderPicked(GameEngineInput::GetInst().GetMousePos(), MovePos))
		//{
		//	SJH_Yuki::MainPlayer->GetTransform()->SetWorldPosition(MovePos);
		//}
#pragma endregion

#pragma region Mesh To Ray CrossCheck
		if (nullptr != SJH_FloorMap::FloorMap)
		{
			// 220810 SJH �ӽ��ּ�ó��
			//if (true == Ray_->IsPicked(GameEngineInput::GetInst().GetMousePos(), MovePos, SJH_FloorMap::FloorMap->GetFloorMapRenderer()))
			//{
			//	SJH_Yuki::MainPlayer->GetTransform()->SetWorldPosition(MovePos);
			//}

			SJH_NaviCell* PickedNaviCell = Ray_->IsPickedCell(SJH_FloorMap::FloorMap->GetFloorMapRenderer(), GameEngineInput::GetInst().GetMousePos(), MovePos);
			if (nullptr != PickedNaviCell)
			{
				SJH_Yuki::MainPlayer->MoveStart(PickedNaviCell, MovePos);
			}
		}
#pragma endregion
	}

	// 
	if (true == GameEngineInput::GetInst().Down("RBUTTON"))
	{

	}
}

SJH_Mouse::SJH_Mouse()
	: Renderer_(nullptr)
	, Collider_(nullptr)
	, Ray_(nullptr)
{
}

SJH_Mouse::~SJH_Mouse()
{
}
