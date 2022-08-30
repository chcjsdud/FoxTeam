#include "PreCompile.h"
#include "GHRio.h"
#include "GHRayTestLevel.h"
#include "MousePointer.h"
#include "GHMap.h"
#include "GHNavMesh.h"

#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineLevelControlWindow.h>


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
	//float4 spawnPoint = { -6780.f, 0.0f, -780.f };
	float4 spawnPoint = { 0.f, 0.0f, 0.f };
	GetTransform()->SetLocalPosition(spawnPoint);
	destination_ = spawnPoint;

	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();

	renderer_->SetFBXMesh("Rio_Run.fbx", "TextureDeferredLightAni");
	//renderer_->SetFBXMesh("Rio_Wait.UserMesh", "TextureDeferredLightAni");

	for (size_t i = 0; i < renderer_->GetRenderSetCount(); i++)
	{
		renderer_->GetRenderSet(static_cast<unsigned int>(i)).ShaderHelper->SettingTexture("DiffuseTex", "Rio_000_LOD1.png");
	}

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	renderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	renderer_->CreateFBXAnimation("Run", "Rio_Run.fbx", 0);
	renderer_->CreateFBXAnimation("Wait", "Rio_Wait.fbx", 0);

	//renderer_->CreateFBXAnimation("Run", "Rio_Run.UserAnimation", 0);
	//renderer_->CreateFBXAnimation("Wait", "Rio_Wait.UserAnimation", 0);

	renderer_->ChangeFBXAnimation("Wait");

	GameEngineInput::GetInst().CreateKey("LButton", VK_LBUTTON);
	GameEngineInput::GetInst().CreateKey("RButton", VK_RBUTTON);

	currentMap_ = GetLevelConvert<GHRayTestLevel>()->GetMap();

	if (nullptr == currentMap_)
	{
		GameEngineDebug::MsgBoxError("레벨에 맵이 배치되지 않았습니다.");
	}

	currentNavMesh_ = currentMap_->GetCurrentNavMesh(GetTransform()->GetWorldPosition());

	if (nullptr == currentNavMesh_)
	{
		GameEngineDebug::MsgBoxError("초기 캐릭터 위치가 네비게이션 메쉬 위에 있지 않습니다.");
	}
}

void GHRio::Update(float _deltaTime)
{
	static float pathFindTime = 0.f;
	float4 worldPosition = GetTransform()->GetWorldPosition();
	GHRayTestLevel* level = dynamic_cast<GHRayTestLevel*>(GetLevel());
	MousePointer* mouse = nullptr;
	if (nullptr != level)
	{
		mouse = level->GetMousePointer();
	}

	if (GameEngineInput::GetInst().Press("LButton"))
	{
		if (nullptr != mouse)
		{
			destination_ = mouse->GetIntersectionYAxisPlane(0.0f, 1000.f);
		}
	}

	if (GameEngineInput::GetInst().Up("LButton"))
	{
		destination_ = worldPosition;
		GHMap* map = level->GetMap();
		if (nullptr != map && nullptr != mouse)
		{
			GameEngineTime::GetInst().TimeCheck();
			destinations_ = map->FindPath(worldPosition, mouse->GetIntersectionYAxisPlane(0.0f, 1000.f));
			GameEngineTime::GetInst().TimeCheck();
			pathFindTime = GameEngineTime::GetInst().GetDeltaTime();
		}
	}

	GameEngineLevelControlWindow* controlWindow = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");
	if (nullptr != controlWindow)
	{
		controlWindow->AddText("Path find time elapsed : " + std::to_string(pathFindTime) + " sec");
	}


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

		if (true == currentMap_->IsIntersectionMesh(*currentNavMesh_, nextMovePosition))
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
