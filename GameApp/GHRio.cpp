#include "PreCompile.h"
#include "GHRio.h"
#include "MousePointer.h"
#include "LumiaMap.h"
#include "GHNavMesh.h"

#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineLevelControlWindow.h>
#include <GameApp/LumiaLevel.h>


GHRio::GHRio()
	: renderer_(nullptr)
	, currentMap_(nullptr)
	, currentNavMesh_(nullptr)
{

}

GHRio::~GHRio()
{

}

void GHRio::Start()
{
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer_->SetFBXMesh("Rio_Run.fbx", "TextureDeferredLightAni");

	//for (size_t i = 0; i < renderer_->GetRenderSetCount(); i++)
	//{
	//	renderer_->GetRenderSet(static_cast<unsigned int>(i)).ShaderHelper->SettingTexture("DiffuseTex", "Rio_000_LOD1.png");
	//}

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	renderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	renderer_->CreateFBXAnimation("Run", "Rio_Run.fbx", 0);
	renderer_->CreateFBXAnimation("Wait", "Rio_Wait.fbx", 0);
	renderer_->ChangeFBXAnimation("Wait");

	GameEngineInput::GetInst().CreateKey("LButton", VK_LBUTTON);
	GameEngineInput::GetInst().CreateKey("RButton", VK_RBUTTON);
}

void GHRio::Update(float _deltaTime)
{
	if (!TempFlag)
	{
		return;
	}

	static float pathFindTime = 0.f;
	float4 worldPosition = GetTransform()->GetWorldPosition();
	LumiaLevel* level = dynamic_cast<LumiaLevel*>(GetLevel());
	MousePointer* mouse = nullptr;
	if (nullptr != level)
	{
		mouse = level->GetMousePointer();
	}

	if (GameEngineInput::GetInst().Press("RButton"))
	{
		if (nullptr != mouse)
		{
			destination_ = mouse->GetIntersectionYAxisPlane(0.0f, 1000.f);
			//float4 clickPosition = mouse->GetIntersectionYAxisPlane(0.0f, 1000.f);
			//LumiaMap* map = level->GetMap();
			//if (nullptr != map)
			//{
			//	float height = 0.f;
			//	
			//	GHNavMesh* clickedMesh = map->GetNavMesh(clickPosition);

			//	if (nullptr != clickedMesh)
			//	{
			//		map->IsMeshIntersected(*clickedMesh, clickPosition, height);
			//		destination_.x = clickPosition.x;
			//		destination_.y = height;
			//		destination_.z = clickPosition.z;
			//	}
			//	else
			//	{
			//		destination_ = clickPosition;
			//	}

			//}
		}
	}

	if (GameEngineInput::GetInst().Up("RButton"))
	{
		if ((destination_ - worldPosition).Len3D() > 300.f)
		{
			LumiaMap* map = level->GetMap();
			if (nullptr != map && nullptr != mouse)
			{
				GameEngineTime::GetInst().TimeCheck();
				destinations_ = map->FindPath(worldPosition, mouse->GetIntersectionYAxisPlane(0.0f, 1000.f));
				GameEngineTime::GetInst().TimeCheck();
				pathFindTime = GameEngineTime::GetInst().GetDeltaTime();
				
				destination_ = worldPosition;
			}
		}
	}

	GameEngineLevelControlWindow* controlWindow = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");
	if (nullptr != controlWindow)
	{
		controlWindow->AddText("Path find time elapsed : " + std::to_string(pathFindTime) + " sec");
	}

	worldPosition.y = destination_.y;
	if ((destination_ - worldPosition).Len3D() > 10.f)
	{
		renderer_->ChangeFBXAnimation("Run");


		direction_ = destination_ - worldPosition;
		direction_.Normalize3D();

		float4 cross = float4::Cross3D(direction_, { 0.0f, 0.0f, 1.0f });
		cross.Normalize3D();

		float angle = float4::DegreeDot3DToACosAngle(direction_, { 0.0f, 0.0f, 1.0f });

		GetTransform()->SetLocalRotationDegree({ 0.0f, angle * -cross.y, 0.0f });


		float4 moveSpeed = direction_ * SPEED * _deltaTime;
		float4 nextMovePosition = worldPosition + moveSpeed;

		float temp;
		if (true == currentMap_->IsMeshIntersected(*currentNavMesh_, nextMovePosition, temp))
		{
			GetTransform()->SetWorldPosition(nextMovePosition);
		}
		else
		{
			GHNavMesh* adjacentMesh = currentMap_->FindAdjacentMeshIntersect(*currentNavMesh_, nextMovePosition);

			if (nullptr == adjacentMesh)
			{
				destination_ = worldPosition;
			}
			else
			{
				currentNavMesh_ = adjacentMesh;
				GetTransform()->SetWorldPosition(nextMovePosition);
			}
		}

		//GetTransform()->SetWorldPosition(nextMovePosition);
	}
	else
	{
		if (!destinations_.empty())
		{
			destination_ = destinations_.back();
			destinations_.pop_back();
		}
		else
		{
			renderer_->ChangeFBXAnimation("Wait");
		}
	}

	float4 position = GetTransform()->GetWorldPosition();
	float height = 0;
	currentMap_->IsMeshIntersected(*currentNavMesh_, position, height);
	position.y = height;
	GetTransform()->SetWorldPosition(position);
}

void GHRio::SetSpawnPoint(const float4& _position)
{
	//float4 spawnPoint = { -6780.f, 0.0f, -780.f };
	//float4 spawnPoint = { -2500.f, 0.0f, 10000.f };
	//float4 spawnPoint = { 0.f, 0.0f, 0.f };
	GetTransform()->SetLocalPosition(_position);
	destination_ = _position;

	currentMap_ = GetLevelConvert<LumiaLevel>()->GetMap();

	if (nullptr == currentMap_)
	{
		GameEngineDebug::MsgBox("레벨에 맵이 배치되지 않았습니다.");
	}
	else
	{
		currentNavMesh_ = currentMap_->GetNavMesh(GetTransform()->GetWorldPosition());
	}

	if (nullptr == currentNavMesh_)
	{
		GameEngineDebug::MsgBox("초기 캐릭터 위치가 네비게이션 메쉬 위에 있지 않습니다.");
	}
}

void GHRio::startIdle(float _deltaTime)
{
}

void GHRio::updateIdle(float _deltaTime)
{
}

void GHRio::startRun(float _deltaTime)
{
}

void GHRio::updateRun(float _deltaTime)
{
}
