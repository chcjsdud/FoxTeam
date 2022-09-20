#include "PreCompile.h"
#include "GHRio.h"
#include "MousePointer.h"
#include "LumiaMap.h"

#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineLevelControlWindow.h>
#include <GameApp/LumiaLevel.h>


Rio::Rio()
	: Character()
	, renderer_(nullptr)
	, currentMap_(nullptr)
{

}

Rio::~Rio()
{

}

void Rio::Start()
{
	Character::Start();

	initRendererAnimation();
	initInput();
	initState();
}

void Rio::Update(float _deltaTime)
{
	//mainState_.Update(_deltaTime);

	if (!TempFlag)
	{
		static float pathFindTime = 0.f;

		float4 worldPosition = GetTransform()->GetWorldPosition();
		LumiaLevel* level = dynamic_cast<LumiaLevel*>(GetLevel());
		MousePointer* mouse = nullptr;
		LumiaMap* map = nullptr;

		if (nullptr != level)
		{
			mouse = level->GetMousePointer();
			map = level->GetMap();
		}

		float height = 0.f;
		NavFace* clickedFace = nullptr;
		if (GameEngineInput::GetInst().Press("LButton"))
		{
			if (nullptr != map)
			{
				bool result = map->GetNavMesh()->GetIntersectionPointFromMouseRay(destination_);

				if (!result)
				{
					if (mouse != nullptr)
					{
						destination_ = mouse->GetIntersectionYAxisPlane(worldPosition.y, 2000.f);
					}
				}
			}
		}

		if (GameEngineInput::GetInst().Up("LButton"))
		{
			bool result = false;
			LumiaMap* map = level->GetMap();
			if (nullptr != map)
			{
				result = map->GetNavMesh()->GetIntersectionPointFromMouseRay(destination_);

				if (!result)
				{
					if (mouse != nullptr)
					{
						destination_ = mouse->GetIntersectionYAxisPlane(worldPosition.y, 2000.f);
						//clickedFace = map->GetNavMesh()->GetNavFaceFromPositionXZ(destination_, float4::DOWN, height);

						//if (nullptr != clickedFace)
						//{
						//	destination_.y = height;
						//}
					}
				}
			}

			destinations_.clear();
			if ((destination_ - worldPosition).Len3D() > 400.f)
			{
				LumiaMap* map = level->GetMap();
				if (nullptr != map && nullptr != mouse)
				{
					GameEngineTime::GetInst().TimeCheck();

					if (result)
					{
						destinations_ = map->FindPath(worldPosition, destination_);
						if (destinations_.size() > 0)
						{
							destinations_[0] = destination_;
						}
						GameEngineTime::GetInst().TimeCheck();
						pathFindTime = GameEngineTime::GetInst().GetDeltaTime();

						destination_ = worldPosition;
					}
				}
			}
		}

		GameEngineLevelControlWindow* controlWindow = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");
		if (nullptr != controlWindow)
		{
			controlWindow->AddText("Path find time elapsed : " + std::to_string(pathFindTime) + " sec");

			controlWindow->AddText("hi");
			controlWindow->AddText("nice to meet you");
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
			if (true == currentMap_->GetNavMesh()->CheckIntersects(nextMovePosition + float4{ 0.0f, FT::MAX_HEIGHT, 0.0f }, float4::DOWN, temp))
			{
				GetTransform()->SetWorldPosition(nextMovePosition);
			}
			else
			{
				destination_ = worldPosition;
			}
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

		if (nullptr != currentNavFace_)
		{
			float Dist = currentNavFace_->YCheck(GetTransform());
			GetTransform()->SetWorldMove({ 0.0f, -Dist, 0.0f });
		}

		Character::Update(_deltaTime);
	}
}

void Rio::InitSpawnPoint(const float4& _position)
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

	// 부모인 NaviActor가 항상 자신의 위치노드를 업데이트함

	//else
	//{
	//	currentNavMesh_ = currentMap_->GetNavMesh(GetTransform()->GetWorldPosition());
	//}

	//if (nullptr == currentNavMesh_)
	//{
	//	GameEngineDebug::MsgBox("초기 캐릭터 위치가 네비게이션 메쉬 위에 있지 않습니다.");
	//}
}



void Rio::initRendererAnimation()
{
	renderer_ = CreateTransformComponent<GameEngineFBXRenderer>();
	renderer_->SetFBXMesh("Rio_Run.fbx", "TextureDeferredLightAni");

	renderer_->GetTransform()->SetLocalScaling({ 100.f, 100.f, 100.f });
	renderer_->GetTransform()->SetLocalRotationDegree({ -90.f,0.0f });

	renderer_->CreateFBXAnimation("Run", "Rio_Run.fbx", 0);
	renderer_->CreateFBXAnimation("Wait", "Rio_Wait.fbx", 0);
	renderer_->ChangeFBXAnimation("Wait");
}

void Rio::initInput()
{
	GameEngineInput::GetInst().CreateKey("LButton", VK_LBUTTON);
	GameEngineInput::GetInst().CreateKey("RButton", VK_RBUTTON);
}

void Rio::initState()
{
	normalState_.CreateState(MakeState(Rio, Normal));
	normalState_.CreateState(MakeState(Rio, CrowdControl));
}

void Rio::startNormal()
{
	renderer_->ChangeFBXAnimation("Wait");
}

void Rio::updateNormal(float _deltaTime)
{
}

void Rio::startCrowdControl()
{
}

void Rio::updateCrowdControl(float _deltaTime)
{
}

void Rio::endCrowdControl()
{
}

void Rio::startIdle()
{
}

void Rio::updateIdle(float _deltaTime)
{
}

void Rio::startRun()
{
}

void Rio::updateRun(float _deltaTime)
{
}

void Rio::startStun()
{
}

void Rio::updateStun(float _deltaTime)
{
}
