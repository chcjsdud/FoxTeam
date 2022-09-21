#include "PreCompile.h"
#include "Rio.h"
#include "MousePointer.h"
#include "LumiaMap.h"

#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineLevelControlWindow.h>
#include <GameApp/LumiaLevel.h>

Rio::Rio()
	: Character()
	, renderer_(nullptr)
	, mousePressDelay_(FT::Char::MOUSE_PRESS_DELAY)
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
	assert(mouse_ != nullptr);
	assert(currentMap_ != nullptr);

	if (!TempFlag)
	{
		return;
	}

	mainState_.Update(_deltaTime);

	GameEngineLevelControlWindow* controlWindow = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");
	{
		if (nullptr != controlWindow)
		{
			controlWindow->AddText("MainState : " + mainState_.GetCurrentState()->Name_);
			controlWindow->AddText("NormalState : " + normalState_.GetCurrentState()->Name_);
			controlWindow->AddText("Character Pos : " + 
				std::to_string(transform_.GetWorldPosition().x) + ", " + 
				std::to_string(transform_.GetWorldPosition().y) + ", " + 
				std::to_string(transform_.GetWorldPosition().z) + ", ");


		}
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

void Rio::Move(const float4& _position)
{
	destination_ = _position;
	destinations_.clear();
}

void Rio::MoveWithPathFind(const float4& _position)
{
	destination_ = _position;

	destinations_.clear();

	if ((destination_ - transform_.GetWorldPosition()).Len3D() > 300.f)
	{
		destinations_ = currentMap_->FindPath(transform_.GetWorldPosition(), destination_);
		if (destinations_.size() > 0)
		{
			destinations_[0] = destination_;
		}

		destination_ = transform_.GetWorldPosition();
	}
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
	mainState_.CreateState(MakeState(Rio, NormalState));
	mainState_.CreateState(MakeState(Rio, CrowdControlState));

	normalState_.CreateState(MakeState(Rio, Wait));
	normalState_.CreateState(MakeState(Rio, Run));

	mainState_ << "NormalState";
}

void Rio::inputProcess(float _deltaTime)
{
	bool result = false;
	float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.f);
	if (GameEngineInput::Down("LButton"))
	{
		result = currentMap_->GetNavMesh()->GetIntersectionPointFromMouseRay(destination_);
		if (result)
		{
			Move(destination_);
		}
		else
		{
			destination_ = mousePosition;
		}
	}

	if (GameEngineInput::Press("LButton"))
	{
		mousePressDelay_ -= _deltaTime;

		if (mousePressDelay_ <= 0)
		{
			result = currentMap_->GetNavMesh()->GetIntersectionPointFromMouseRay(destination_);

			if (result)
			{
				Move(destination_);
			}
			else
			{
				destination_ = mousePosition;
			}

			mousePressDelay_ = FT::Char::MOUSE_PRESS_DELAY;
		}
	}

	if (GameEngineInput::Up("LButton"))
	{
		bool result = false;
		result = currentMap_->GetNavMesh()->GetIntersectionPointFromMouseRay(destination_);
		if (result)
		{
			result = currentMap_->GetNavMesh()->GetIntersectionPointFromMouseRay(destination_);

			if (result)
			{
				MoveWithPathFind(mousePosition);
			}
			else
			{
				destination_ = mousePosition;
			}

		}
	}
}

void Rio::moveProcess(float _deltaTime)
{
	float4 worldPosition = transform_.GetWorldPosition();
	worldPosition.y = destination_.y;
	if ((destination_ - worldPosition).Len3D() > 10.f)
	{
		moveTick(_deltaTime, worldPosition);
	}
	else
	{
		if (!destinations_.empty())
		{
			destination_ = destinations_.back();
			destinations_.pop_back();

			// 여기서 한번 더 해주지 않으면 움직임이 1 프레임 손실된다.
			if ((destination_ - worldPosition).Len3D() > 10.f)
			{
				moveTick(_deltaTime, worldPosition);
			}
		}
		else
		{
			normalState_ << "Wait";
			return;
		}
	}
}

void Rio::moveTick(float _deltaTime, const float4& _startPosition)
{
	direction_ = destination_ - _startPosition;
	direction_.Normalize3D();

	float4 cross = float4::Cross3D(direction_, { 0.0f, 0.0f, 1.0f });
	cross.Normalize3D();

	float angle = float4::DegreeDot3DToACosAngle(direction_, { 0.0f, 0.0f, 1.0f });

	GetTransform()->SetLocalRotationDegree({ 0.0f, angle * -cross.y, 0.0f });

	float4 moveSpeed = direction_ * BASE_SPEED * _deltaTime;
	float4 nextMovePosition = _startPosition + moveSpeed;

	float temp;
	if (true == currentMap_->GetNavMesh()->CheckIntersects(nextMovePosition + float4{ 0.0f, FT::Map::MAX_HEIGHT, 0.0f }, float4::DOWN, temp))
	{
		renderer_->ChangeFBXAnimation("Run");
		GetTransform()->SetWorldPosition(nextMovePosition);
	}
	else
	{
		destination_ = _startPosition;
	}
}

void Rio::startNormalState()
{
	normalState_ << "Wait";
}

void Rio::updateNormalState(float _deltaTime)
{
	

	// Normal State 업데이트
	normalState_.Update(_deltaTime);
	Character::Update(_deltaTime);

	if (nullptr != currentNavFace_)
	{
		float Dist = currentNavFace_->YCheck(GetTransform());
		GetTransform()->SetWorldMove({ 0.0f, -Dist, 0.0f });
	}
}

void Rio::startCrowdControlState()
{
}

void Rio::updateCrowdControlState(float _deltaTime)
{
	crowdControlState_.Update(_deltaTime);
}

void Rio::endCrowdControlState()
{

}

void Rio::startWait()
{
	renderer_->ChangeFBXAnimation("Wait");
}

void Rio::updateWait(float _deltaTime)
{
	inputProcess(_deltaTime);

	float4 worldPosition = GetTransform()->GetWorldPosition();
	worldPosition.y = destination_.y;
	if ((destination_ - worldPosition).Len3D() > 10.f)
	{
		normalState_ << "Run";
		return;
	}
}

void Rio::startRun()
{
}

void Rio::updateRun(float _deltaTime)
{
	inputProcess(_deltaTime);
	moveProcess(_deltaTime);
}

void Rio::startStun()
{
}

void Rio::updateStun(float _deltaTime)
{
}
