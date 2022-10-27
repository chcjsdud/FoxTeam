#include "PreCompile.h"

#include <GameEngine/GameEngineCollision.h>

#include "Character.h"
#include "LumiaLevel.h"
#include "MousePointer.h"
#include "LumiaMap.h"
#include "ItemBox.h"
#include "ItemBase.h"
#include "PlayerInfoManager.h"
#include "PlayerUIController.h"
#include <GameEngine/GameEngineLevelControlWindow.h>
#include "CharStatPacket.h"
#include "GameServer.h"
#include "GameClient.h"
#include "ItemBoxManager.h"
#include "PacketSoundPlay.h"
#include "CharDeathPacket.h"

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
	, timerStun_(0.0f)
	, timerKnockback_(0.0f)
	, currentAnimation_(eCurrentAnimation::None)
	, myIndex_(-1)
	, uiController_(nullptr)
	, overrideAnimationName_("")
	, coolTimeQ_(0.0f)
	, coolTimeW_(0.0f)
	, coolTimeE_(0.0f)
	, coolTimeR_(0.0f)
	, coolTimeD_(0.0f)
	, slowTimer_(0.0f)
	, slowRatio_(0.0f)
	, bSlowFlag_(false)
{
	// 생성과 동시에 유닛타입 결정
	UnitType_ = UnitType::CHARACTER;
}

Character::~Character()
{

}

void Character::SetCurrentNavMesh(NavMesh* _NavMesh)
{
	currentNavMesh_ = _NavMesh;
	currentNavFace_ = currentNavMesh_->CurrentCheck(&transform_, float4::DOWN);
}

void Character::SetCharacterDeath()
{
	isPlayerDead_ = true;

	LumiaLevel* level = GetLevelConvert<LumiaLevel>();
	level->SubtractSurvivorCount();

	mainState_.ChangeState("DeathState", true);
	deathState_.ChangeState("PlayerDeath", true);
}

void Character::SetFraggerIndex(int _index)
{
	fraggerIndex_ = _index;
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
	// 현재 레벨이 루미아 레벨이 아닌 경우 처리하지 않음
	if (nullptr == level)
	{
		return;
	}

	itemBoxmanager_ = level->GetItemBoxManager();
	inventory_.resize(10);

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


	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	if (nullptr == uiController_)
	{
		uiController_ = GetLevel()->CreateActor<PlayerUIController>();
	}
	uiController_->SetJobType((JobType)(pm->GetMyPlayer().character_));

	equipedItem_.resize(static_cast<int>(EquipmentType::MAX));
	equipBuildItem_.resize(static_cast<int>(EquipmentType::MAX));

	// 현우아이템트리로 일단 고정
	SetEquipBuildItem("ImperialBurgonet", EquipmentType::HEAD);
	SetEquipBuildItem("CommandersArmor", EquipmentType::CHEST);
	SetEquipBuildItem("TindalosBand", EquipmentType::ARM);
	SetEquipBuildItem("TachyonBrace", EquipmentType::LEG);
	SetEquipBuildItem("WhiteCraneFan", EquipmentType::ACCESSORY);
	SetEquipBuildItem("PlasmaTonfa", EquipmentType::WEAPON);

	checkBuildItems();
}

void Character::Update(float _DeltaTime)
{
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	LumiaLevel* level = GetLevelConvert<LumiaLevel>();

	SlowCheck(_DeltaTime);

	if (true == isPlayerDead_)
	{
		collision_->Off();
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

	// 이 밑에서부터는 Focued 된 내 캐릭터에만 해당되는 행위들이다.

	if (uiController_ != nullptr)
	{
		if (stat_.HP <= (stat_.HPMax * 0.33f))
		{
			uiController_->GetBloodBackground()->Draw(true);

			if (true == isPlayerDead_)
			{
				uiController_->GetBloodBackground()->PlayerDead();
			}
		}
		else
		{
			uiController_->GetBloodBackground()->Draw(false);
		}

		if (1 >= level->GetSurvivorCount() && false == isPlayerDead_)
		{
			uiController_->GetBloodBackground()->Draw(false);
			mainState_.ChangeState("DeathState", true);
			deathState_.ChangeState("PlayerWinner", true);
		}
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


int Character::GetIndex() { return myIndex_; }

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
		LumiaLevel* level = GetLevelConvert<LumiaLevel>();
		if (nullptr != level)
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
		GameEngineDebug::MsgBox("아이템을 찾지 못했습니다.");
		return;
	}

	for (auto& invenItem : inventory_)
	{
		if (nullptr != invenItem)
		{
			continue;
		}

		invenItem = Item;
		//이건호 : 기존 UI를 비우고 인벤토리 벡터 전체를 받아오도록 설정
		uiController_->GetInventoryUI()->EmptySlot();
		uiController_->GetInventoryUI()->GetInventoryInfo(inventory_);
		itemBoxmanager_->DeleteItemFromItemBox(_index);

		std::list<ItemBase*>::iterator iter = allMyBuildItems_.begin();
		std::list<ItemBase*>::iterator iterErase = allMyBuildItems_.end();

		for (; iter != allMyBuildItems_.end(); ++iter)
		{
			if ((*iter) != Item)
			{
				continue;
			}

			iterErase = iter;
			break;
		}

		if (iterErase != allMyBuildItems_.end())
		{
			allMyBuildItems_.erase(iterErase);
		}

		break;
	}

	checkItemRecipes();
}

void Character::getItem(const std::string& _itemName)
{
	// 임의로 아이템을 인벤토리에 넣음

	ItemBase* item = itemBoxmanager_->GetItemFromItemList(_itemName);

	for (auto& invenItem : inventory_)
	{
		if (nullptr != invenItem)
		{
			continue;
		}

		invenItem = item;
		uiController_->GetInventoryUI()->EmptySlot();
		uiController_->GetInventoryUI()->GetInventoryInfo(inventory_);

		std::list<ItemBase*>::iterator iter = allMyBuildItems_.begin();
		std::list<ItemBase*>::iterator iterErase = allMyBuildItems_.end();

		for (; iter != allMyBuildItems_.end(); ++iter)
		{
			if ((*iter) != item)
			{
				continue;
			}

			iterErase = iter;
			break;
		}

		if (iterErase != allMyBuildItems_.end())
		{
			allMyBuildItems_.erase(iterErase);
		}

		break;
	}

	checkItemRecipes();
}

void Character::checkItemBox()
{
	if (itemBoxmanager_ == nullptr)
	{
		return;
	}

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

bool sortItemQueue(QueueItem _left, QueueItem _right)
{
	// 우선도에 따라 정렬 (루트)
	if (false == _left.isMyBuild_ &&
		true == _right.isMyBuild_)
	{
		return false;
	}

	return true;
}

void Character::checkItemRecipes()
{
	std::vector<std::vector<int>> cases =
		GameEngineMath::Combination(static_cast<int>(inventory_.size()), 2);

	std::map<CombineItem, ItemBase*>& itemRecipes = itemBoxmanager_->GetAllItemRecipes();

	queueItemMixing_.clear();

	for (int i = 0; i < cases.size(); i++)
	{
		int left = cases[i][0];
		int right = cases[i][1];

		if (nullptr == inventory_[left] ||
			nullptr == inventory_[right])
		{
			continue;
		}

		// 아이템 조합가능여부를 판별
		std::map<CombineItem, ItemBase*>::iterator iter = itemRecipes.end();

		CombineItem CI = CombineItem(inventory_[left], inventory_[right]);

		iter = itemRecipes.find(CI);

		if (itemRecipes.end() == iter)
		{
			continue;
		}

		QueueItem QI;
		QI.CI_ = CI;

		for (const auto& myItem : allMyBuildItems_)
		{
			if (myItem == (*iter).second)
			{
				QI.isMyBuild_ = true;
			}
		}

		// 조합 대기열에 등록
		queueItemMixing_.push_back(QI);

		queueItemMixing_.sort(sortItemQueue);
	}
}

void Character::mixingItem()
{
	if (queueItemMixing_.empty())
	{
		return;
	}

	CombineItem itemNames = queueItemMixing_.front().CI_;
	queueItemMixing_.pop_front();

	std::map<CombineItem, ItemBase*>& itemRecipes = itemBoxmanager_->GetAllItemRecipes();

	std::map<CombineItem, ItemBase*>::iterator iter = itemRecipes.end();
	iter = itemRecipes.find(itemNames);

	if (itemRecipes.end() == iter)
	{
		GameEngineDebug::MsgBoxError("없는 아이템 조합식입니다.");
		return;
	}

	for (auto& invenItem : inventory_)
	{
		if (nullptr == invenItem)
		{
			continue;
		}

		if (invenItem == itemNames.left_)
		{
			invenItem->Release();
			invenItem = nullptr;
			break;
		}
	}

	for (auto& invenItem : inventory_)
	{
		if (nullptr == invenItem)
		{
			continue;
		}

		if (invenItem == itemNames.right_)
		{
			invenItem->Release();
			invenItem = nullptr;
			break;
		}
	}

	for (auto& invenItem : inventory_)
	{
		if (nullptr != invenItem)
		{
			continue;
		}

		invenItem = iter->second->Copy();
		uiController_->GetInventoryUI()->EmptySlot();
		uiController_->GetInventoryUI()->GetInventoryInfo(inventory_);
		break;
	}

	checkItemRecipes();
}

void Character::SetEquipBuildItem(const std::string& _itemName, EquipmentType _type)
{
	equipBuildItem_[static_cast<int>(_type)] = reinterpret_cast<EquipmentItem*>
		(itemBoxmanager_->GetItemFromItemList(_itemName));
}


void Character::checkBuildItems()
{
	checkBuildItemsRecursive(reinterpret_cast<ItemBase*>(equipBuildItem_[static_cast<int>(EquipmentType::HEAD)]));
	checkBuildItemsRecursive(reinterpret_cast<ItemBase*>(equipBuildItem_[static_cast<int>(EquipmentType::CHEST)]));
	checkBuildItemsRecursive(reinterpret_cast<ItemBase*>(equipBuildItem_[static_cast<int>(EquipmentType::ARM)]));
	checkBuildItemsRecursive(reinterpret_cast<ItemBase*>(equipBuildItem_[static_cast<int>(EquipmentType::LEG)]));
	checkBuildItemsRecursive(reinterpret_cast<ItemBase*>(equipBuildItem_[static_cast<int>(EquipmentType::ACCESSORY)]));
	checkBuildItemsRecursive(reinterpret_cast<ItemBase*>(equipBuildItem_[static_cast<int>(EquipmentType::WEAPON)]));

	allMyBuildItems_;

	int a = 0;
}

void Character::checkBuildItemsRecursive(ItemBase* _item)
{
	std::map<CombineItem, ItemBase*> Recipes = itemBoxmanager_->GetAllItemRecipes();

	for (const auto& iter : Recipes)
	{
		if (iter.second->GetName() == _item->GetName())
		{
			allMyBuildItems_.push_back(iter.first.left_);
			allMyBuildItems_.push_back(iter.first.right_);

			checkBuildItemsRecursive(iter.first.left_);
			checkBuildItemsRecursive(iter.first.right_);
		}
	}
}

IUnit* Character::getMousePickedCharacter()
{
	GameEngineCollision* mousePickedCollision = mouse_->GetRayCollision()->GetCollision(eCollisionGroup::Player);
	GameEngineActor* mousePickedActor = nullptr;
	IUnit* mousePickedCharacter = nullptr;
	if (nullptr != mousePickedCollision)
	{
		mousePickedActor = mousePickedCollision->GetActor();
		if (nullptr != mousePickedActor)
		{
			return mousePickedCharacter = dynamic_cast<IUnit*>(mousePickedActor);
		}
	}

	{
		GameEngineCollision* mousePickedCollision = mouse_->GetRayCollision()->GetCollision(eCollisionGroup::Monster);
		GameEngineActor* mousePickedActor = nullptr;
		IUnit* mousePickedMonster = nullptr;
		if (nullptr != mousePickedCollision)
		{
			mousePickedActor = mousePickedCollision->GetActor();
			if (nullptr != mousePickedActor)
			{
				return mousePickedMonster = dynamic_cast<IUnit*>(mousePickedActor);
			}
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

	if ((destination_ - transform_.GetWorldPosition()).Len3D() > FT::Char::MOVE_PATH_FIND_DISTANCE)
	{
		destinations_ = currentMap_->FindPath(transform_.GetWorldPosition(), destination_);
		if (destinations_.size() > 0)
		{
			destinations_[0] = destination_;
		}

		destination_ = transform_.GetWorldPosition();
	}
}

void Character::ChangeAnimation(const std::string& _animationName, bool _bForce)
{
	if ("" == _animationName)
	{
		return;
	}

	curAnimationName_ = _animationName;
	renderer_->ChangeFBXAnimation(_animationName, _bForce);
}

void Character::ChangeOverrideAnimation(const std::string& _animationName, const std::string& _boneNameToAffect)
{
	if ("" == _animationName)
	{
		renderer_->ClearOverrideAnimation();
		return;
	}

	overrideAnimationName_ = _animationName;
	overrideAnimationBoneName_ = _boneNameToAffect;
	renderer_->OverrideFBXAnimation(_animationName, _boneNameToAffect);
}

void Character::Damage(float _Amount, IUnit* _Target)
{
	LumiaLevel* level = GetLevelConvert<LumiaLevel>();

	if (0.0f >= _Amount)
	{
		return;
	}

	stat_.HP -= _Amount;

	if (stat_.HP <= 0.f && false == isPlayerDead_)
	{


		if (GameServer::GetInstance()->IsOpened())
		{
			this->SetCharacterDeath();

			CharDeathPacket deathpacket;
			deathpacket.SetTargetIndex(myIndex_);

			if (_Target->UnitType_ == UnitType::CHARACTER)
			{
				Character* charTmp = dynamic_cast<Character*>(_Target);
				int tmpIndex = charTmp->GetIndex();
				deathpacket.SetFraggerType(static_cast<int>(UnitType::CHARACTER));
				deathpacket.SetFraggerIndex(tmpIndex);
			}
			else if (_Target->UnitType_ == UnitType::MONSTER)
			{
				// animal
				deathpacket.SetFraggerType(static_cast<int>(UnitType::MONSTER));
				deathpacket.SetFraggerIndex(-2);
			}

			FT::SendPacket(deathpacket);

		}
		else if (GameClient::GetInstance()->IsConnected())
		{
			CharDeathPacket deathpacket;
			deathpacket.SetTargetIndex(myIndex_);

			if (_Target->UnitType_ == UnitType::CHARACTER)
			{
				Character* charTmp = dynamic_cast<Character*>(_Target);
				int tmpIndex = charTmp->GetIndex();
				deathpacket.SetFraggerType(static_cast<int>(UnitType::CHARACTER));
				deathpacket.SetFraggerIndex(tmpIndex);
			}
			else if (_Target->UnitType_ == UnitType::MONSTER)
			{
				// animal
				deathpacket.SetFraggerType(static_cast<int>(UnitType::MONSTER));
				deathpacket.SetFraggerIndex(-2);
			}

			FT::SendPacket(deathpacket);
		}
	}


	CharStatPacket packet;
	packet.SetStat(stat_);
	packet.SetTargetIndex(GetIndex());

	FT::SendPacket(packet);
}

void Character::LevelUP(LevelUPData _Data)
{
	// 레벨이없네!!!!!
	//stat_.Level += 1;
	stat_.AttackPower += _Data.AttackPower_;					// 공격력
	stat_.HPMax += _Data.HP_;									// 체력(최대체력)
	stat_.HPRegeneration += _Data.HPRegeneration_;				// 체력재생
	stat_.SPMax += _Data.SP_;									// 스태미나(최대스태미나)
	stat_.SPRegeneration += _Data.SPRegeneration_;				// 스태미나재생
	stat_.Defence += _Data.Defence_;							// 방어력
	stat_.AttackSpeed += _Data.AttackSpeed_;					// 공격속도
	stat_.CriticalChance += _Data.CriticalChance_;				// 치명타
	stat_.MovementSpeed += _Data.MovementSpeed_;				// 이동속도
	stat_.VisionRange += _Data.VisionRange_;					// 시야
}

void Character::Stun(float _stunTime)
{
	mainState_ << "CrowdControlState";
	crowdControlState_ << "Stun";
	timerStun_ = _stunTime;
	changeAnimationWait();
}

void Character::Knockback(float _knockbackTime, float4 _knockbackSpeed)
{
	mainState_.ChangeState("CrowdControlState", true);
	crowdControlState_.ChangeState("Knockback", true);
	timerKnockback_ = _knockbackTime;
	knockbackSpeed_ = _knockbackSpeed;
}

void Character::WallSlam(float _knockbackTime, float4 _knockbackSpeed, float _stunTime)
{
	mainState_ << "CrowdControlState";
	crowdControlState_ << "WallSlam";
	timerKnockback_ = _knockbackTime;
	knockbackSpeed_ = _knockbackSpeed;
	timerStun_ = _stunTime;
}

void Character::Slow(float _slowTime, float _slowRatio)
{
	slowTimer_ = _slowTime;

	if (1.0f < _slowRatio)
	{
		slowRatio_ = 1.0f;
	}
	else
	{
		slowRatio_ = _slowRatio;
	}

	bSlowFlag_ = true;
}

void Character::initInput()
{
	GameEngineInput::GetInst().CreateKey("LButton", VK_LBUTTON);
	GameEngineInput::GetInst().CreateKey("RButton", VK_RBUTTON);
	GameEngineInput::GetInst().CreateKey("Q", 'Q');
	GameEngineInput::GetInst().CreateKey("W", 'W');
	GameEngineInput::GetInst().CreateKey("E", 'E');
	GameEngineInput::GetInst().CreateKey("R", 'R');
	GameEngineInput::GetInst().CreateKey("D", 'D');
	GameEngineInput::GetInst().CreateKey("A", 'A');
	GameEngineInput::GetInst().CreateKey("S", 'S');
	GameEngineInput::GetInst().CreateKey("M", 'M');
	GameEngineInput::GetInst().CreateKey("V", 'V');
	GameEngineInput::GetInst().CreateKey("Tab", VK_TAB);
	GameEngineInput::GetInst().CreateKey("Mixing", VK_OEM_3);	// ~ Key
}

void Character::initState()
{
	mainState_.CreateState(MakeStateWithEnd(Character, NormalState));
	mainState_.CreateState(MakeState(Character, AttackState));
	mainState_.CreateState(MakeState(Character, CrowdControlState));
	mainState_.CreateState(MakeState(Character, CustomState));
	mainState_.CreateState(MakeState(Character, DeathState));

	normalState_.CreateState(MakeState(Character, Watch));
	normalState_.CreateState(MakeState(Character, Stop));
	normalState_.CreateState(MakeState(Character, Run));
	normalState_.CreateState(MakeState(Character, Chase));
	normalState_.CreateState(MakeState(Character, MixItem));
	normalState_.CreateState(MakeState(Character, Craft));
	normalState_.CreateState(MakeState(Character, Cook));

	attackState_.CreateState(MakeState(Character, BasicAttack));
	attackState_.CreateState(MakeState(Character, BasicAttacking));
	attackState_.CreateState(MakeState(Character, QSkill));
	attackState_.CreateState(MakeState(Character, WSkill));
	attackState_.CreateState(MakeState(Character, ESkill));
	attackState_.CreateState(MakeState(Character, RSkill));
	attackState_.CreateState(MakeState(Character, DSkill));

	crowdControlState_.CreateState(MakeState(Character, Stun));
	crowdControlState_.CreateState(MakeState(Character, Knockback));
	crowdControlState_.CreateState(MakeState(Character, WallSlam));

	deathState_.CreateState(MakeState(Character, PlayerAlive));
	deathState_.CreateState(MakeState(Character, PlayerDeath));
	deathState_.CreateState(MakeState(Character, PlayerWinner));


	mainState_ << "NormalState";

	normalState_ << "Watch";

	crowdControlState_ << "Stun";

	deathState_ << "PlayerAlive";
}


void Character::inputProcess(float _deltaTime)
{
	if (mouse_ == nullptr)
	{
		return;
	}
	GameEngineCollision* rayCol = mouse_->GetRayCollision();

	bool result = false;
	float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.f);
	if (GameEngineInput::Press("LButton") || GameEngineInput::Down("LButton"))
	{

		IUnit* otherCharacter = getMousePickedCharacter();
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

	float4 moveSpeed = (direction_ * stat_.MovementSpeed * stat_.MovementRatio) * _deltaTime;
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
	if (bFocused_)
	{
		if (true == GameEngineInput::GetInst().Down("Q"))
		{
			mainState_.ChangeState("AttackState", true);
			attackState_.ChangeState("QSkill", true);
			return;
		}

		if (true == GameEngineInput::GetInst().Down("W"))
		{
			mainState_.ChangeState("AttackState", true);
			attackState_.ChangeState("WSkill", true);
			return;
		}

		if (true == GameEngineInput::GetInst().Down("E"))
		{
			mainState_.ChangeState("AttackState", true);
			attackState_.ChangeState("ESkill", true);
			return;
		}

		if (true == GameEngineInput::GetInst().Down("R"))
		{
			mainState_.ChangeState("AttackState", true);
			attackState_.ChangeState("RSkill", true);
			return;
		}

		if (true == GameEngineInput::GetInst().Down("D"))
		{
			mainState_.ChangeState("AttackState", true);
			attackState_.ChangeState("DSkill", true);
			return;
		}

		if (true == GameEngineInput::Down("Mixing"))
		{
			// 큐에 들어있는 조합을 실행
			// Mixing State 따로 만들 예정
			//mixingItem();
			normalState_ << "MixItem";
			return;
		}
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
	//destination_ = transform_.GetWorldPosition();
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

void Character::startCustomState()
{
}

void Character::updateCustomState(float _deltaTime)
{
	onUpdateCustomState(_deltaTime);
}

void Character::startDeathState()
{
	changeDeathAnimation();
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

	//if (distance > stat_.VisionRange)
	//{
	//	normalState_ << "Watch";
	//	target_ = nullptr;
	//	return;
	//}

	if (distance < stat_.AttackRange)
	{
		setRotationTo(targetPosition, playerPosition);

		if (attackCooldown_ > 1.0f / stat_.AttackSpeed)
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

void Character::startMixItem()
{

}

void Character::updateMixItem(float _deltaTime)
{
	if (queueItemMixing_.empty())
	{
		normalState_ << "Watch";
		return;
	}

	QueueItem item = queueItemMixing_.front();
	ItemType itemType = item.CI_.left_->GetItemType();

	if (itemType == ItemType::USEABLE)
	{
		normalState_ << "Cook";
	}
	else
	{
		normalState_ << "Craft";
	}
}

void Character::startCraft()
{
	changeAnimationCraft();
	FT::PlaySoundAndSendPacket("chracter_Craft_Tool.wav", transform_.GetWorldPosition());
}

void Character::updateCraft(float _deltaTime)
{
	if (renderer_->IsCurrentAnimationEnd())
	{
		mixingItem();
		normalState_ << "Watch";
		return;
	}

	if (normalState_.GetTime() > 8.0f)
	{
		mixingItem();
		normalState_ << "Watch";
		return;
	}
}

void Character::startCook()
{
	changeAnimationCook();
	FT::PlaySoundAndSendPacket("chracter_Craft_Food.wav", transform_.GetWorldPosition());
}

void Character::updateCook(float _deltaTime)
{
	if (renderer_->IsCurrentAnimationEnd())
	{
		mixingItem();
		normalState_ << "Watch";
		return;
	}

	if (normalState_.GetTime() > 6.0f)
	{
		mixingItem();
		normalState_ << "Watch";
		return;
	}
}

void Character::startStun()
{
	changeAnimationWait();
}

void Character::updateStun(float _deltaTime)
{
	timerStun_ -= _deltaTime;
	if (timerStun_ <= 0.0f)
	{
		destination_ = transform_.GetWorldPosition();
		destinations_.clear();
		mainState_ << "NormalState";
		return;
	}
}

void Character::startKnockback()
{
	changeAnimationWait();
}

void Character::updateKnockback(float _deltaTime)
{
	timerKnockback_ -= _deltaTime;
	if (timerKnockback_ <= 0.0f)
	{
		destination_ = transform_.GetWorldPosition();
		destinations_.clear();
		mainState_ << "NormalState";
		return;
	}

	transform_.AddWorldPosition(knockbackSpeed_ * _deltaTime);
}

void Character::startWallSlam()
{
	changeAnimationWait();
}

void Character::updateWallSlam(float _deltaTime)
{
	timerKnockback_ -= _deltaTime;
	if (timerKnockback_ <= 0.0f)
	{
		destination_ = transform_.GetWorldPosition();
		destinations_.clear();
		mainState_ << "NormalState";
		return;
	}

	float4 moveSpeed = knockbackSpeed_ * _deltaTime;
	float4 nextMovePosition = transform_.GetWorldPosition() + moveSpeed;

	float temp;
	if (true == currentMap_->GetNavMesh()->CheckIntersects(nextMovePosition + float4{ 0.0f, FT::Map::MAX_HEIGHT, 0.0f }, float4::DOWN, temp))
	{
		GetTransform()->SetWorldPosition(nextMovePosition);
	}
	else
	{
		Stun(timerStun_);
		GameEngineSoundManager::GetInstance()->PlaySoundByName("hyunwoo_Skill03_Hit.wav");
		PacketSoundPlay packet;
		packet.SetSound("hyunwoo_Skill03_Hit.wav", transform_.GetWorldPosition());

		FT::SendPacket(packet);
	}
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

	if (attackTime_ < stat_.AttackStartTime / stat_.AttackSpeed)
	{
		GameEngineCollision* rayCol = mouse_->GetRayCollision();

		bool result = false;
		float4 mousePosition = mouse_->GetIntersectionYAxisPlane(transform_.GetWorldPosition().y, 2000.f);
		if (GameEngineInput::Press("LButton") || GameEngineInput::Down("LButton"))
		{

			IUnit* otherCharacter = getMousePickedCharacter();

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

	if (attackTime_ > stat_.AttackStartTime / stat_.AttackSpeed)
	{
		attackState_ << "BasicAttacking";
		return;
	}
}

void Character::startBasicAttacking()
{
	onStartBasicAttacking(target_);
}

void Character::updateBasicAttacking(float _deltaTime)
{
	attackTime_ += _deltaTime;
	if (attackTime_ > stat_.AttackEndTime / stat_.AttackSpeed)
	{
		attackCooldown_ = 0.0f;
		mainState_ << "NormalState";
		return;
	}

	onUpdateBasicAttacking(target_, _deltaTime);
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

void Character::startPlayerAlive()
{
	//  MainState::Death 의 아무 것도 안 하는 서브 스테이트입니다.
	// 이후 죽을 때만 PlayerDeathState 로 체인지 됩니다.
}

void Character::updatePlayerAlive(float _deltaTime)
{
	return;
}

void Character::startPlayerDeath()
{
	LumiaLevel* level = GetLevelConvert<LumiaLevel>();
	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();

	stat_.HP = 0.0f;
	collision_->Off();

	level->SubtractSurvivorCount();
	// 총 플레이어 중 몇 위인가?
	int myRank = pm->GetPlayerList().size();

	for (int i = 0; i < pm->GetPlayerList().size(); i++)
	{
		if (i == myIndex_)
		{
			continue;
		}

		if (pm->GetPlayerList()[i].stat_->HP <= 0.0f)
		{
			myRank--;
		}
	}

	if (bFocused_)
	{
		uiController_->GetWinLoseUI()->SetPortrait(GetJobType(), false);

		if (fraggerIndex_ == -1)
		{
			uiController_->GetWinLoseUI()->SetText("플레이어가 자살했습니다." + std::to_string(myRank) + " \/ " + std::to_string(pm->GetPlayerList().size()));
		}
		else if (-2 == fraggerIndex_)
		{
			uiController_->GetWinLoseUI()->SetText("플레이어가 몬스터에게 살해당했습니다." + std::to_string(myRank) + " \/ " + std::to_string(pm->GetPlayerList().size()));
		}
		else
		{
			uiController_->GetWinLoseUI()->SetText(pm->GetPlayerList()[fraggerIndex_].playerNickname_ + " 에게 살해당했습니다.\n" + std::to_string(myRank) + " \/ " + std::to_string(pm->GetPlayerList().size()));
		}

		uiController_->GetWinLoseUI()->Activate();
	}


	onStartDeath();
}

void Character::updatePlayerDeath(float _deltaTime)
{

	onUpdateDeath(_deltaTime);
}

void Character::startPlayerWinner()
{

	PlayerInfoManager* pm = PlayerInfoManager::GetInstance();
	LumiaLevel* level = GetLevelConvert<LumiaLevel>();

	int myRank = pm->GetPlayerList().size();

	uiController_->GetWinLoseUI()->SetWinner();
	uiController_->GetWinLoseUI()->SetPortrait(GetJobType(), true);
	uiController_->GetWinLoseUI()->SetText("승리자 : " + pm->GetMyPlayer().playerNickname_ + "\n마지막까지 생존하였습니다.");
	uiController_->GetWinLoseUI()->Activate();

	changeAnimationWait();
}

void Character::updatePlayerWinner(float _deltaTime)
{
}

void Character::PlayEffect(const std::string& _effectName)
{
	onPlayEffect(_effectName);
}

void Character::SlowCheck(float _DeltaTIme)
{
	// 슬로우 같은건 CC로 퉁칠수가 없습니다.
	// (걸린 와중에도 이동속도만 느려지지 모든 행동을 할 수 있기 때문)

	// 따라서 업데이트 중에 플래그와 슬로우 지속시간을 체크하며, 주어진 슬로우 비율만큼 이동속도를 늦춥니다.
	int a = 0;

	if (true == bSlowFlag_)
	{
		stat_.MovementRatio = (1.0f - slowRatio_);

		if (slowTimer_ <= 0.0f)
		{
			slowTimer_ = 0.0f;
			stat_.MovementRatio = 1.0f;
			bSlowFlag_ = false;
			return;
		}

		slowTimer_ -= _DeltaTIme;

	}

}
