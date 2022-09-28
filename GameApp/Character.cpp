#include "PreCompile.h"

#include <GameEngine/GameEngineCollision.h>

#include "Character.h"
#include "LumiaLevel.h"
#include "MousePointer.h"
#include "LumiaMap.h"
#include "ItemBox.h"
#include "PlayerInfoManager.h"
//#include "PlayerUIController.h"
#include <GameEngine/GameEngineLevelControlWindow.h>


Character::Character()
	: collision_(nullptr)
	, currentNavFace_(nullptr)
	, currentNavMesh_(nullptr)
	, itemBoxmanager_(nullptr)
	, currentMap_(nullptr)
	, mouse_(nullptr)
	, renderer_(nullptr)
	, bFocused_(false)
	, attackDelay_(0.f)
	, target_(nullptr)
	, isPlayerDead_(false)
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
	collision_->SetCollisionGroup(eCollisionGroup::Player);
	collision_->SetCollisionType(CollisionType::OBBBox3D);

	itemBoxmanager_ = GetLevelConvert<LumiaLevel>()->GetItemBoxManager();
	inventory_.resize(10);

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

	//PlayerUIController* UIController = GetLevel()->CreateActor<PlayerUIController>();
	
}

void Character::Update(float _DeltaTime)
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	if (0.0f >= actorStat_.HP)
	{
		if (true == isPlayerDead_)
		{
			return;
		}
	
		mainState_.ChangeState("DeathState", true);
		deathState_.ChangeState("PlayerDeath", true);
		isPlayerDead_ = true;
		return;
	}

	if (nullptr != collision_ &&
		collision_->IsUpdate())
	{
		GetLevel()->PushDebugRender(collision_->GetTransform(), CollisionType::OBBBox3D, float4::RED);
	}



	if (false == bFocused_)
	{
		// direction �� ���� �����̼��� �ٲ� �ְ�
		// ������ �ִϸ��̼����� �ٲ� �ִ� �ڵ尡 ���⿡�� �����ؾ� ��.


		return;
	}

	checkCurrentNavFace();
	checkItemBox();

	mainState_.Update(_DeltaTime);

	GameEngineLevelControlWindow* controlWindow = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");
	if (nullptr != controlWindow)
	{
		controlWindow->AddText("MainState : " + mainState_.GetCurrentStateName());
		controlWindow->AddText("NormalState : " + normalState_.GetCurrentStateName());
		controlWindow->AddText("CrowdControlState : " + crowdControlState_.GetCurrentStateName());
		controlWindow->AddText("AttackState : " + attackState_.GetCurrentStateName());

		for (int i = 0; i < pm->GetPlayerList().size(); i++)
		{
			controlWindow->AddText("Player " + std::to_string(i) + "curHP(Server) : " + std::to_string(pm->GetPlayerList()[i].stat_->HP));
		}
		
	}
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

void Character::getItem(int _index)
{
	// �ڽ��� �����ִ� ���¿���
	// �����ִ� UI�� �������� Ŭ���ϴ� ���
	// �� �������� ������´�.

	ItemBase* Item = itemBoxmanager_->GetItemFromItemBox(_index);

	if (nullptr == Item)
	{
		GameEngineDebug::MsgBoxError("�������� ã�� ���߽��ϴ�.");
	}

	for (auto& invenItem : inventory_)
	{
		if (nullptr != invenItem)
		{
			continue;
		}

		invenItem = Item;
		itemBoxmanager_->DeleteItemFromItemBox(_index);
		break;
	}
}

void Character::checkItemBox()
{
	// Test�� 
	if (true == itemBoxmanager_->isOpen())
	{
		if (true == GameEngineInput::GetInst().Down("1"))
		{
			getItem(0);
		}
		if (true == GameEngineInput::GetInst().Down("2"))
		{
			getItem(1);
		}
		if (true == GameEngineInput::GetInst().Down("3"))
		{
			getItem(2);
		}
	}

	// Player�� SelectBox�� ���� �浹���������� üũ

	GameEngineCollision* OtherCol = collision_->GetCollision(static_cast<int>(eCollisionGroup::ItemBox));

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

Character* Character::getMousePickedCharacter()
{
	GameEngineCollision* mousePickedCollision = mouse_->GetRayCollision()->GetCollision(eCollisionGroup::Player);
	GameEngineActor* mousePickedActor = nullptr;
	Character* mousePickedCharacter = nullptr;
	if (nullptr != mousePickedCollision)
	{
		mousePickedActor = mousePickedCollision->GetActor();
		if (nullptr != mousePickedActor)
		{
			return mousePickedCharacter = dynamic_cast<Character*>(mousePickedActor);
		}
	}

	return nullptr;
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

void Character::Damage(float _amount)
{
	actorStat_.HP -= _amount;
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
	mainState_.CreateState(MakeState(Character, DeathState));

	normalState_.CreateState(MakeState(Character, Watch));
	normalState_.CreateState(MakeState(Character, Stop));
	normalState_.CreateState(MakeState(Character, Run));
	normalState_.CreateState(MakeState(Character, Chase));

	attackState_.CreateState(MakeState(Character, BasicAttack));
	attackState_.CreateState(MakeState(Character, QSkill));

	deathState_.CreateState(MakeState(Character, PlayerDeath));



	mainState_ << "NormalState";

	normalState_ << "Watch";
	
	deathState_ << "PlayerDeath";
}


void Character::inputProcess(float _deltaTime)
{
	GameEngineCollision* rayCol = mouse_->GetRayCollision();

	bool result = false;
	float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.f);
	if (GameEngineInput::Press("LButton") || GameEngineInput::Down("LButton"))
	{

		Character* otherCharacter = getMousePickedCharacter();
		if (nullptr != otherCharacter && otherCharacter != this)
		{
			// ���� ó��
			target_ = otherCharacter;
			normalState_ << "Chase";
			return;
		}
		else
		{
			target_ = nullptr;
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

	float4 moveSpeed = direction_ * actorStat_.MovementSpeed * _deltaTime;
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
}

void Character::updateNormalState(float _deltaTime)
{


	if (true == GameEngineInput::GetInst().Down("Q"))
	{
		mainState_.ChangeState("AttackState", true);
		attackState_.ChangeState("QSkill", true);
		return;
	}


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

void Character::startDeathState()
{
	
}

void Character::updateDeathState(float _deltaTime)
{
	deathState_.Update(_deltaTime);
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

void Character::startChase()
{
}

void Character::updateChase(float _deltaTime)
{
	inputProcess(_deltaTime);
	if (target_ == nullptr)
	{
		normalState_ << "Watch";
		return;
	}


	float4 targetPosition = target_->GetTransform()->GetWorldPosition();
	float4 playerPosition = transform_.GetWorldPosition();
	float distance = float4::Calc_Len3D(playerPosition, targetPosition);
	if (distance > actorStat_.VisionRange)
	{
		normalState_ << "Watch";
		target_ = nullptr;
		return;
	}

	if (distance < actorStat_.AttackRange)
	{
		mainState_ << "AttackState";
		attackState_ << "BasicAttack";
		return;
	}

	Move(targetPosition);
	moveProcess(_deltaTime);
}

void Character::startStun()
{
	int a = 0;
}

void Character::updateStun(float _deltaTime)
{

	mainState_.ChangeState("NormalState");
	
	
}

void Character::startBasicAttack()
{
	attackDelay_ = 0;
	changeAnimationBasicAttack();
}

void Character::updateBasicAttack(float _deltaTime)
{	
	attackDelay_ += _deltaTime;

	if (attackDelay_ > 1.0f / actorStat_.AttackSpeed)
	{
		mainState_ << "NormalState";
	}
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

void Character::startPlayerDeath()
{
	onStartDeath();
}

void Character::updatePlayerDeath(float _deltaTime)
{
	onUpdateDeath(_deltaTime);
}
