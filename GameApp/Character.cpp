#include "PreCompile.h"

#include <GameEngine/GameEngineCollision.h>

#include "Character.h"
#include "LumiaLevel.h"
#include "MousePointer.h"
#include "LumiaMap.h"
#include "ItemBox.h"
#include "ItemBase.h"
#include "PlayerInfoManager.h"
//#include "PlayerUIController.h"
#include <GameEngine/GameEngineLevelControlWindow.h>
#include "CharStatPacket.h"
#include "GameServer.h"
#include "GameClient.h"

Character::Character()
	: collision_(nullptr)
	, currentNavFace_(nullptr)
	, currentNavMesh_(nullptr)
	, itemBoxmanager_(nullptr)
	, currentMap_(nullptr)
	, mouse_(nullptr)
	, renderer_(nullptr)
	, bFocused_(false)
	, attackCooldown_(0.f)
	, attackTime_(0.f)
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
	currentNavFace_ = currentNavMesh_->CurrentCheck(&transform_, float4::DOWN);
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


	LumiaLevel* level = GetLevelConvert<LumiaLevel>();
	if (nullptr != level)
	{
		itemBoxmanager_ = level->GetItemBoxManager();
		inventory_.resize(10);

		LumiaLevel* level = dynamic_cast<LumiaLevel*>(level_);


		currentMap_ = level->GetMap();
		mouse_ = level->GetMousePointer();

		if (nullptr == currentMap_)
		{
			GameEngineDebug::MsgBoxError("level에 Map 정보가 없습니다.");
		}

		if (nullptr == mouse_)
		{
			GameEngineDebug::MsgBoxError("level에 MousePointer 정보가 없습니다.");
		}
	}


	//if (nullptr == level)
	//{
	//	GameEngineDebug::MsgBoxError("level 변환에 실패했습니다. class GameEngineLevel to " + std::string(typeid(LumiaLevel).name()));
	//}



	//PlayerUIController* UIController = GetLevel()->CreateActor<PlayerUIController>();

}

void Character::Update(float _DeltaTime)
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	if (0.0f >= actorStat_.HP)
	{
		if (true == isPlayerDead_)
		{
			mainState_.Update(_DeltaTime);
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
		// direction 에 따라 로테이션을 바꿔 주고
		// 적절한 애니메이션으로 바꿔 주는 코드가 여기에도 존재해야 함.


		return;
	}

	checkCurrentNavFace();
	checkItemBox();

	attackCooldown_ += _DeltaTime;

	mainState_.Update(_DeltaTime);

	GameEngineLevelControlWindow* controlWindow = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineLevelControlWindow>("LevelControlWindow");
	if (nullptr != controlWindow)
	{
		controlWindow->AddText("MainState : " + mainState_.GetCurrentStateName());
		controlWindow->AddText("NormalState : " + normalState_.GetCurrentStateName());
		controlWindow->AddText("CrowdControlState : " + crowdControlState_.GetCurrentStateName());
		controlWindow->AddText("AttackState : " + attackState_.GetCurrentStateName());
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
	// 박스가 열려있는 상태에서
	// 열려있는 UI의 아이템을 클릭하는 경우
	// 그 아이템을 가지고온다.

	ItemBase* Item = itemBoxmanager_->GetItemFromItemBox(_index);

	if (nullptr == Item)
	{
		GameEngineDebug::MsgBoxError("아이템을 찾지 못했습니다.");
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

	checkItemRecipes();
}

void Character::checkItemBox()
{
	// Test용 
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

	// Player와 SelectBox가 서로 충돌상태인지를 체크

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

		// 현재 박스를 누른 후에 커서를 SelectBox 바깥으로 옮기면 박스가 열리지 않음
		// Player가 조건이 만족하는 경우 박스를 연다.
		// ItemBox UI 를 열음
		itemBoxmanager_->OpenItemBox();
		return;
	}



	itemBoxmanager_->CloseItemBox();


	// CloseItemBox
	// Player가 아이템박스로부터 멀어진 경우
	// UI가 닫힌다.
	// SelectBox도 nullptr로 초기화
	// 초기화하지 않으면 SelectBox 근처에 다가가면 UI가 계속 열리게 됨
}

void Character::checkItemRecipes()
{
	std::vector<std::vector<int>> cases =
		GameEngineMath::Combination(inventory_.size(), 2);

	std::map<CombineItem, std::string>& itemRecipes = itemBoxmanager_->GetAllItemRecipes();
	std::list<ItemBase*> itemList = itemBoxmanager_->GetAllItemList();

	for (size_t i = 0; i < cases.size(); i++)
	{
		int left = cases[i][0];
		int right = cases[i][1];

		if (nullptr == inventory_[left] ||
			nullptr == inventory_[right])
		{
			continue;
		}

		// 아이템 조합가능여부를 판별
		std::map<CombineItem, std::string>::iterator iter = itemRecipes.end();

		iter = itemRecipes.find(CombineItem(inventory_[left]->GetName(), inventory_[right]->GetName()));

		if (itemRecipes.end() == iter)
		{
			continue;
		}

		std::string resultItemName = iter->second;
		bool isFind = false;

		for (const auto& item : itemList)
		{
			if (item->GetName() != resultItemName)
			{
				continue;
			}

			isFind = true;

			inventory_[left]->Release();
			inventory_[right]->Release();
				
			inventory_[left] = nullptr;
			inventory_[right] = nullptr;

			for (auto& invenItem : inventory_)
			{
				if (nullptr != invenItem)
				{
					continue;
				}

				invenItem = item->Copy();
				break;
			}

			break;
		}

		if (false == isFind)
		{
			GameEngineDebug::MsgBoxError("조합 실패, 아이템리스트에 존재하지 않음");
		}
	}
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
	GetTransform()->SetWorldPosition(_position);
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
	mainState_.CreateState(MakeStateWithEnd(Character, NormalState));
	mainState_.CreateState(MakeState(Character, AttackState));
	mainState_.CreateState(MakeState(Character, CrowdControlState));
	mainState_.CreateState(MakeState(Character, DeathState));

	normalState_.CreateState(MakeState(Character, Watch));
	normalState_.CreateState(MakeState(Character, Stop));
	normalState_.CreateState(MakeState(Character, Run));
	normalState_.CreateState(MakeState(Character, Chase));

	attackState_.CreateState(MakeState(Character, BasicAttack));
	attackState_.CreateState(MakeState(Character, BasicAttackDone));
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
			// 공격 처리
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

			// 여기서 한번 더 해주지 않으면 움직임이 1 프레임 손실된다.
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
	setRotationTo(destination_, _startPosition);

	float4 moveSpeed = direction_ * actorStat_.MovementSpeed * _deltaTime;
	float4 nextMovePosition = _startPosition + moveSpeed;

	float temp;
	if (true == currentMap_->GetNavMesh()->CheckIntersects(nextMovePosition + float4{ 0.0f, FT::Map::MAX_HEIGHT, 0.0f }, float4::DOWN, temp))
	{

		changeAnimationRun();
		currentAnimation_ = eCurrentAnimation::Run;
		GetTransform()->SetWorldPosition(nextMovePosition);
	}
	else
	{
		destination_ = _startPosition;
	}
}

void Character::setRotationTo(const float4& _destination, const float4 _startPosition)
{
	direction_ = _destination - _startPosition;
	direction_.Normalize3D();

	float4 cross = float4::Cross3D(direction_, { 0.0f, 0.0f, 1.0f });
	cross.Normalize3D();

	float angle = float4::DegreeDot3DToACosAngle(direction_, { 0.0f, 0.0f, 1.0f });

	transform_.SetLocalRotationDegree({ 0.0f, angle * -cross.y, 0.0f });
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


	// Normal State 업데이트
	normalState_.Update(_deltaTime);


	// 업데이트 후에 높이 체크
	checkCurrentNavFace();
	if (nullptr != currentNavFace_)
	{
		float Dist = currentNavFace_->YCheck(GetTransform());
		transform_.SetWorldMove({ 0.0f, -Dist, 0.0f });
	}
}

void Character::endNormalState()
{
	destination_ = transform_.GetWorldPosition();
	destinations_.clear();
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

	checkCurrentNavFace();
	if (nullptr != currentNavFace_)
	{
		float Dist = currentNavFace_->YCheck(GetTransform());
		transform_.SetWorldMove({ 0.0f, -Dist, 0.0f });
	}
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
	currentAnimation_ = eCurrentAnimation::Wait;
}

void Character::updateWatch(float _deltaTime)
{
	inputProcess(_deltaTime);

	float4 worldPosition = GetTransform()->GetWorldPosition();
	worldPosition.y = destination_.y;
	if ((destination_ - worldPosition).Len3D() > FT::Char::MOVE_FINISH_CHECK_DISTANCE)
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

	//if (distance > actorStat_.VisionRange)
	//{
	//	normalState_ << "Watch";
	//	target_ = nullptr;
	//	return;
	//}

	if (distance < actorStat_.AttackRange)
	{
		setRotationTo(targetPosition, playerPosition);

		if (attackCooldown_ > 1.0f / actorStat_.AttackSpeed)
		{
			mainState_ << "AttackState";
			attackState_ << "BasicAttack";
			return;
		}

		if (currentAnimation_ == eCurrentAnimation::BasicAttack)
		{
			if (renderer_->IsCurrentAnimationEnd())
			{
				changeAnimationWait();
				currentAnimation_ = eCurrentAnimation::Wait;
			}
		}
		else if (currentAnimation_ == eCurrentAnimation::Run)
		{
			changeAnimationWait();
			currentAnimation_ = eCurrentAnimation::Wait;
		}

	}
	else
	{
		Move(targetPosition);
		moveProcess(_deltaTime);
	}

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
	attackTime_ = 0.0f;
	changeAnimationBasicAttack();
	currentAnimation_ = eCurrentAnimation::BasicAttack;
}

void Character::updateBasicAttack(float _deltaTime)
{
	attackTime_ += _deltaTime;

	if (attackTime_ < actorStat_.AttackStartTime / actorStat_.AttackSpeed)
	{
		GameEngineCollision* rayCol = mouse_->GetRayCollision();

		bool result = false;
		float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.f);
		if (GameEngineInput::Press("LButton") || GameEngineInput::Down("LButton"))
		{

			Character* otherCharacter = getMousePickedCharacter();

			if (target_ != otherCharacter)
			{
				inputProcess(_deltaTime);

				float4 worldPosition = GetTransform()->GetWorldPosition();
				worldPosition.y = destination_.y;
				if ((destination_ - worldPosition).Len3D() > FT::Char::MOVE_FINISH_CHECK_DISTANCE)
				{
					normalState_ << "Run";
				}

				mainState_ << "NormalState";
				return;
			}
		}
	}

	if (attackTime_ > actorStat_.AttackStartTime / actorStat_.AttackSpeed)
	{
		attackState_ << "BasicAttackDone";
		return;
	}
}

void Character::startBasicAttackDone()
{
	target_->Damage(actorStat_.AttackPower);
	CharStatPacket packet;
	packet.SetStat(target_->actorStat_);
	packet.SetTargetIndex(target_->GetIndex());

	if (true == GameServer::GetInstance()->IsOpened())
	{
		GameServer::GetInstance()->Send(&packet);
	}
	else if (true == GameClient::GetInstance()->IsConnected())
	{
		GameClient::GetInstance()->Send(&packet);
	}
	
}

void Character::updateBasicAttackDone(float _deltaTime)
{
	attackTime_ += _deltaTime;
	if (attackTime_ > actorStat_.AttackEndTime / actorStat_.AttackSpeed)
	{
		attackCooldown_ = 0.0f;
		mainState_ << "NormalState";
		return;
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
