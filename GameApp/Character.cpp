#include "PreCompile.h"

#include <GameEngine/GameEngineCollision.h>

#include "Character.h"
#include "LumiaLevel.h"
#include "MousePointer.h"
#include "LumiaMap.h"
#include "ItemBox.h"
#include "PlayerInfoManager.h"

Character::Character()
	: collision_(nullptr)
	, currentNavFace_(nullptr)
	, currentNavMesh_(nullptr)
	, itemBoxmanager_(nullptr)
	, currentMap_(nullptr)
	, mouse_(nullptr)
	, renderer_(nullptr)
	, bFocused_(false)
{
}

Character::~Character()
{

}

void Character::SetCurrentNavMesh(NavMesh* _NavMesh)
{
	currentNavMesh_ = _NavMesh;
	currentNavFace_ = currentNavMesh_->CurrentCheck(GetTransform(), float4::DOWN);
}

void Character::Start()
{
	initRendererAndAnimation();
	initInput();
	initState();

	collision_ = CreateTransformComponent<GameEngineCollision>();
	collision_->GetTransform()->SetLocalScaling(150.0f);
	collision_->SetCollisionGroup(CollisionGroup::Player);
	collision_->SetCollisionType(CollisionType::AABBBox3D);

	itemBoxmanager_ = GetLevelConvert<LumiaLevel>()->GetItemBoxManager();


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

	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	if (false == bFocused_)
	{
		// direction �� ���� �����̼��� �ٲ� �ְ�
		// ������ �ִϸ��̼����� �ٲ� �ִ� �ڵ尡 ���⿡�� �����ؾ� ��.

		
		return;
	}



	checkCurrentNavFace();

	checkItemBox();

	mainState_.Update(_DeltaTime);
}

void Character::checkCurrentNavFace()
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
			SetCurrentNavFace(GetLevelConvert<LumiaLevel>()->GetMap()->GetNavMesh()->CurrentCheck(GetTransform(), float4::DOWN));
		}
	}
}

void Character::getItem()
{
	// �ڽ��� �����ִ� ���¿���
	// �����ִ� UI�� �������� Ŭ���ϴ� ���
	// �� �������� ������´�.
}

void Character::checkItemBox()
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
		itemBoxmanager_->OpenItemBox();
		return;
	}

	itemBoxmanager_->CloseItemBox();


	// CloseItemBox
	// Player�� �����۹ڽ��κ��� �־��� ���
	// UI�� ������.
	// SelectBox�� nullptr�� �ʱ�ȭ
	// �ʱ�ȭ���� ������ SelectBox ��ó�� �ٰ����� UI�� ��� ������ ��
}

void Character::InitSpawnPoint(const float4& _position)
{
	//float4 spawnPoint = { -6780.f, 0.0f, -780.f };
	//float4 spawnPoint = { -2500.f, 0.0f, 10000.f };
	//float4 spawnPoint = { 0.f, 0.0f, 0.f };
	GetTransform()->SetLocalPosition(_position);
	destination_ = _position;

	currentMap_ = GetLevelConvert<LumiaLevel>()->GetMap();

	if (nullptr == currentMap_)
	{
		GameEngineDebug::MsgBox("������ ���� ��ġ���� �ʾҽ��ϴ�.");
	}

	// �θ��� NaviActor�� �׻� �ڽ��� ��ġ��带 ������Ʈ��

	//else
	//{
	//	currentNavMesh_ = currentMap_->GetNavMesh(GetTransform()->GetWorldPosition());
	//}

	//if (nullptr == currentNavMesh_)
	//{
	//	GameEngineDebug::MsgBox("�ʱ� ĳ���� ��ġ�� �׺���̼� �޽� ���� ���� �ʽ��ϴ�.");
	//}
}

void Character::Move(const float4& _position)
{
	destination_ = _position;
	destinations_.clear();
}

void Character::MoveWithPathFind(const float4& _position)
{
	destination_ = _position;

	destinations_.clear();

	if ((destination_ - transform_.GetWorldPosition()).Len3D() > 400.f)
	{
		destinations_ = currentMap_->FindPath(transform_.GetWorldPosition(), destination_);
		if (destinations_.size() > 0)
		{
			destinations_[0] = destination_;
		}

		destination_ = transform_.GetWorldPosition();
	}
}

void Character::ChangeAnimation(std::string _animationName)
{
		if ("" == _animationName)
		{
			return;
		}
		
		curAnimation_ = _animationName;
		renderer_->ChangeFBXAnimation(_animationName);
}

void Character::initInput()
{
	GameEngineInput::GetInst().CreateKey("LButton", VK_LBUTTON);
	GameEngineInput::GetInst().CreateKey("RButton", VK_RBUTTON);
	GameEngineInput::GetInst().CreateKey("Q", 'Q');
	GameEngineInput::GetInst().CreateKey("W", 'W');
	GameEngineInput::GetInst().CreateKey("E", 'E');
	GameEngineInput::GetInst().CreateKey("R", 'R');
	GameEngineInput::GetInst().CreateKey("A", 'A');
	GameEngineInput::GetInst().CreateKey("S", 'S');
	GameEngineInput::GetInst().CreateKey("M", 'M');
	GameEngineInput::GetInst().CreateKey("Tab", VK_TAB);
}

void Character::initState()
{
	mainState_.CreateState(MakeState(Character, NormalState));
	mainState_.CreateState(MakeState(Character, AttackState));
	mainState_.CreateState(MakeState(Character, CrowdControlState));

	normalState_.CreateState(MakeState(Character, Watch));
	normalState_.CreateState(MakeState(Character, Stop));
	normalState_.CreateState(MakeState(Character, Run));

	attackState_.CreateState(MakeState(Character, NoAttack));


	mainState_ << "NormalState";


	normalState_ << "Watch";
	attackState_ << "NoAttack";
}


void Character::inputProcess(float _deltaTime)
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
	else if (GameEngineInput::Press("LButton"))
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
	else if (GameEngineInput::Up("LButton"))
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

void Character::moveProcess(float _deltaTime)
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

			// ���⼭ �ѹ� �� ������ ������ �������� 1 ������ �սǵȴ�.
			if ((destination_ - worldPosition).Len3D() > 10.f)
			{
				moveTick(_deltaTime, worldPosition);
			}
		}
		else
		{
			normalState_ << "Watch";
			return;
		}
	}
}

void Character::moveTick(float _deltaTime, const float4& _startPosition)
{

	direction_ = destination_ - _startPosition;
	direction_.Normalize3D();

	float4 cross = float4::Cross3D(direction_, { 0.0f, 0.0f, 1.0f });
	cross.Normalize3D();

	float angle = float4::DegreeDot3DToACosAngle(direction_, { 0.0f, 0.0f, 1.0f });

	GetTransform()->SetLocalRotationDegree({ 0.0f, angle * -cross.y, 0.0f });

	float4 moveSpeed = direction_ * stat_.MovementSpeed * _deltaTime;
	float4 nextMovePosition = _startPosition + moveSpeed;

	float temp;
	if (true == currentMap_->GetNavMesh()->CheckIntersects(nextMovePosition + float4{ 0.0f, FT::Map::MAX_HEIGHT, 0.0f }, float4::DOWN, temp))
	{
		
		changeAnimationRun();
		GetTransform()->SetWorldPosition(nextMovePosition);
	}
	else
	{
		destination_ = _startPosition;
	}
}

void Character::startNormalState()
{
	normalState_ << "Watch";
}

void Character::updateNormalState(float _deltaTime)
{
	// Normal State ������Ʈ
	normalState_.Update(_deltaTime);

	// ������Ʈ �Ŀ� ���� üũ
	checkCurrentNavFace();
	if (nullptr != currentNavFace_)
	{
		float Dist = currentNavFace_->YCheck(GetTransform());
		GetTransform()->SetWorldMove({ 0.0f, -Dist, 0.0f });
	}
}

void Character::startCrowdControlState()
{
}

void Character::updateCrowdControlState(float _deltaTime)
{
	crowdControlState_.Update(_deltaTime);
}

void Character::startAttackState()
{
}

void Character::updateAttackState(float _deltaTime)
{
	attackState_.Update(_deltaTime);
}

void Character::startWatch()
{
	changeAnimationWait();
}

void Character::updateWatch(float _deltaTime)
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

void Character::startStop()
{
}

void Character::updateStop(float _deltaTime)
{
}

void Character::startRun()
{

}

void Character::updateRun(float _deltaTime)
{
	inputProcess(_deltaTime);
	moveProcess(_deltaTime);
}

void Character::startStun()
{
}

void Character::updateStun(float _deltaTime)
{
}

void Character::startNoAttack()
{
}

void Character::updateNoAttack(float _deltaTime)
{
}

void Character::startBasicAttack()
{
}

void Character::updateBasicAttack(float _deltaTime)
{
}

void Character::startQSkill()
{
	onStartQSkill();
}

void Character::updateQSkill(float _deltaTime)
{
	onUpdateQSkill(_deltaTime);
}

void Character::startWSkill()
{
	onStartWSkill();
}

void Character::updateWSkill(float _deltaTime)
{
	onUpdateWSkill(_deltaTime);
}

void Character::startESkill()
{
	onStartESkill();
}

void Character::updateESkill(float _deltaTime)
{
	onUpdateESkill(_deltaTime);
}

void Character::startRSkill()
{
	onStartRSkill();
}

void Character::updateRSkill(float _deltaTime)
{
	onUpdateRSkill(_deltaTime);
}

void Character::startDSkill()
{
	onStartDSkill();
}

void Character::updateDSkill(float _deltaTime)
{
	onUpdateDSkill(_deltaTime);
}
